/** @file tuum_vision.cpp
 *  @brief Vision system implementation.
 *
 *  @authors Meelik Kiik
 *  @version 0.1
 *  @date 27. October 2016
 */

#include "mathematicalConstants.hpp"

#include "tuum_logger.hpp"
#include "tuum_fs.hpp"
#include "tuum_ogl.hpp"
#include "tuum_ogl_core.hpp"

#include "Perspective.hpp"

#include "tuum_visioning.hpp"
#include "tuum_localization.hpp"

using namespace tuum::hal;
using namespace std;

namespace tuum {

  const char* DEF_PPL_NONE = "./assets/empty.ppl";

  const char* DEF_PPL_RTEX_FOOTBALL = "./assets/rtex_football.ppl";
  const char* DEF_PPL_IMFORM = "./assets/imformat.ppl";

  const char* DEF_PPL_BLOBS  = "./assets/blobs.ppl";
  const char* DEF_PPL_GEOM = "./assets/geometry.ppl";

  const char* DEF_DISC_YUV = "./assets/dYUV.dat";

  vision::LayoutLoader* gLdr = create_layout_loader();

  File* discYUVfp = NewFile(DEF_DISC_YUV);
  image_t discrete_yuv;

  TxFormat* txDiscYUVFormat = nullptr;
  Texture*  txDiscYUV = nullptr;

  Visioning::Visioning():
    m_cam_N(1), m_lid(0),
    m_threshold_enable(true)
  {
    for(size_t i=0; i < m_cam_N; i++) {
      m_inpStreams[i] = nullptr;
      m_format[i] = nullptr;
    }
  }

  void Visioning::init() {
    // Initialize image buffers
    Env::spawnBuffer(m_iFrame);
    Env::spawnBuffer(m_oFrame);

    // Configure stream settings
    m_inpStreams[0] = hal::hw.getCamera()->getStream();
    m_inpStreams[0]->initBuf(m_iFrame);
    m_inpStreams[0]->initBuf(m_oFrame);
    auto iprop = hal::hw.getCamera()->getFormat();

    // Configure GLiP (OpenGL) texture objects
    m_format[0] = new TxFormat(iprop.width, iprop.height, GL_RGB, GL_UNSIGNED_BYTE);
    m_tex = new Texture(*m_format[0]);

    txDiscYUVFormat = new TxFormat(32*32, 16, GL_RGB, GL_UNSIGNED_BYTE);

    gLdr->addRequiredElement("inputFormat", *m_format[0]);

    // Load graphics pipeline
    m_plRtexFootball.use(gLdr);
    m_plRtexFootball.load(DEF_PPL_RTEX_FOOTBALL, "RobotexFootballPipe");

    initYUVDiscreteLUT();

    RTXLOG("Ready.");
  }

  int Visioning::initYUVDiscreteLUT()
  {
    Env::spawnBuffer(discrete_yuv);
    char* d;
    size_t size;

    if(discYUVfp->bread(d, size) >= 0) {
      discrete_yuv->init(size);
      memcpy((uint8_t*)discrete_yuv->data, (uint8_t*)d, size);

      //printf("%d, %d, %d; len=%lu\n", d[3], d[4], d[5], discrete_yuv->size);

      // Load discrete YUV LUT into GLip (OpenGL) texture.
      txDiscYUV = new Texture(*txDiscYUVFormat);
      txDiscYUV->write(discrete_yuv->data, GL_RGB, GL_UNSIGNED_BYTE);

    } else {
      RTXLOG(format("Discrete YUV space load failed ( '%s' )", DEF_DISC_YUV), LOG_ERR);
      return -1;
    }

    return 0;
  }

  void Visioning::run() {
    if(nextFrame() > 0) {
      //if(!pplIsReady()) return -2;
      doFramePass();
    }
  }

  bool Visioning::pplIsReady() {
    if(!m_plRtexFootball.isReady()) return false;
    if(!m_plImFormat.isReady()) return false;
    //if(!m_plGeom.isReady()) return false;
    return true;
  }

  int Visioning::nextFrame() {
    uint8_t i = 0; //TODO: Multiple camera support
    if(m_inpStreams[i] == nullptr) return -1;

    while(!m_iFrameLock.try_lock()) {}; //TODO: Timeout

    ImageStream* strm = m_inpStreams[i];
    strm->read(m_iFrame);

    int res;
    if(m_iFrame->id != m_lid) res = 1;
    else res = 0;
    m_lid = m_iFrame->id;

    m_iFrameLock.unlock();
    return res;
  }

  int Visioning::readFrame(image_t& out) {
    while(!m_oFrameLock.try_lock()) {};
    m_oFrame->paste(*out);
    m_oFrameLock.unlock();
    return 0;
  }


  bool dbg = false;
  int Visioning::doFramePass()
  {
    Pipeline *ppl, *nppl;
    try
    {
      size_t id;
      while(!m_iFrameLock.try_lock()) {};
      m_tex->write(m_iFrame->data, GL_RGB, GL_UNSIGNED_BYTE);
      id = m_iFrame->id;

      m_iFrameLock.unlock();

      ppl = m_plRtexFootball.get();

      (*ppl) << (*m_tex) << (*txDiscYUV) << Pipeline::Process;

      while(!m_iFrameLock.try_lock()) {};
      ppl->out(0).read(m_iFrame->data);

      // Thresholding
      if(m_threshold_enable)
        mVisionFilter.apply(m_iFrame);

      while(!m_oFrameLock.try_lock()) {};
      m_oFrame->id = id;
      m_iFrame->paste(*m_oFrame);

      m_iFrameLock.unlock();
      m_oFrameLock.unlock();

      return 1;
    } catch(Glip::Exception& e) {
      std::cerr << e.what() << std::endl;
      return -3;
    }

    return 0;
  }

  void Visioning::setup() {
    if(gVision == nullptr) {
      gVision = new Visioning();
      gVision->init();
    }
  }

  void Visioning::process() {
    if(gVision != nullptr) gVision->run();
  }

  Visioning* gVision = nullptr;

}
