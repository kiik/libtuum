
#include "tuum_logger.hpp"

#include "core/clab_lang.hpp"
#include "core/clab_parser.hpp"

#include "sys/clab_procedure.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  ProcedureParser::ProcedureParser(Parser* p, expr_t* expr):
    ParserModule(p, expr)
  {

  }

  int ProcedureParser::parseRange() {
    /*
    Token type;

    if(scopeStep(type) < ScopeSignal::Continue) return -1;

    expr_t expr;

    switch(type) {
      case Token::TK_Operator:
      {
        const std::string data = getBuffer();

        printf("#TODO: Range = %s\n", getBuffer().c_str());

        if(parse_expression(data.c_str(), expr) < 0) return -2;

        printf("#TODO: set Range%s\n", data.c_str());

        break;
      }
      default:
        RTXLOG("");
        errUnexpSymbol(type);
        return -2;
    }

    //getContext()->add(expr);
    */

    return 0;
  }

  int ProcedureParser::parseCall() {
    Token type;

    /*
    if(scopeStep(type) < ScopeSignal::Continue) return -1;

    switch(type) {
      case Token::TK_Operator:
      {
        const std::string data = getBuffer();
        printf("#TODO: Task = %s\n", data.c_str());

        expr_t expr;
        //if(parse_expression(data.c_str(), expr) < 0) return -2;

        //printf("#TODO: set Task%s\n", data.c_str());
        break;
      }
      default:
        RTXLOG("");
        errUnexpSymbol(type);
        return -2;
    }*/

    return 0;
  }

  int ProcedureParser::parseProcedure()
  {
    /*
    Token type;

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
    */
    return -100;
  }

  int ProcedureParser::parse()
  {
    /*
    Token type;

    if(scopeEnter() < 0) return -1;

    while(scopeStep(type) >= ScopeSignal::Continue) {
      switch(type) {
        case SymbolType::ST_Symbol:
        case SymbolType::ST_Keyword:
          if(matchCustomSymbol("Pass") > 0) {
            if(parseProcedure() < 0) return -4;;
            break;
          }
        default:
          errUnexpSymbol(type);
          return -5;
      }
    };
    */
    return -1;
  }

}}}
