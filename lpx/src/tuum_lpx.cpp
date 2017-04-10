/** @file tuum_lpx.hpp
 *  @brief Library proxy module - handles external library addons.
 *
 *  @author Meelik Kiik ( kiik.meelik@gmail.com )
 */

#include <gtkmm.h>

#include "tuum_logger.hpp"

#include "tuum_fs.hpp"
#include "tuum_db.hpp"
#include "tuum_comm.hpp"

#include "tuum_ogl.hpp"
#include "tuum_ocl.hpp"

#include "tuum_lpx.hpp"

namespace tuum {

  int init_addon(const TuumAddon& addon_type)
  {
    switch(addon_type)
    {
      case TuumAddon::TUUM_DB:
        tuum::db::init();
        break;
      case TuumAddon::TUUM_OGL:
        lpx::register_addon({"tuum::ogl", tuum::ogl_setup, tuum::ogl_process});
        tuum::ogl_init();
        break;
      case TuumAddon::TUUM_OCL:
        lpx::register_addon({"tuum::ocl", tuum::ocl_setup, tuum::ocl_process});
        tuum::ocl_init();
        break;
      case TuumAddon::TUUM_COMM:
        lpx::register_addon({"tuum::comm", tuum::comm::setup, tuum::comm::process});
        tuum::comm::init();
        break;
    }

    return 0;
  }

}

namespace tuum { namespace lpx {

  typedef std::vector<addon_t> AddonSet;

  AddonSet gAddons;

  int register_addon(const addon_t& addon)
  {
    RTXLOG(format("'%s'", addon.name));
    gAddons.push_back(addon);
    return 0;
  }

  int init() {
    return 0;
  }

  void setup() {
    for(auto it = gAddons.begin(); it != gAddons.end(); it++)
      it->setup();
  }

  void process() {
    for(auto it = gAddons.begin(); it != gAddons.end(); it++)
      it->process();
  }

}}
