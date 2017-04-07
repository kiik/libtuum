
#include "tuum_logger.hpp"

#include "core/clab_lang.hpp"
#include "core/clab_parser.hpp"

#include "sys/clab_properties.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  symbol_t sym_Unknown = {SymbolType::ST_Unknown, "None"};
  symbol_t cls_Buffer = {SymbolType::ST_Class, "Buffer"};

  SymbolTable gSymbolTable = {
    {"0", &sym_Unknown},
    {"Buffer", &cls_Buffer},
  };

  PropertyParser::PropertyParser(Parser* p):
    ParserModule(p)
  {

  }

  int PropertyParser::parseProperty(const std::string& varName)
  {
    /*
    SymbolType type;
    gParser->readSymbol(type);

    if(type != SymbolType::ST_Operator) {
      RTXLOG(format("Error - expected tuple, got '%s' (%i)", gParser->getBuffer().c_str(), type));
      return -1;
    }

    std::string callData = gParser->getBuffer();

    expr_t expr;
    if(parse_expression(callData.c_str(), expr) < 0) return -2;

    printf("#TODO: new %s%s\n", varName.c_str(), callData.c_str());
    */

    return 0;
  }

  int PropertyParser::parse()
  {
    /*
    SymbolType type;

    if(scopeEnter() < 0) return -1;

    while(true) {
      ScopeSignal s = scopeStep(type);
      switch(s) {
        case ScopeSignal::OutOfScope:
          return 1;
        case ScopeSignal::Exit:
          continue;
      }

      switch(type) {
        case SymbolType::ST_Symbol:
          if(parseProperty(gParser->getBuffer()) < 0) return -4;
          break;
        case SymbolType::ST_Keyword:
        case SymbolType::ST_Operator:
        default:
          RTXLOG(format("Error - unexpected symbol '%s'(%i)", gParser->getBuffer().c_str(), type));
          return -5;
      }

    }
    */


    // <Symbol> <SymbolName> <Tuple>

    expr_t* scope = new expr_t();
    if(enterScope(scope) < 0) return -1;

    scope->setSymbolTable(gSymbolTable);

    if(parseScope(scope) < 0) return -2;

    scope->debugTreePrint();
    delete(scope);

    printf("#TODO: PropertyParser::parse\n");

    return -1;
  }

}}}
