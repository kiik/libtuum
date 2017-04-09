/** @file tuum_lpx.hpp
 *  @brief Library proxy module - handles external library addons.
 *
 *  @author Meelik Kiik ( kiik.meelik@gmail.com )
 */

#ifndef TUUM_LPX_H
#define TUUM_LPX_H

#include "tuum_buff.hpp"

namespace tuum {

  enum TuumAddon {
    TUUM_DB,
    TUUM_OGL,
    TUUM_OCL,
    TUUM_COMM,
  };

  int init_addon(const TuumAddon&);

}

namespace tuum { namespace lpx {

  static void nullfn() {};


  struct addon_t {
    std::string name;
    std::function<void()> setup = nullfn, process = nullfn;

    addon_t(std::string n, std::function<void()> s, std::function<void()> p):
      name(n), setup(s), process(p)
    {

    }
  };

  int register_addon(const addon_t&);

  void init();
  void setup();
  void process();

}}

#endif
