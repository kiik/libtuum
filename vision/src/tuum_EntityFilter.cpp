
#include "tuum_entities.hpp"

#include "tuum_vision.hpp"

#include "tuum_EntityFilter.hpp"

namespace tuum {

  EntityFilter::EntityFilter()
  {
    mDebugTmr.start(4000);
  }

  void EntityFilter::process()
  {
    mBallDetect.update();
    mGoalDetect.update();

    /*
    if(mDebugTmr.isTime()) {
      json dat;
      toJSON(dat);
      printf("dbg: %s\n", dat.dump().c_str());
      mDebugTmr.start(4000);
    }
    */
  }

  void EntityFilter::digest(BlobSet* blobs)
  {
    for(auto it = blobs->begin(); it != blobs->end(); it++) {

      /*
      switch(it->getType()) {
        case Blob::TypeId::Ball:
          digestBall(*it);
          break;
        case Blob::TypeId::Goal_Blue:
        case Blob::TypeId::Goal_Yellow:
          digestGoal(*it);
          break;
      }*/
    }
  }

  int EntityFilter::digestBall(Blob& blob) {
    double d = blob.getDistance(), a = blob.getAngle();
    Ball* b = new Ball(Transform((int)(cos(a)*d), (int)(sin(a)*d)), blob);
    mBallDetect.processProbableEntity(b);
    return 1;
  }

  int EntityFilter::digestGoal(Blob& blob) {
    double d = blob.getDistance(), a = blob.getAngle();
    Goal* obj = new Goal(Transform((int)(cos(a)*d), (int)(sin(a)*d)), blob);
    mGoalDetect.processProbableEntity(obj);
    return 1;
  }

  void EntityFilter::toJSON(json& out) {
    out["entities"] = json::array();

    Ball* ptr;
    Transform* tr;
    for(auto it = mBallDetect.objs.begin(); it != mBallDetect.objs.end(); it++) {
      ptr = (*it);
      tr = ptr->getTransform();


      json o = {
        {"id", ptr->getId()},
        {"health", ptr->getHealth()},
        {"pos", {tr->getX(), tr->getY()}}
      };

      out["entities"].push_back(o);
    }

    out["balls"] = mBallDetect.objs.size();
  }

  void EntityFilter::fromJSON(json in) {
    //TODO
  }

}
