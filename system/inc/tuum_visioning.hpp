/** @file tuum_visioning.hpp
 *  Visioning interface declaration.
 *
 *  @authors Ants-Oskar Mäesalu, Meelik Kiik
 *  @version 0.2
 *  @date 4. December 2015
 */

#ifndef TUUM_VISIONING_H
#define TUUM_VISIONING_H

#include <vector>

#include <boost/thread/mutex.hpp>

#include "json.hpp"
#include "hal.hpp"

#include "rtxmath.hpp"
#include "Feature.hpp"
#include "strms/ImageStream.hpp"

#include "tuum_buff.hpp"
#include "tuum_ppl.hpp"

#include "tuum_entities.hpp"

#include "tuum_VisionFilter.hpp"
#include "tuum_EntityFilter.hpp"

#include "tuum_physics.hpp"

using namespace boost;
using namespace tuum::vision;

using json = nlohmann::json;

namespace tuum {

  const uint8_t TUUM_CAM_N = 1;

  class Visioning;

  extern Visioning* gVision;

  class Visioning
  {
  public:

    Visioning();

    void init();
    void run();

    int initYUVDiscreteLUT();

    bool pplIsReady();

    int nextFrame();
    int readFrame(image_t&);

    int doFramePass();
    int doEntityPass();

    bool thresholdPassEnabled();
    bool entityPassEnabled();

    static void setup();
    static void process();

    template<typename T>
    T* get() {
      return nullptr;
    }

    void setThresholding(bool s) { m_threshold_enable = s; }

    void pplConfig(const json& dat);

    VisionFilter* getFilter() { return &mVisionFilter; }

  protected:
    ImageStream* m_inpStreams[TUUM_CAM_N];
    TxFormat* m_format[TUUM_CAM_N];

    size_t m_cam_N, m_lid;
    image_t m_iFrame, m_oFrame;
    mutex m_iFrameLock, m_oFrameLock;
    Texture* m_tex;

    PipeBase m_plRtexFootball; // Color space conversion & simplification
    PipeBase m_plImFormat;     // Get image in display color space

  private:
    bool m_threshold_enable;

    VisionFilter mVisionFilter;
    EntityFilter mEntityFilter;

  public:
    Physics gPhysics;
  };

}

#endif // TUUM_VISIONING_H
