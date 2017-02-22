
#include "tuum_logger.hpp"

#include "core/clab_parser.hpp"
#include "sys/clab_kernel.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  KernelParser::KernelParser(Parser* p):
    ParserModule(p)
  {

  }

  int KernelParser::parse()
  {
    std::string name;

    expr_t expr;

    if(gParser->readExpression(expr) < 0) return -1;

    if(expr.type != SymbolType::ST_StringLiteral) {
      RTXLOG(format("Error - Expected string, got '%s'", expr.data.c_str()));
      return -2;
    }

    name = expr.data;

    if(gParser->readScopeAsString(expr) < 0) return -3;

    if(expr.type != SymbolType::ST_StringLiteral) {
      RTXLOG(format("Error - Expected string, got '%s'", expr.data.c_str()));
      return -4;
    }

    RTXLOG(format("new Kernel(%s, \"%s\");", name.c_str(), expr.data.c_str()));

    kernel_t kernel(name, expr.data); // name, body
    gParser->getScriptContext()->add(kernel);

    return 0;
  }

}}}
