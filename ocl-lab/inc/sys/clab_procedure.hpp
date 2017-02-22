
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
    ProcedureParser(Parser*);

    int parseRange();
    int parseCall();
    int parseProcedure();

    int parse();
  };

}}}

#endif
