
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

    virtual int parse() = 0;

  protected:
    Parser* gParser;
  };

  class PropertyParser : public ParserModule
  {
  public:
    PropertyParser(Parser* p):
      ParserModule(p)
    {

    }

    int parse();
  };


}}}

#include "sys/clab_pipeline.hpp"
#include "sys/clab_kernel.hpp"

#endif
