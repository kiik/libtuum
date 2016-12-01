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

//#define TUUM_VISION_BENCH
//#define TUUM_VISION_PASS_BENCH

namespace tuum {

  const char* DEF_PPL_NONE = "./assets/empty.ppl";

  const char* DEF_PPL_RTEX_FOOTBALL = "./assets/rtex_football.ppl";
  const char* DEF_PPL_RTEX_FOOTBALL_IM = "./assets/rtex_football_image.ppl";

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
    m_threshold_enable(true), m_gpu_enable(true)
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
    m_plRtexFootball_im.use(gLdr);
    m_plRtexFootball_im.load(DEF_PPL_RTEX_FOOTBALL_IM, "RobotexFootballPipe");

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
#ifdef TUUM_VISION_BENCH
    size_t t0, t1, t2, t3;
#endif

    int res;

#ifdef TUUM_VISION_BENCH
    t0 = millis();
#endif

    if(nextFrame() > 0) {
      //if(!pplIsReady()) return -2;
#ifdef TUUM_VISION_BENCH
      t1 = millis();
#endif

      res = doFramePass();

#ifdef TUUM_VISION_BENCH
      t2 = millis();
#endif

#ifdef TUUM_VISION_BENCH
      printf("[Vision - stat]frame %lums, pass %lums\n", (t1 - t0), (t2 - t1));
#endif

      if(res < 0) {
        RTXLOG(format("'doFramePass' error %i", res), LOG_ERR);
        return;
      }
    }
  }

  bool Visioning::pplIsReady() {
    if(!m_plRtexFootball.isReady()) return false;
    if(!m_plRtexFootball_im.isReady()) return false;
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

  float value = 0.5;
  bool m_ppl_config_dirty = true;

  void Visioning::configure(const json& dat) {
    auto ppl = m_plRtexFootball.get();

    if(dat.find("gpu_en") != dat.end()) {
      m_gpu_enable = dat["gpu_en"].get<bool>();
    }

    if(dat.find("thr_en") != dat.end()) {
      m_threshold_enable = dat["thr_en"].get<bool>();
    }

    auto it = dat.find("FisheyePwr");
    if(it != dat.end()) {
      value = (it->get<int>() / 1000.0);
      m_ppl_config_dirty = true;
      //Glip::CorePipeline::Filter& filter = (*ppl)[ppl->getElementID("FisheyeFilter")];
      //filter.program().setVar("uPower", GL_FLOAT, value);
      RTXLOG(format("Fisheye power updated to %.2f", value));
    }

  }


  int Visioning::doFramePass()
  {
#ifdef TUUM_VISION_PASS_BENCH
    size_t t0, t1, t2, t3, t4, t5;
#endif

    bool gpu_threshold = thresholdGPUEnabled() && thresholdPassEnabled();

    Pipeline *ppl, *nppl;
    try
    {
      if(gpu_threshold)
        ppl = m_plRtexFootball.get();
      else
        ppl = m_plRtexFootball_im.get();

      /*
      if(m_ppl_config_dirty) {
        Glip::CorePipeline::Filter& filter = (*ppl)[ppl->getElementID("FisheyeFilter")];
        filter.program().setVar("uPower", GL_FLOAT, value);
        m_ppl_config_dirty = false;
      }*/

#ifdef TUUM_VISION_PASS_BENCH
      t0 = millis();
#endif

      size_t id;
      while(!m_iFrameLock.try_lock()) {};
      m_tex->write(m_iFrame->data, GL_RGB, GL_UNSIGNED_BYTE);
      id = m_iFrame->id;
      m_iFrameLock.unlock();

#ifdef TUUM_VISION_PASS_BENCH
      t1 = millis();
#endif

      if(gpu_threshold)
        (*ppl) << (*m_tex) << Pipeline::Process;
      else
        (*ppl) << (*m_tex) << Pipeline::Process; // << (*txDiscYUV)

#ifdef TUUM_VISION_PASS_BENCH
      t2 = millis();
#endif

      while(!m_iFrameLock.try_lock()) {};
      ppl->out(0).read(m_iFrame->data);

#ifdef TUUM_VISION_PASS_BENCH
      t3 = millis();
#endif

      if(thresholdPassEnabled()) {
        if(gpu_threshold)
          mVisionFilter.parse(m_iFrame);
        else
          mVisionFilter.apply(m_iFrame);

        //mVisionFilter.addBlobDebugLayer(m_iFrame);
      }

#ifdef TUUM_VISION_PASS_BENCH
      t4 = millis();
#endif

      while(!m_oFrameLock.try_lock()) {};
      m_oFrame->id = id;
      m_iFrame->paste(*m_oFrame);

      m_iFrameLock.unlock();
      m_oFrameLock.unlock();

#ifdef TUUM_VISION_PASS_BENCH
      t5 = millis();
#endif

#ifdef TUUM_VISION_PASS_BENCH
      printf("input-copy: %lums, ppl-run: %lums, gl-read: %lums, clsfy(%s): %lums, output-copy: %lums\n", (t1 - t0), (t2 - t1), (t3 - t2), (gpu_threshold ? "gpu" : "cpu"), (t4 - t3), (t5 - t4));
#endif
      return 1;
    } catch(Glip::Exception& e) {
      std::cerr << e.what() << std::endl;
      return -3;
    }

    return 0;
  }

  bool Visioning::thresholdPassEnabled() {
    return m_threshold_enable;
  }

  bool Visioning::thresholdGPUEnabled() {
    return m_gpu_enable;
  }

  void Visioning::toJSON(json& out) {
    out["thr_en"] = m_threshold_enable;
    out["gpu_en"] = m_gpu_enable;
  }

}
