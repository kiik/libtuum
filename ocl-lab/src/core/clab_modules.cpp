
#include "tuum_logger.hpp"

#include "core/clab_types.hpp"
#include "core/clab_parser.hpp"

#include "core/clab_modules.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  int ParserModule::enterScope() {
    SymbolType type;
    int res = gParser->readSymbol(type);
    if(res < 0) return -1;

    if(type != SymbolType::ST_ScopeBegin) {
      RTXLOG(format("Error - expected scope, got <symbol %i>", type));
      return -2;
    }

    return 1;
  }

}}}
