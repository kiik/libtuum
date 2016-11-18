
#ifndef TUUM_CMV_DATA_H
#define TUUM_CMV_DATA_H

#include "platform.hpp"

namespace tuum { namespace CMV {

  const float U8_16CLS_REDUCTION = 15.0 / 255.0;
  const float U8_32CLS_REDUCTION = 31.0 / 255.0;

  struct ColorClass {
    uint8_t mn[3], mx[3];
  };

  // Runline type
  class rl_t {
  public:
    size_t x0 = 0, x1 = 0, y = 0;
    uint32_t cls = 0;

    bool isTouching(rl_t);
  };

  // TODO: Refactor into platform or math library
  struct rect_t {
    size_t x0 = 0, y0 = 0, x1 = 0, y1 = 0;

    rect_t()
    {
      x0 = 0; y0 = 0; x1 = 0; y1 = 0;
    }

    rect_t(size_t X0, size_t Y0, size_t X1, size_t Y1)
    {
      x0 = X0; y0 = Y0; x1 = X1; y1 = Y1;
    }

    size_t getArea() {
      return (y1 - y0) * (x1 - x0);
    }

  };

  typedef std::vector<rl_t> RunlineSet;

  struct blob_t {
    RunlineSet rls; // Runlines

    rect_t rect;

    void addRunline(rl_t rl) {
      if(rl.x0 < rect.x0) rect.x0 = rl.x0;
      if(rl.x1 > rect.x1) rect.x1 = rl.x1;

      if(rls.size() == 0) {
        rect.x0 = rl.x0;
        rect.y0 = rl.y;
        rect.y1 = rl.y;
      }
      else
      {
        if(rl.y > rect.y1) rect.y1 = rl.y;
        else if(rl.y < rect.y0) rect.y0 = rl.y;
      }

      rls.push_back(rl);
    }

    size_t getArea() {
      return rect.getArea();
    }

  };

  typedef std::vector<blob_t> BlobSet;

  struct blob_field_t {
     BlobSet data;
  };

}}

#endif