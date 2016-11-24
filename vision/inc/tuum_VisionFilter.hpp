
#ifndef TUUM_VF_H
#define TUUM_VF_H

#include "tuum_json.hpp"
#include "tuum_math.hpp"
#include "tuum_cmv.hpp"

#include "Entity.hpp"

namespace tuum {

  class VisionFilter : public JSONSerializable<VisionFilter>
  {
  public:
    typedef CMV::YUVFilter filter_t;

    struct ColorClass : public filter_t::ColorClass
    {
      using filter_t::ColorClass::ColorClass;

      std::string name;
      Entity::TypeId type;
    };

    typedef std::vector<ColorClass> ColorClsSet;


    VisionFilter();

    void initUVClassifiers();
    void initYUVClassifiers();

    int updateYUVClassifier(ColorClass);

    void insertClassifiers();

    int apply(image_t);

    BlobSet* getBlobs();

    void addBlobDebugLayer(image_t);

    void toJSON(json&);
    void fromJSON(json);

  protected:
    ColorClsSet mClss;   // Color classes
    BlobSet mBlobs;

    CMV::YUVFilter mFilter;           // Color filtering machine
    CMV::BlobSet mBlobBuffer;

    Timer mDebugTmr;
  };

}

#endif
