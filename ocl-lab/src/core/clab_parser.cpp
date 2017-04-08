
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include "tuum_logger.hpp"

#include "core/clab_lang.hpp"
#include "core/clab_reader.hpp"
#include "core/clab_modules.hpp"
#include "core/clab_parser.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  Parser::Parser()
  {

  }

  Token Parser::matchToken(const TokenSet& set) {
    return match_token(mReader->getBuffer(), set);
  }

  Token Parser::matchToken() {
    return match_token(mReader->getBuffer());
  }


  int Parser::readUntil(Token& match, const TokenSet& filter) {
    int res;

    do {
      res = mReader->bufferChar();
      if(res < 0) break;

      match = matchToken(filter);
      if(match != Token::TK_Unknown) break;
    } while(res >= 0);

    return 1;
  }



  int Parser::parseComment() {
    switch(matchToken()) {
      case Token::TK_CommentBlock:
      {
        Token match = Token::TK_Unknown;
        const TokenSet set = {Token::TK_CommentBlockE};

        if(readUntil(match, set) < 0) return -1;
        mReader->clearBuffer();
        break;
      }
      case Token::TK_Comment:
      {
        Token match = Token::TK_Unknown;
        const TokenSet set = {Token::TK_LineFeed};

        if(readUntil(match, set) < 0) return -1;
        mReader->clearBuffer();
        break;
      }
      default:
        RTXLOG(format("Error - Unkown comment token error '%s'", mReader->getBuffer()->c_str()));
        return -1;
    }

    return 0;
  }

  int Parser::parseString(expr_t& out) {
    Token match;
    const TokenSet set = {Token::TK_String};

    mReader->clearBuffer();
    if(readUntil(match, set) < 0) return -1;

    out.type = Token::TK_String;
    out.str_val = mReader->getBuffer()->substr(0, mBuffer->size() - 1);

    return 0;
  }

  int Parser::parseTuple(expr_t* out) {
    out->str_val = "Tuple";

    return readContainer(out, {TK_TupleE});
  }

  int Parser::parseScope(expr_t* out)
  {
    if(out->type != Token::TK_Scope) return -1;

    expr_t* scope_ptr = out;
    size_t scope_seq = 1;

    expr_t* expr_ptr;

    do {
      expr_ptr = new expr_t();

      expr_ptr->setParent(scope_ptr);

      if(readExpression(expr_ptr) < 0) {
        delete(expr_ptr);
        return -2;
      }

      switch(expr_ptr->type) {
        case Token::TK_Scope:
          scope_seq++;
          break;
        case Token::TK_ScopeE:
          scope_seq--;
          break;
        default:
          if(handleExpression(expr_ptr) < 0) {
            delete(expr_ptr);
            return -3;
          }
          break;
      }

      scope_ptr->addChild(expr_ptr);

    } while(scope_seq > 0);

    return 1;
  }

  int Parser::parseNamespace(expr_t* out)
  {
    if(out->type != Token::TK_Namespace) return -1;

    expr_t* expr_ptr = new expr_t();

    expr_ptr->setParent(out);

    if(readExpression(expr_ptr) < 0) {
      delete(expr_ptr);
      return -2;
    }

    printf("#TODO: parseNamespace\n");
    //expr_ptr->debugPrint();

    //out->addChild(expr_ptr);

    /*
    switch(expr_ptr->type) {
      case Token::TK_Scope:
        scope_seq++;
        break;
      case Token::TK_ScopeE:
        scope_seq--;
        break;
      default:
        if(handleExpression(expr_ptr) < 0) {
          delete(expr_ptr);
          return -3;
        }
        break;
    }*/

      //scope_ptr->addChild(expr_ptr);

    return -100;
  }



  int Parser::readExpression(expr_t* out)
  {
    std::string buf;
    Token tok;
    Keyword kw;

    mReader->clearBuffer();

    bool run = true;
    int res = -1;
    do {
      res = mReader->bufferChar() > 0;
      if(res < 0) break;

      tok = matchToken();

      // Handle keyword/literal terminators
      switch(tok) {
        case Token::TK_LineFeed:
        case Token::TK_Terminator:

          size_t token_len = 1;

          if(mBuffer->size() <= token_len) { // No expression to terminate.
            mReader->clearBuffer();
            mBuffer->clear();
            continue;
          } else {
            *mBuffer = mBuffer->substr(0, mBuffer->size() - token_len); //TODO: - sizeof(token_str)
            kw = match_keyword(*mBuffer);

            //printf("term: '%s'[%lu]\n", mBuffer->c_str(), mBuffer->size());

            out->str_val = *mBuffer;
            out->kw = kw;
            if(kw == Keyword::KW_Unknown) {
              out->type = Token::TK_Symbol;
            } else out->type = Token::TK_Keyword;
            goto END;
          }
          break;
      }

      // Handle buffered symbol
      switch(tok) {
        case Token::TK_Unknown: // Unfinished symbol
          break;
        case Token::TK_Comment:
        case Token::TK_CommentBlock:
        case Token::TK_CommentBlockE:
          if(parseComment() < 0) return -1;
          break;
        case Token::TK_String:
          if(parseString(*out) < 0) return -1;
          else goto END;
          break;
        case Token::TK_Scope:
        case Token::TK_ScopeE:
          out->str_val = "Scope";
        case Token::TK_Tuple:
        case Token::TK_TupleE:
        case Token::TK_Namespace:
          out->type = tok;
          goto END;
        default:
          printf("Error - Unhandled token type '%i' ('%s')\n", tok, mBuffer->c_str());
          if(mBuffer->size() == 0) continue;
          return 1;
          break;
      }

      buf = "";
    } while(run);

END:

    //printf("read: ");
    //out->debugPrint();

    return 1;
  }


  int Parser::readContainer(expr_t* out, const Token TK_End) {
    expr_t* expr_ptr;

    do {
      expr_ptr = new expr_t();

      expr_ptr->setParent(out);

      if(readExpression(expr_ptr) < 0) {
        delete(expr_ptr);
        return -1;
      }

      if(expr_ptr->type != TK_End) {
        if(handleExpression(expr_ptr) < 0) {
          delete(expr_ptr);
          return -2;
        }
      }

      out->addChild(expr_ptr);

    } while(expr_ptr->type != TK_End);

    return 0;
  }

  int Parser::readScopeAsString(expr_t& out)
  {
    if(out.type != Token::TK_Scope) return -1;

    mReader->clearBuffer();

    Token tok;
    size_t lvl = 1;

    do
    {
      if(readUntil(tok, {Token::TK_Scope, Token::TK_ScopeE}) < 0) {
        return -2;
      }

      switch(tok) {
        case Token::TK_Scope:
          lvl++;
          break;
        case Token::TK_ScopeE:
          lvl--;
          break;
      }

    } while(lvl > 0);

    out.type = Token::TK_String;
    out.str_val = *mBuffer;

    return 1;
  }



  int Parser::handleKeyword(expr_t* in) {
    switch(in->kw) {
      case Keyword::KW_Pipeline:
      {
        PipelineParser p(this, in);
        return p.parse();
      }
      case Keyword::KW_Properties:
      {
        PropertyParser p(this, in);
        return p.parse();
      }
      case Keyword::KW_Kernel:
      {
        KernelParser p(this, in);
        return p.parse();
      }
      case Keyword::KW_Procedure:
      {
        ProcedureParser p(this, in);
        return p.parse();
      }
    }

    return 0;
  }

  int Parser::handleSymbol(expr_t* in)
  {
    if(in->type != Token::TK_Symbol) return -1;

    expr_t* scope = in->getScope();

    symbol_t* sym = scope->findSymbol(in->str_val);

    if(sym->type == SymbolType::ST_Unknown) {
      if(scope->parent != nullptr) {
        expr_t* root_scope = scope->getRootScope();
        sym = root_scope->findChildSymbol(in->str_val);
      }
    }

    switch(sym->type) {
      case SymbolType::ST_Class:
        {
          expr_t* xname = new expr_t(), *xargs = new expr_t();

          xname->setParent(in);
          xargs->setParent(in);

          if(read({Token::TK_Symbol}, xname) < 0) {
            delete(xname);
            delete(xargs);
            return -2;
          }

          if(scope->findSymbol(xname->str_val)->type != SymbolType::ST_Unknown) {
            printf("[Parser::handleSymbol]Error - symbol already defined '%s'\n", xname->str_val.c_str());
            delete(xname);
            delete(xargs);
            return -4;
          }

          if(read({Token::TK_Tuple}, xargs) < 0) {
            delete(xname);
            delete(xargs);
            return -3;
          }

          if(handleExpression(xargs) < 0) {
            delete(xname);
            delete(xargs);
            return -5;
          }

          in->addChild(xname);
          in->addChild(xargs);

          // new expr_t({TK_Symbol}, "Buffer", {(TK_Symbol, "Input_YUYV"), (TK_Tuple, (...))})
          // new symbol_t({ST_Object, "Input_YUYV", expr_t*, parent("Buffer")})
          symbol_t* obj = new symbol_t({ST_Object, xname->str_val});
          obj->setSource(in);
          obj->setClass(sym);

          if(scope->addSymbol(obj) < 0) return -6;

          //printf("%s %s = new %s(...);\n", in->str_val.c_str(), xname->str_val.c_str(), in->str_val.c_str());
        }
        return 1;

      case SymbolType::ST_Function:
      {
        expr_t* xargs = new expr_t();

        xargs->setParent(in);

        if(read({Token::TK_Tuple}, xargs) < 0) {
          delete(xargs);
          return -2;
        }

        if(handleExpression(xargs) < 0) {
          delete(xargs);
          return -3;
        }

        in->addChild(xargs);

        // new expr_t({TK_Symbol}, "Buffer", {(TK_Symbol, "Input_YUYV"), (TK_Tuple, (...))})
        // new symbol_t({ST_Object, "Input_YUYV", expr_t*, parent("Buffer")})
        symbol_t* obj = new symbol_t({ST_BoundFunction, in->str_val});
        obj->setSource(in);
        obj->setClass(sym);

        if(scope->addSymbol(obj) < 0) return -4;

        //printf("%s %s = new %s(...);\n", in->str_val.c_str(), xname->str_val.c_str(), in->str_val.c_str());
      }
      return 1;
      case SymbolType::ST_Unknown:
        printf("[Parser::handleSymbol]Error - Unknown symbol '%s'\n", in->str_val.c_str());
        return -2;
      default:
        break;
    }

    // Otherwise is reference
    in->type = Token::TK_Reference;
    in->setReference(sym);

    return 1;
  }

  int Parser::handleExpression(expr_t* in)
  {
    switch(in->type) {
      case Token::TK_Keyword:
        return handleKeyword(in);
      case Token::TK_Symbol:
        if(parse_numeric(in) > 0) return 1;
        return handleSymbol(in);
      case Token::TK_Tuple:
        return parseTuple(in);
      case Token::TK_Namespace:
        return parseNamespace(in);
    }

    return 0;
  }



  int Parser::read(const TokenSet& filter, expr_t* out) {
    if(readExpression(out) < 0) return -1;

    for(auto it = filter.begin(); it != filter.end(); it++) {
      if(*it == out->type) return 1;
    }

    printf("[Parser::read]Error - unexpected token %i ('%s')\n", out->type, out->str_val.c_str());

    return -2;
  }



  // High level public methods
  int Parser::load(const char* path) {
    mReader = new Reader(path);
    mBuffer = mReader->getBuffer();

    return 1;
  }

  int Parser::parse(script_ctx_t* ctx) {
    mCtx = ctx;

    expr_t* root_scope = new expr_t();
    root_scope->type = Token::TK_Scope;
    expr_t* expr = nullptr;

    while(mReader->getUnreadSize() > 0) {
      expr = new expr_t();
      expr->setParent(root_scope);
      root_scope->addChild(expr);

      if(readExpression(expr) < 0) goto ERR;
      if(handleExpression(expr) < 0) goto ERR;
    }

    //root_scope->debugTreePrint();

    RTXLOG("Script loaded.");
    delete(root_scope);

    return 0;
ERR:
    RTXLOG("Script parsing failed!");

    if(expr != nullptr)
      expr->debugTreePrint();

    delete(root_scope);
    return -1;
  }

}}}
