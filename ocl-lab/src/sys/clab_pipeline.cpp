
#include "sys/clab_pipeline.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  PipelineParser::PipelineParser(Parser* p, expr_t* expr):
    ParserModule(p, expr)
  {

  }

  int PipelineParser::parse()
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
    if(enterScope(expr) < 0) {
      delete(expr);
      return -2;
    }
    mExpr->addChild(expr);

    if(parseScope(expr) < 0) return -3;

    return 1;
  }

}}}
