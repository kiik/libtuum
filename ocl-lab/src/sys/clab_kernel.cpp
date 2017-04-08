
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

    expr = new expr_t();

    expr->setParent(mExpr);
    if(enterScope(expr) < 0) return -2;
    if(gParser->readScopeAsString(*expr) < 0) return -3;
    mExpr->addChild(expr);

    return 1;
  }

}}}
