
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

    if(mDebugTmr.isTime()) {

      printf("Balls: %i\n", mBallDetect.objs.size());

      mDebugTmr.start(4000);
    }
  }

  void EntityFilter::digest(BlobSet* blobs)
  {
    for(auto it = blobs->begin(); it != blobs->end(); it++) {
      switch(it->getType()) {
        case Blob::TypeId::Ball:
          digestBall(*it);
          break;
      }
    }
  }

  int EntityFilter::digestBall(Blob& blob) {
    double d = blob.getDistance(), a = blob.getAngle();
    Ball* b = new Ball(Transform((int)(cos(a)*d), (int)(sin(a)*d)), blob);
    mBallDetect.processProbableEntity(b);
    return 1;
  }

  void EntityFilter::toJSON(json& out) {
    //TODO
  }

  void EntityFilter::fromJSON(json in) {
    //TODO
  }

}
