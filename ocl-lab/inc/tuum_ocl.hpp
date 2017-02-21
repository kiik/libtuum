
#ifndef TUUM_OCL_H
#define TUUM_OCL_H

namespace tuum {

  int ocl_init();

  void ocl_setup();
  void ocl_process();

}

namespace tuum {
namespace ocl {

  class Pipeline;

  int loadScript(const char*, ocl::Pipeline*&);

}}

#endif
