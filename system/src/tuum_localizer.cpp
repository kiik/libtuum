
#include "tuum_localizer.hpp"

namespace tuum {

  Subsystem::TypeVar Localizer::Type;

  Localizer::Localizer():
    Subsystem("Localizer")
  {

  }

  Vec2i Localizer::toAbsolute(Vec2i v) {
    // #TODO: return v - mRobotPos;
    return v - Vec2i({200, 200});
  }

  void Localizer::setup() {

  }

  void Localizer::process() {

  }

}
