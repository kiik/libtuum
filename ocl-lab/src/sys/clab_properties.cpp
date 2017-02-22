
#include "tuum_logger.hpp"

#include "core/clab_parser.hpp"

#include "sys/clab_properties.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  PropertyParser::PropertyParser(Parser* p):
    ParserModule(p)
  {

  }

  int PropertyParser::parseProperty(const std::string& varName)
  {
    SymbolType type;
    gParser->readSymbol(type);

    if(type != SymbolType::ST_Operator) {
      RTXLOG(format("Error - expected tuple, got '%s' (%i)", gParser->getBuffer().c_str(), type));
      return -1;
    }

    std::string callData = gParser->getBuffer();
    printf("#TODO: new %s%s\n", varName.c_str(), callData.c_str());

    return 0;
  }

  int PropertyParser::parse()
  {
    SymbolType type;

    int res = gParser->readSymbol(type);
    if(res < 0) return -1;

    if(type != SymbolType::ST_ScopeBegin) {
      RTXLOG(format("Error - expected scope, got <symbol %i>", type));
      return -2;
    }

    size_t lvl = 1;
    do {
      res = gParser->readSymbol(type);
      if(res < 0) return -3;

      switch(type) {
        case SymbolType::ST_ScopeEnd:
          lvl--;
          break;
        case SymbolType::ST_Symbol:
          if(parseProperty(gParser->getBuffer()) < 0) return -4;
          break;
        case SymbolType::ST_Keyword:
        case SymbolType::ST_Operator:
        default:
          RTXLOG(format("Error - unexpected symbol '%s'(%i)", gParser->getBuffer().c_str(), type));
          return -5;
      }

    } while(lvl > 0);

    return 0;
  }

}}}
