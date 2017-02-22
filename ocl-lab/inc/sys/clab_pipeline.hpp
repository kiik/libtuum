
#ifndef CLAB_PIPELINE_H
#define CLAB_PIPELINE_H

#include "core/clab_types.hpp"
#include "core/clab_modules.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  class PipelineParser : public ParserModule
  {
  public:
    PipelineParser(Parser*);

    int parse();
  };

}}}

#endif
