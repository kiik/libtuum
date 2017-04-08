
#include "tuum_logger.hpp"

#include "core/clab_parser.hpp"
#include "sys/clab_kernel.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  KernelParser::KernelParser(Parser* p, expr_t* expr):
    ParserModule(p, expr)
  {

  }

  int KernelParser::parse()
  {

    expr_t* expr = new expr_t();

    expr->setParent(mExpr);
    if(read({Token::TK_String}, expr) < 0) {
      delete(expr);
      return -1;
    }
    mExpr->addChild(expr);
    std::string name = expr->str_val;

    expr = new expr_t();

    expr->setParent(mExpr);
    if(enterScope(expr) < 0) return -2;
    if(gParser->readScopeAsString(*expr) < 0) return -3;
    mExpr->addChild(expr);

    expr_t* scope = mExpr->getScope();
    if(scope == nullptr) return -4;

    if(scope->findSymbol(name)->type != SymbolType::ST_Unknown) {
      printf("[KernelParser::parse]Error - symbol already defined '%s'\n", name.c_str());
      return -5;
    }

    symbol_t* obj = new symbol_t({ST_Kernel, name});
    obj->setSource(mExpr);
    scope->addSymbol(obj);

    return 1;
  }

}}}
