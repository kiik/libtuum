
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

  PropertyParser::PropertyParser(Parser* p, expr_t* expr):
    ParserModule(p, expr)
  {

  }

  int PropertyParser::parse()
  {
    expr_t* scope = new expr_t();
    scope->setParent(mExpr);

    if(enterScope(scope) < 0) {
      delete(scope);
      return -1;
    }
    mExpr->addChild(scope);

    scope->setSymbolTable(gSymbolTable);

    if(parseScope(scope) < 0) return -2;

    return 1;
  }

}}}
