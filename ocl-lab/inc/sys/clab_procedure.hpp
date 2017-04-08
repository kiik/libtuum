
#ifndef CLAB_PASS_H
#define CLAB_PASS_H

#include "core/clab_types.hpp"
#include "core/clab_modules.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  class ProcedureParser : public ParserModule
  {
  public:
    ProcedureParser(Parser*, expr_t*);

    int parseRange();
    int parseCall();

    int parsePass(expr_t*);

    int parse();
  };

}}}

#endif
