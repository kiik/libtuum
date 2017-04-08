
#include "tuum_logger.hpp"

#include "core/clab_lang.hpp"
#include "core/clab_parser.hpp"

#include "sys/clab_procedure.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  symbol_t fn_Range = {SymbolType::ST_Function, "Range"};
  symbol_t fn_Call = {SymbolType::ST_Function, "Call"};

  SymbolTable gPassSymbolTable = {
    {"Range", &fn_Range},
    {"Call", &fn_Call}
  };

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

  int ProcedureParser::parsePass(expr_t* out)
  {
    expr_t* scope = new expr_t();

    scope->setParent(out);
    if(enterScope(scope) < 0) {
      delete(scope);
      return -1;
    }
    out->addChild(scope);

    scope->setSymbolTable(gPassSymbolTable);

    if(parseScope(scope) < 0) return -2;

    return 1;
  }

  int ProcedureParser::parse()
  {
    expr_t* scope = new expr_t();

    scope->setParent(mExpr);
    if(enterScope(scope) < 0) {
      delete(scope);
      return -1;
    }
    mExpr->addChild(scope);

    expr_t* expr;
    do {
      expr = new expr_t();
      expr->setParent(mExpr);

      if(read(expr) < 0) {
        delete(expr);
        return -2;
      }

      switch(expr->type) {
        case Token::TK_Symbol:
          if(expr->str_val != "Pass") {
            printf("[ProcedureParser::parse]Error - Expected 'Pass' symbol, got '%s'\n", expr->str_val.c_str());
            delete(expr);
            return -3;
          } else {
            if(parsePass(expr) < 0) {
              delete(expr);
              return -4;
            }
          }
          break;
        case Token::TK_ScopeE: break;
        default:
          printf("[ProcedureParser::parse]Error - Unexpected token: ");
          expr->debugPrint();
          delete(expr);
          return -5;
      }

      mExpr->addChild(expr);

    } while(expr->type != Token::TK_ScopeE);

    return 1;
  }

}}}
