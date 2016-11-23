
#ifndef TUUM_VFM_H
#define TUUM_VFM_H

#include "tuum_json.hpp"
#include "tuum_cmv.hpp"

namespace tuum {

  class VisionFilter : public JSONSerializable<VisionFilter>
  {
  public:
    typedef CMV::YUVFilter filter_t;

    struct ColorClass : public filter_t::ColorClass
    {
      using filter_t::ColorClass::ColorClass;

      std::string name;
    };

    typedef std::vector<ColorClass> ColorClsSet;

    VisionFilter();

    void initUVClassifiers();
    void initYUVClassifiers();

    int updateYUVClassifier(ColorClass);

    void insertClassifiers();

    int apply(image_t);

    void addBlobDebugLayer(image_t);

    void toJSON(json&);
    void fromJSON(json);

  protected:
    ColorClsSet mClss;   // Color classes
    CMV::YUVFilter mFilter;           // Color filtering machine
    CMV::BlobSet mBlobs, mBlobBuffer;

    Timer mDebugTmr;
  };

}

#endif
