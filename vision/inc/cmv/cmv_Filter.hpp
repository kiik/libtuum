
#ifndef TUUM_CMV_FILTER_H
#define TUUM_CMV_FILTER_H

#include "cmv/cmv_data.hpp"

namespace tuum { namespace CMV {

  class FilterBase
  {
  public:
    FilterBase();

    //virtual int addClass(ColorClass, uint32_t&);
    //virtual int rmvClass(ColorClass);

    int createId(uint32_t&);

    virtual void clear() {}

    virtual uint32_t operator()(uint8_t, uint8_t, uint8_t) { return 0; }

  protected:
    size_t id_seq;
  };

  class YUVFilter : public FilterBase
  {
  public:
    struct ColorClass {
      uint8_t mn[3], mx[3];
      uint32_t id = 0;

      ColorClass():
        ColorClass(0, 0, 0, 0, 0, 0)
      {

      }

      ColorClass(uint8_t ym, uint8_t um, uint8_t vm, uint8_t yx, uint8_t ux, uint8_t vx)
      {
        mn[0] = ym; mn[1] = um; mn[2] = vm;
        mx[0] = yx; mx[1] = ux; mx[2] = vx;
      }

    };

    YUVFilter();

    int addClass(ColorClass&);
    int removeClass(ColorClass&);

    void clear();

    uint32_t operator()(uint8_t, uint8_t, uint8_t);

  public:
    uint32_t clss_Y[16], clss_U[32], clss_V[32];

  };

  class UVFilter : public FilterBase
  {
  public:
    struct UVSpan {
      uint8_t Y;
      uint8_t U[2], V[2];

      UVSpan(uint16_t y, uint32_t um, uint32_t ux, uint32_t vm, uint32_t vx)
      {
        Y = y;
        U[0] = um; U[1] = ux;
        V[0] = vm; V[1] = vx;
      }

    };

    struct ColorClass {
      std::vector<UVSpan> clss;
      uint32_t id = 0;
    };

    UVFilter();

    int addClass(ColorClass&);

    void clear();

    uint32_t operator()(uint8_t, uint8_t, uint8_t);

  public:
    uint32_t clss_U[16][32], clss_V[16][32];

  };

}}

#endif
