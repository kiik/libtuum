
#ifndef CLAB_PASS_H
#define CLAB_PASS_H

#include "core/clab_types.hpp"
#include "core/clab_modules.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  class PassParser : public ParserModule
  {
  public:
    PassParser(Parser*);

    int parseRange();
    int parseCall();
    int parseProcedure();

    int parse();
  };

}}}

#endif
