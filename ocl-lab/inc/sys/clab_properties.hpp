
#ifndef CLAB_PROPERTIES_H
#define CLAB_PROPERTIES_H

#include "core/clab_types.hpp"
#include "core/clab_modules.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  class PropertyParser : public ParserModule
  {
  public:
    PropertyParser(Parser*);

    int parse();
  };

}}}

#endif
