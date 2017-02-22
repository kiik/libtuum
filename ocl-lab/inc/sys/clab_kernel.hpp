
#ifndef CLAB_KERNEL_H
#define CLAB_KERNEL_H

#include "core/clab_types.hpp"
#include "core/clab_modules.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  class KernelParser : public ParserModule
  {
  public:
    KernelParser(Parser*);

    int parse();
  };

}}}

#endif
