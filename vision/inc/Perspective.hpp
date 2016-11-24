/**
 *  @file Perspective.hpp
 *  Perspective class for coordinate mapping.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 4 December 2015
 */

#ifndef RTX_VISION_PERSPECTIVE_H
#define RTX_VISION_PERSPECTIVE_H

#include <vector>

#include "tuum_vector.hpp"

namespace tuum { namespace Vision {

  namespace Perspective {

  extern std::vector<double> A;
  extern std::vector<double> B;
  extern std::vector<double> C;

  // TODO: Maybe move samples here?

  // TODO: Use point class

  void setup();

  vec2 virtualToReal(const unsigned int&, const unsigned int&, const unsigned int&);
  vec2 virtualToReal(const vec2u&, const unsigned int&);
  vec2 virtualToReal(const vec2*, const unsigned int&);

  vec2u realToVirtual(const double&, const double&, const unsigned int&);
  vec2u realToVirtual(const vec2&, const unsigned int&);

  vec2i imageToWorld(vec2i);

  }

}}

#endif // RTX_VISION_PERSPECTIVE_H
