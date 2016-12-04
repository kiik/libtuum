
#include "tuum_draw.hpp"
#include "tuum_vision.hpp"

#include "tuum_VisionFilter.hpp"

namespace tuum {

  const size_t BLOB_MIN_AREA = 10;
  const double BLOB_MIN_DENSITY = 0.3;
  const double BLOB_MAX_DENSITY = 1.0;

  void VisionFilter::initYUVClassifiers()
  {
    mClss.clear();
    mFilter.clear();

    ColorClass CLS_BALL = {99,98,150,194,108,204}; // {94,97,141, 221,115,189};
    CLS_BALL.name = "cBall";
    CLS_BALL.type = Blob::TypeId::Ball;
    mClss.push_back(CLS_BALL);

    ColorClass CLS_GOAL_BLUE = {32,127,90,167,161,113};
    CLS_GOAL_BLUE.name = "cGoalBlue";
    CLS_GOAL_BLUE.type = Blob::TypeId::Goal_Blue;
    mClss.push_back(CLS_GOAL_BLUE);

    ColorClass CLS_GOAL_YEL = {125,94,139,228,103,147};
    CLS_GOAL_YEL.name = "cGoalYellow";
    CLS_GOAL_YEL.type = Blob::TypeId::Goal_Yellow;
    mClss.push_back(CLS_GOAL_YEL);
  }

  int VisionFilter::updateYUVClassifier(ColorClass cls)
  {
    filter_t::ColorClass* ptr;
    for(auto it = mClss.begin(); it != mClss.end(); it++) {
      ptr = &(*it);
      if(ptr->id != cls.id) continue;
      mFilter.removeClass(*ptr);
      mFilter.addClass(cls);
      *ptr = cls;
      return 0;
    }

    return -1;
  }

  void VisionFilter::insertClassifiers()
  {
    filter_t::ColorClass* ptr;
    for(auto it = mClss.begin(); it != mClss.end(); it++) {
      ptr = &(*it);
      mFilter.addClass(*ptr);
    }
  }

  VisionFilter::VisionFilter()
  {
    initYUVClassifiers();
    insertClassifiers();
    mFilter.dump();

    /*
    {
      json dat;
      toJSON(dat);
      fromJSON(dat);
    }
    {
      json dat;
      toJSON(dat);
    }*/

    mDebugTmr.start(1000);
  }

  int VisionFilter::apply(image_t in)
  {
    mBlobBuffer.clear();
    vision::parse_blobs(in, mFilter, mBlobBuffer);

    mBlobs.clear();
    for(auto it = mBlobBuffer.begin(); it != mBlobBuffer.end(); it++) {
      if(it->rect.getArea() < BLOB_MIN_AREA) continue;
      if(it->getDensity() < BLOB_MIN_DENSITY) continue;
      if(it->getDensity() > BLOB_MAX_DENSITY) continue;

      blob_t blob = (blob_t)(*it);

      uint32_t cls = it->rls.back().cls;

      Blob::TypeId t = getType(cls);
      if(t == Blob::TypeId::None) continue;
      mBlobs.push_back(Blob(blob, t));
    }

    /*
    if(mDebugTmr.isTime()) {
      printf("%lu\n", mBlobs.size());

      for(auto it = mBlobs.begin(); it != mBlobs.end(); it++) {
        printf("d:%.2f\n", it->getDensity());
      }

      mDebugTmr.start(1000);
    }
    */
    return 0;
  }

  int VisionFilter::parse(image_t in)
  {
    mBlobBuffer.clear();
    vision::parse_blobs(in, mBlobBuffer);

    mBlobs.clear();
    for(auto it = mBlobBuffer.begin(); it != mBlobBuffer.end(); it++) {
      if(it->rect.getArea() < BLOB_MIN_AREA) continue;
      if(it->getDensity() < BLOB_MIN_DENSITY) continue;
      if(it->getDensity() > BLOB_MAX_DENSITY) continue;

      blob_t blob = (blob_t)(*it);

      uint32_t cls = it->rls.back().cls;

      Blob::TypeId t = getType(cls);
      if(t == Blob::TypeId::None) continue;
      mBlobs.push_back(Blob(blob, t));
    }

    return 0;
  }

  Blob::TypeId VisionFilter::getType(uint32_t cls) {
    for(auto cls_it = mClss.begin(); cls_it != mClss.end(); cls_it++) {
      if(cls_it->id != cls) continue;
      return cls_it->type;
    }

    return Blob::TypeId::None;
  }

  BlobSet* VisionFilter::getBlobs() { return &mBlobs; }

  void VisionFilter::addBlobDebugLayer(image_t out)
  {
    for(auto it = mBlobs.begin(); it < mBlobs.end(); it++) {
      draw_rect(out, it->getRect());
    }
  }

  void VisionFilter::toJSON(json& out) {
    out["classes"] = json::array();

    for(auto it = mClss.begin(); it != mClss.end(); it++) {
      json cls;
      cls["id"] = it->id;
      cls["name"] = it->name;
      cls["range"] = {it->mn[0], it->mn[1], it->mn[2], it->mx[0], it->mx[1], it->mx[2]};
      out["classes"].push_back(cls);
    }
  }

  //FIXME: Set blob class type id
  void VisionFilter::fromJSON(json in) {
    mClss.clear();
    mFilter.clear();

    ColorClass cls;
    for (json::iterator it = in["classes"].begin(); it != in["classes"].end(); ++it) {
      auto dat = *it;

      cls.id = dat["id"];
      cls.name = dat["name"];

      for(uint8_t i = 0; i < 6; i++)
        cls.mn[i] = dat["range"][i];

      mClss.push_back(cls);
      mFilter.addClass(cls);
    }
  }

}
