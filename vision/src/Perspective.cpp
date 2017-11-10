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

  Vec2i virtualToReal(const unsigned int &x, const unsigned int &y, const unsigned int &cameraID) {
    // ActualDistance = A + B / PixelVerticalCoord
    double verticalCoordinate = A[cameraID] + B[cameraID] / y;
    // ActualRight = C * PixelRight / PixelVerticalCoord
    double horisontalCoordinate = C[cameraID] * ((double) x - CAMERA_WIDTH / 2.0) / y;
    return Vec2i(horisontalCoordinate, verticalCoordinate);
  }

  Vec2i virtualToReal(const Vec2i &point, const unsigned int &cameraID) {
    return virtualToReal(point.x, point.y, cameraID);
  }

  Vec2i virtualToReal(const Vec2i *point, const unsigned int &cameraID) {
    return virtualToReal(point->x, point->y, cameraID);
  }

  Vec2i realToVirtual(const double &x, const double &y, const unsigned int &cameraID) {
    // PixelVerticalCoord = B / (ActualDistance - A)
    unsigned int verticalCoordinate = B[cameraID] / (y - A[cameraID]);
    // PixelRight = ActualRight * PixelVerticalCoord / C
    unsigned int horisontalCoordinate = x * verticalCoordinate / C[cameraID] + CAMERA_WIDTH / 2.0;
    return Vec2i(horisontalCoordinate, verticalCoordinate);
  }

  Vec2i realToVirtual(const Vec2i &point, const unsigned int &cameraID) {
    return realToVirtual(point.x, point.y, cameraID);
  }

  Vec2i imageToWorld(Vec2i p) {
    return Vec2i((CAMERA_HEIGHT - p.y - 400) / 10.0, (CAMERA_WIDTH / 2.0 - p.x) * 0.714 / 10.0);
  }

}}}
