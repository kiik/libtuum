/**
 *  @file Perspective.cpp
 *  Perspective class for coordinate mapping.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.2
 *  @date 5 December 2015
 */

#include "Perspective.hpp"

#include "cameraConstants.hpp"
#include "tuum_platform.hpp"


namespace tuum { namespace Vision { namespace Perspective {

  std::vector<double> A;
  std::vector<double> B;
  std::vector<double> C;

  void setup() {
    A.push_back((double) tuum::gC.getInt("Perspective.FirstA"));
    A.push_back((double) tuum::gC.getInt("Perspective.SecondA"));
    B.push_back((double) tuum::gC.getInt("Perspective.FirstB"));
    B.push_back((double) tuum::gC.getInt("Perspective.SecondB"));
    C.push_back((double) tuum::gC.getInt("Perspective.FirstC"));
    C.push_back((double) tuum::gC.getInt("Perspective.SecondC"));
  }

  vec2 virtualToReal(const unsigned int &x, const unsigned int &y, const unsigned int &cameraID) {
    // ActualDistance = A + B / PixelVerticalCoord
    double verticalCoordinate = A[cameraID] + B[cameraID] / y;
    // ActualRight = C * PixelRight / PixelVerticalCoord
    double horisontalCoordinate = C[cameraID] * ((double) x - CAMERA_WIDTH / 2.0) / y;
    return vec2(horisontalCoordinate, verticalCoordinate);
  }

  vec2 virtualToReal(const vec2u &point, const unsigned int &cameraID) {
    return virtualToReal(point.x, point.y, cameraID);
  }

  vec2 virtualToReal(const vec2 *point, const unsigned int &cameraID) {
    return virtualToReal(point->getX(), point->getY(), cameraID);
  }

  vec2u realToVirtual(const double &x, const double &y, const unsigned int &cameraID) {
    // PixelVerticalCoord = B / (ActualDistance - A)
    unsigned int verticalCoordinate = B[cameraID] / (y - A[cameraID]);
    // PixelRight = ActualRight * PixelVerticalCoord / C
    unsigned int horisontalCoordinate = x * verticalCoordinate / C[cameraID] + CAMERA_WIDTH / 2.0;
    return vec2u(horisontalCoordinate, verticalCoordinate);
  }

  vec2u realToVirtual(const vec2 &point, const unsigned int &cameraID) {
    return realToVirtual(point.x, point.y, cameraID);
  }

  vec2i imageToWorld(vec2i p) {
    return vec2i(CAMERA_HEIGHT - p.y - 300, CAMERA_WIDTH / 2.0 - p.x);
  }

}}}
