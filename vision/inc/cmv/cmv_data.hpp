
#ifndef TUUM_CMV_DATA_H
#define TUUM_CMV_DATA_H

#include "platform.hpp"
#include "tuum_math.hpp"

namespace tuum { namespace CMV {

  const float U8_16CLS_REDUCTION = 15.0 / 255.0;
  const float U8_32CLS_REDUCTION = 31.0 / 255.0;

  // Runline type
  class rl_t {
  public:
    int x0 = 0, x1 = 0, y = 0;
    uint32_t cls = 0;

    bool isTouching(rl_t);
  };

  typedef std::vector<rl_t> RunlineSet;

  struct blob_t : public tuum::blob_t
  {
    RunlineSet rls; // Runlines

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
      realArea += (rl.x1 - rl.x0);
    }

  };

  typedef std::vector<blob_t> BlobSet;

  struct blob_field_t {
     BlobSet data;
  };

}}

#endif
