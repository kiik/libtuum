/** @file tuum_lpx.hpp
 *  @brief Library proxy module - handles external library addons.
 *
 *  @author Meelik Kiik ( kiik.meelik@gmail.com )
 */

#include <gtkmm.h>

#include "tuum_logger.hpp"

#include "tuum_fs.hpp"
#include "tuum_db.hpp"

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
        tuum::ogl_init();
        lpx::register_addon({"tuum::ocl", tuum::ogl_setup, tuum::ogl_process});
        break;
      case TuumAddon::TUUM_OCL:
        tuum::ocl_init();
        lpx::register_addon({"tuum::ocl", tuum::ocl_setup, tuum::ocl_process});
        break;
    }

    return 0;
  }

}

namespace tuum { namespace lpx {

  //Glib::RefPtr<Gtk::Application> gtkApp;

  typedef std::vector<addon_t> AddonSet;

  AddonSet gAddons;

  int register_addon(const addon_t& addon)
  {
    RTXLOG(format("Enabling '%s'", addon.name));
    gAddons.push_back(addon);
    return 0;
  }

  void init() {
    //auto gtkApp = Gtk::Application::create();
    //run_file_tests();
  }

  void setup() {

  }

  void process() {

  }

}}
