
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

  int PassParser::parseRange() {
    SymbolType type;

    if(scopeStep(type) < ScopeSignal::Continue) return -1;

    switch(type) {
      case SymbolType::ST_Operator:
        printf("#TODO: Range = %s\n", getBuffer().c_str());
        break;
      default:
        RTXLOG("");
        errUnexpSymbol(type);
        return -2;
    }

    return 0;
  }

  int PassParser::parseCall() {
    SymbolType type;

    if(scopeStep(type) < ScopeSignal::Continue) return -1;

    switch(type) {
      case SymbolType::ST_Operator:
        printf("#TODO: Task = %s\n", getBuffer().c_str());
        break;
      default:
        RTXLOG("");
        errUnexpSymbol(type);
        return -2;
    }

    return 0;
  }

  int PassParser::parseProcedure()
  {
    SymbolType type;

    if(scopeEnter() < 0) return -1;

    while(scopeStep(type) >= ScopeSignal::Continue) {
      switch(type) {
        case SymbolType::ST_Symbol:
          if(matchCustomSymbol("Range") > 0) {
            if(parseRange() < 0) return -2;
            break;
          } else if(matchCustomSymbol("Call") > 0) {
            if(parseCall() < 0) return -3;
            break;
          }
        default:
          errUnexpSymbol(type);
          return -5;
      }
    };

    return -100;
  }

  int PassParser::parse()
  {
    SymbolType type;

    if(scopeEnter() < 0) return -1;

    while(scopeStep(type) >= ScopeSignal::Continue) {
      switch(type) {
        case SymbolType::ST_Symbol:
        case SymbolType::ST_Keyword:
          if(matchCustomSymbol("Procedure") > 0) {
            if(parseProcedure() < 0) return -4;;
            break;
          }
        default:
          errUnexpSymbol(type);
          return -5;
      }
    };

    return -1;
  }

}}}
