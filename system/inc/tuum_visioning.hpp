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

#include <opencv/cv.h>
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

using namespace boost;
using namespace tuum::vision;

using json = nlohmann::json;

namespace tuum {

  const uint8_t TUUM_CAM_N = 1;

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

    int doPass(cv::Mat&, cv::Mat&);

    int doFramePass();
    int doEntityPass();

    bool thresholdPassEnabled();
    bool thresholdGPUEnabled();

    static void setup();
    static void process();

    template<typename T>
    T* get() {
      return nullptr;
    }

    void setThresholding(bool s) { m_threshold_enable = s; }

    void configure(const json& dat);

    VisionFilter* getFilter() { return &mVisionFilter; }

    void toJSON(json&);

  protected:
    ImageStream* m_inpStreams[TUUM_CAM_N];
    TxFormat* m_format[TUUM_CAM_N];

    size_t m_cam_N, m_lid;
    image_t m_iFrame, m_oFrame;
    boost::mutex m_iFrameLock, m_oFrameLock;
    Texture* m_tex;

    PipeBase m_plRtexFootball;    // Corrects image, subsamples colors, classifies pixels
    PipeBase m_plRtexFootball_im; // Preserves image

  private:
    bool m_threshold_enable, m_gpu_enable;

    VisionFilter mVisionFilter;
  };

}

#endif // TUUM_VISIONING_H

#include "tuum_context.hpp"
