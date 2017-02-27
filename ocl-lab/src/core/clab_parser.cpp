
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

  SymbolType Parser::matchSymbol(const SymbolSet& set) {
    return match_symbol(mReader->getBuffer(), set);
  }

  SymbolType Parser::matchSymbol() {
    return match_symbol(mReader->getBuffer());
  }


  int Parser::readUntil(SymbolType& match, const SymbolSet& set) {
    char c[] = {'\0', '\0'};

    int res = -1;
    do {
      res = mReader->bufferChar(c[0]);
      if(res < 0) break;

      match = matchSymbol(set);
      if(match != SymbolType::ST_Unknown) break;
    } while(res >= 0);

    return res;
  }

  int Parser::readSymbol(SymbolType &type) {
    std::string buf;

    mReader->clearBuffer();

    bool run = true;
    int res = -1;
    do {
      res = mReader->bufferChar() > 0;
      if(res < 0) break;

      type = SymbolType::ST_Unknown;
      type = matchSymbol();

      // Handle keyword/literal terminators
      switch(type) {
        case SymbolType::ST_LineFeed:
        case SymbolType::ST_Terminator:
          if(mBuffer->size() < 1) { // No expression to terminate.
            mReader->clearBuffer();
            continue;
          } else {
            *mBuffer = mBuffer->substr(0, mBuffer->size() - 1);
            type = match_keyword(*mBuffer);
          }
          break;
      }

      // Handle buffered symbol
      switch(type) {
        case SymbolType::ST_Unknown: // Unfinished symbol
          break;
        case SymbolType::ST_Comment:
        case SymbolType::ST_CommentBlock:
        case SymbolType::ST_CommentBlockE:
          if(parseComment() < 0) return -1;
          break;
        case SymbolType::ST_StringLiteral:
          if(parseString() < 0) return -1;
          else return 1;
          break;
        case SymbolType::ST_Operator:
          if(parseOperator() < 0) return -1;
          else return 1;
          break;
        default:
          if(mBuffer->size() == 0) continue;
          return 1;
          break;
      }

      buf = "";
    } while(run);

    return res;
  }

  int Parser::readSymbol(SymbolType& out, const SymbolSet& set) {

    return 0;
  }

  int Parser::handleKeyword(const KeywordType& in) {
    switch(in) {
      case KeywordType::KW_Properties:
      {
        PropertyParser p(this);
        return p.parse();
      }
      case KeywordType::KW_Kernel:
      {
        KernelParser p(this);
        return p.parse();
      }
      case KeywordType::KW_Procedure:
      {
        ProcedureParser p(this);
        return p.parse();
      }
    }

    return 0;
  }

  /** :struct expr_t: {name: Kernel, type: ST_Keyword, kval: KW_Kernel}
   *
   */
  int Parser::readExpression(expr_t& out)
  {
    SymbolType type;

    int res;

    do {
      type = SymbolType::ST_Unknown;
      res = readSymbol(type);

      if(type >= 0) {
        out.data = *mBuffer;
        out.type = type;

        std::string dbg_name;
        switch(type) {
          case SymbolType::ST_Operator:
            dbg_name = "operator";
            break;
          case SymbolType::ST_Keyword:
            match_keyword(out.data, out.kwt);
            dbg_name = "keyword";
            break;
          case SymbolType::ST_Symbol:
            dbg_name = "symbol";
            break;
          case SymbolType::ST_StringLiteral:
            dbg_name = "string";
            break;
          case SymbolType::ST_ScopeBegin:
          case SymbolType::ST_ScopeEnd:
            dbg_name = "scope";
            break;
          default:
            dbg_name = "token";
            break;
        }

        printf("<%s.%i '%s'[%lu]>\n", dbg_name.c_str(), type, out.data.c_str(), out.data.size());

        return 1;
      }

      if(res < 0) return -1;

    } while(res >= 0);

    return 0;
  }

  int Parser::handleExpression(const expr_t& in)
  {
    switch(in.type) {
      case SymbolType::ST_Keyword:
        handleKeyword(in.kwt);
        break;
    }
  }



  /** Integrated parsing modules **/

  int Parser::parseScope() {
    size_t lvl = 0;

    SymbolSet set = {SymbolType::ST_ScopeBegin, SymbolType::ST_ScopeEnd};
    SymbolType match;

    while(mReader->bufferChar() > 0) {
      match = matchSymbol(set);

      switch(match) {
        case SymbolType::ST_ScopeBegin:
          lvl++;
          break;
        case SymbolType::ST_ScopeEnd:
          lvl--;
          break;
      }
    }

    printf("<scope(%lu)>\n", lvl);
    mReader->clearBuffer();
  }

  int Parser::parseComment() {
    switch(matchSymbol()) {
      case SymbolType::ST_CommentBlock:
      {
        SymbolType match = SymbolType::ST_Unknown;
        const SymbolSet set = {ST_CommentBlockE};

        if(readUntil(match, set) < 0) return -1;
        mReader->clearBuffer();
        break;
      }
      case SymbolType::ST_Comment:
      {
        SymbolType match = SymbolType::ST_Unknown;
        const SymbolSet set = {SymbolType::ST_LineFeed};

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

  int Parser::parseString() {
    std::string buf;

    SymbolType match = SymbolType::ST_Unknown;
    const SymbolSet set = {ST_StringLiteral};

    if(readUntil(match, set) < 0) return -1;

    return 0;
  }

  int Parser::parseOperator() {
    std::string buf;

    //printf("parse operator from '%s'\n", mBuffer->c_str());

    SymbolType match;
    const SymbolSet set = {SymbolType::ST_Operator};

    size_t line = mReader->getLineNumber();

    while(mReader->getUnreadSize() > 0) {
      if(readUntil(match, set) < 0) {
        RTXLOG(format("Error - failed to parse operator on line %lu!", line));
        return -1;
      }

      match = match_operator(*mBuffer);

      if(match == SymbolType::ST_TupleEnd) return 1;
    }


    return 0;
  }

  int Parser::handleScopeLiteral(const char c)
  {
    switch(c) {
      case '{':
        scope_seq++;
        break;
      case '}':
        if(scope_seq <= 1) {
          scope_seq = 0;
          return 1;
        }

        scope_seq--;
        break;
    }

    return 0;
  }


  int Parser::readScopeAsString(expr_t& out)
  {
    out.type = SymbolType::ST_Unknown;
    SymbolType type;

    if(readSymbol(type) < 0) return -1;

    if(type != SymbolType::ST_ScopeBegin) {
      RTXLOG("Error - Expected scope!");
      return -2;
    }

    size_t lvl = 1;
    mReader->clearBuffer();
    while(readUntil(type, {SymbolType::ST_ScopeBegin, SymbolType::ST_ScopeEnd}) >= 0) {
      switch(type) {
        case SymbolType::ST_ScopeBegin:
          lvl++;
          break;
        case SymbolType::ST_ScopeEnd:
          if(lvl == 1) {
            out.type = SymbolType::ST_StringLiteral;
            out.data = *mBuffer;
            return 1;
          } else {
            lvl--;
          }
          break;
      }
    }
  }


  // High level public methods
  int Parser::load(const char* path) {
    mReader = new Reader(path);
    mBuffer = mReader->getBuffer();

    return 1;
  }

  int Parser::parse(script_ctx_t* ctx) {
    mCtx = ctx;

    SymbolType kw;
    expr_t expr;

    while(mReader->getUnreadSize() > 0) {
      if(readExpression(expr) < 0) goto ERR;
      if(handleExpression(expr) < 0) goto ERR;
    }

FIN:
    RTXLOG("Script loaded.");
    return 0;
ERR:
    RTXLOG("Script parsing failed!");
    return -1;
  }

}}}
