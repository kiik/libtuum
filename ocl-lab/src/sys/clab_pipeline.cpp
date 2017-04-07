
#include "sys/clab_pipeline.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  PipelineParser::PipelineParser(Parser* p):
    ParserModule(p)
  {

  }

  int PipelineParser::parse()
  {
    printf("#TODO: PipelineParser::parse\n");

    std::string name;
    expr_t expr;

    if(read({Token::TK_String}, expr) < 0) return -1;

    name = expr.str_val;

    if(read({Token::TK_Scope}, expr) < 0) return -2;
    if(parseScope(&expr) < 0) return -3;

    printf("#TODO: PipelineParser::parse - Build pipeline");

    return -100;
  }

}}}
