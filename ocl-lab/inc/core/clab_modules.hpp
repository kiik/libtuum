
#ifndef CLAB_MODULES_H
#define CLAB_MODULES_H

#include "core/clab_types.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  class ParserModule {
  public:
    ParserModule(Parser* p):
      gParser(p)
    {

    }

    int enterScope();

    virtual int parse() = 0;

  protected:
    Parser* gParser;
  };

}}}

#include "sys/clab_properties.hpp"
#include "sys/clab_kernel.hpp"
#include "sys/clab_pass.hpp"
#include "sys/clab_pipeline.hpp"

#endif
