
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include "tuum_logger.hpp"

#include "core/clab_reader.hpp"
#include "core/clab_parser.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  const std::string TupleBegin = "(",
                    TupleEnd   = ")",

                    ArrBegin   = "[",
                    ArrEnd     = "]",

                    DQuote     = "\"",
                    NewLine    = "\n",

                    ScopeBegin = "{",
                    ScopeEnd   = "}";


  Parser::SymbolMap Parser::gSymbolMap = {
    {TupleBegin, SymbolType::ST_Operator},
    {TupleEnd,   SymbolType::ST_Operator},

    {std::string(" "), SymbolType::ST_Terminator},
    {NewLine, SymbolType::ST_LineFeed},

    {DQuote, SymbolType::ST_StringLiteral},

    {ScopeBegin, SymbolType::ST_ScopeBegin},
    {ScopeEnd,   SymbolType::ST_ScopeEnd},

    {std::string("//"), SymbolType::ST_Comment},
    {std::string("/*"), SymbolType::ST_CommentBlock},
    {std::string("*/"), SymbolType::ST_CommentBlockE}
  };


  Parser::KeywordMap Parser::gKeywordMap = {
    {std::string("Pipeline"), KeywordType::KW_Pipeline},

    {std::string("Properties"), KeywordType::KW_Properties},

    {std::string("Kernel"), KeywordType::KW_Kernel},
    {std::string("Pass"), KeywordType::KW_Pass},

    {std::string("Procedure"), KeywordType::KW_Procedure}
  };

}}}


namespace tuum {
namespace ocl {
namespace lab {

  Parser::Parser()
  {

  }

  SymbolType Parser::matchSymbol(const std::string* buf)
  {
    SymbolType out = SymbolType::ST_Unknown;

    for(auto it = Parser::gSymbolMap.begin(); it != Parser::gSymbolMap.end(); it++) {
      if(boost::algorithm::ends_with(buf->c_str(), it->first)) {
        out = it->second;
        break;
      }
    }

    return out;
  }

  SymbolType Parser::matchSymbol(const std::string* buf, const SymbolSet& set)
  {
    SymbolType out = matchSymbol(buf);

    for(auto it = set.begin(); it != set.end(); it++) {
      if(out == *it)
        return out;
    }

    return SymbolType::ST_Unknown;
  }

  SymbolType Parser::matchSymbol(const SymbolSet& set) {
    return matchSymbol(mReader->getBuffer(), set);
  }

  SymbolType Parser::matchSymbol() {
    return matchSymbol(mReader->getBuffer());
  }



  /** Symbol iteration methods **/

  int Parser::readUntil(SymbolType& match, const SymbolSet& set) {
    while(mReader->bufferChar() > 0) {
      match = matchSymbol(set);
      if(match != SymbolType::ST_Unknown) break;
    }

    return 0;
  }

  /*
  int Parser::readUntil(const SymbolSet_t& set, Symbol_t match) {
    while(mReader->bufferChar() > 0) {
      match = bufferSymbolMatch(set);
      if(match != "") break;
    }

    return 0;
  }*/

  int Parser::readSymbol(SymbolType &out) {
    SymbolType type;
    std::string buf = "";

    mReader->clearBuffer();

    bool run = true;

    RTXLOG("Do");
    while((run == true) && (mReader->bufferChar() > 0)) {
      type = matchSymbol();

      // Handle keyword/literal terminators
      switch(type) {
        case SymbolType::ST_LineFeed:
        case SymbolType::ST_Terminator:
          if(mBuffer->size() == buf.size()) {
            mReader->clearBuffer();
            continue;
          }
          else {
            *mBuffer = mBuffer->substr(0, mBuffer->size() - buf.size());
            type = SymbolType::ST_Keyword;
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
          parseComment();
          break;
        case SymbolType::ST_StringLiteral:
          parseString();
          break;
        case SymbolType::ST_Operator:
          parseOperator();
          break;
        default:
          //printf("tk: %s, type=%i, tok='%s' \n", mBuffer->c_str(), type, buf.c_str());
          printf("<token %s>\n", mBuffer->c_str());

          out = type; //keywordMatch(*mBuffer);
          run = false;
          break;
      }

      buf = "";
    }

    return 0;
  }

  int Parser::readSymbol(SymbolType& out, const SymbolSet& set) {

    return 0;
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

        if(readSymbol(match, set) < 0) return -1;
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

    printf("<string %s>\n", mBuffer->c_str());
    mReader->clearBuffer();

    return 0;
  }

  int Parser::parseOperator() {
    std::string buf;

    /*
    Symbol_t match = "";
    */

    SymbolType match;
    const SymbolSet set = {SymbolType::ST_TupleEnd};

    if(readUntil(match, set) < 0) return -1;

    printf("<operator %s>\n", mBuffer->c_str());
    mReader->clearBuffer();

    return 0;
  }

  int Parser::handleSymbol(const SymbolType& symb, expr_t& out) {

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



  // High level public methods
  int Parser::load(const char* path) {
    mReader = new Reader(path);
    return 1;
  }

  int Parser::parse() {

    SymbolType kw;
    expr_t expr;

    while(mReader->getUnreadSize() > 0) {
      if(readSymbol(kw) < 0) goto ERR;
      if(handleSymbol(kw, expr) < 0) goto ERR;
    }

FIN:
    RTXLOG("Script loaded.");
    return 0;
ERR:
    RTXLOG("Script parsing failed!");
    return -1;
  }

  int Parser::make(ocl::Pipeline*& out) {
    out = nullptr;
    return -1;
  }


}}}
