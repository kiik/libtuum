
#include "tuum_perception.hpp"

namespace tuum {

  Subsystem::TypeVar Perception::Type;

  Perception::Perception():
    Subsystem("Perception")
  {

  }

  void Perception::setup() {

  }

  void Perception::process() {

  }

  void Perception::processPoints(loc::PointSet input) {
    mLandmarkStr.stream(input);
  }

}
