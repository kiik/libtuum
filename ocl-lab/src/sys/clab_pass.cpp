
#include "tuum_logger.hpp"

#include "core/clab_parser.hpp"

#include "sys/clab_pass.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  PassParser::PassParser(Parser* p):
    ParserModule(p)
  {

  }

  int PassParser::parseProcedure()
  {
    RTXLOG("TODO");

    if(scopeEnter() < 0) return -1;


    return -100;
  }

  int PassParser::parse()
  {
    SymbolType type;

    if(scopeEnter() < 0) return -1;

    int res;
    size_t lvl = 1;
    while(scopeStep(type) >= ScopeSignal::Continue) {

      switch(type) {
        case SymbolType::ST_Symbol:
        case SymbolType::ST_Keyword:
          if(matchCustomSymbol("Procedure")) {
            if(parseProcedure() < 0) return -4;;
            break;
          }
          //if(parseProperty(gParser->getBuffer()) < 0) return -4;
        case SymbolType::ST_Operator:
        default:
          RTXLOG(format("Error - unexpected symbol '%s'(%i)", gParser->getBuffer().c_str(), type));
          return -5;
      }

    };

    return -1;
  }

}}}
