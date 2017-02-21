
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

  SymbolType Parser::tokenMatch(const std::string& value, const SymbolSet& set)
  {
    auto token_it = Parser::gSymbolMap.end();

    std::vector<std::pair<std::string, SymbolType>> map_buf;

    for(auto tok_it = Parser::gSymbolMap.begin(); tok_it != Parser::gSymbolMap.end(); tok_it++) {
      for(auto it = set.begin(); it != set.end(); it++) {
        if(*it == tok_it->second) map_buf.push_back(*tok_it);
      }
    }

    for(size_t ix = 0; ix < map_buf.size(); ix++) {
      auto tok = map_buf[ix];

      if(boost::algorithm::ends_with(value.c_str(), tok.first)) {
        //RTXLOG(format("matched (%s, %i)", tok.first.c_str(), tok.second));
        return tok.second;
      }
    }

    return SymbolType::ST_Unknown;
  }

  SymbolType Parser::tokenMatch(const std::string& value) {

    for(auto it = Parser::gSymbolMap.begin(); it != Parser::gSymbolMap.end(); it++) {
      if(boost::algorithm::ends_with(value.c_str(), it->first)) {
        //RTXLOG(format("matched '%s' => (%s, %i)", value.c_str(), it->first.c_str(), it->second));
        return it->second;
      }
    }

    return SymbolType::ST_Unknown;
  }

  SymbolType Parser::bufferMatch(std::string& out) {
    for(auto it = Parser::gSymbolMap.begin(); it != Parser::gSymbolMap.end(); it++) {
      if(boost::algorithm::ends_with(mBuffer->c_str(), it->first)) {
        //RTXLOG(format("matched '%s' => (%s, %i)", mBuffer->c_str(), it->first.c_str(), it->second));
        out = it->first;
        return it->second;
      }
    }

    return SymbolType::ST_Unknown;
  }

  SymbolType Parser::bufferMatch(SymbolSet set) {
    return tokenMatch(*mBuffer, set);
  }

  SymbolType Parser::bufferMatch() {
    std::string buf;
    return bufferMatch(buf);
  }


  Parser::Symbol_t Parser::bufferSymbolMatch(const SymbolSet_t& set) {
    for(auto it = set.begin(); it != set.end(); it++) {
      if(boost::algorithm::ends_with(*mBuffer->c_str(), *it)) {
        return *it;
      }
    }

    return "";
  }

  KeywordType Parser::keywordMatch(const std::string& in) {
    auto it = Parser::gKeywordMap.find(in);

    if(it != Parser::gKeywordMap.end())
      return it->second;

    return KeywordType::KW_Unknown;
  }


  int Parser::readScope() {
    size_t lvl = 0;

    SymbolSet set = {SymbolType::ST_ScopeBegin, SymbolType::ST_ScopeEnd};
    SymbolType match;

    while(mReader->bufferChar() > 0) {
      match = bufferMatch(set);

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
    std::string buf;

    switch(bufferMatch(buf)) {
      case SymbolType::ST_CommentBlock:
      {
        SymbolType match = SymbolType::ST_Unknown;
        const SymbolSet set = {ST_CommentBlockE};

        if(readSymbol(set, match) < 0) return -1;
        mReader->clearBuffer();
        break;
      }
      case SymbolType::ST_Comment:
      {
        SymbolType match = SymbolType::ST_Unknown;
        const SymbolSet set = {SymbolType::ST_LineFeed};

        if(readUntil(set, match) < 0) return -1;
        mReader->clearBuffer();
        break;
      }
      default:
        RTXLOG(format("Error - Unkown comment token error '%s'", buf.c_str()));
        return -1;
    }

    return 0;
  }

  int Parser::parseString() {
    std::string buf;

    SymbolType match = SymbolType::ST_Unknown;
    const SymbolSet set = {ST_StringLiteral};

    if(readUntil(set, match) < 0) return -1;

    printf("<string %s>\n", mBuffer->c_str());
    mReader->clearBuffer();

    return 0;
  }

  int Parser::parseOperator() {
    std::string buf;

    Symbol_t match = "";
    const SymbolSet_t set = {TupleEnd};

    if(readUntil(set, match) < 0) return -1;

    printf("<operator %s>\n", mBuffer->c_str());
    mReader->clearBuffer();

    return 0;
  }



  int Parser::readUntil(const SymbolSet& set, SymbolType& match) {
    while(mReader->bufferChar() > 0) {
      match = bufferMatch(set);
      if(match != SymbolType::ST_Unknown) break;
    }

    return 0;
  }

  int Parser::readUntil(const SymbolSet_t& set, Symbol_t match) {
    while(mReader->bufferChar() > 0) {
      match = bufferSymbolMatch(set);
      if(match != "") break;
    }

    return 0;
  }



  int Parser::readSymbol(SymbolType &out) {
    SymbolType type;
    std::string buf = "";

    mReader->clearBuffer();

    bool run = true;

    while((run == true) && (mReader->bufferChar() > 0)) {
      type = bufferMatch(buf);

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

  int Parser::handleSymbol(const SymbolType& kw) {

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
  }

  int Parser::parse() {

    KeywordType kw;
    while(mReader->getUnreadSize() > 0) {
      if(readSymbol(kw) < 0) goto ERR;
      if(handleSymbol(kw) < 0) goto ERR;
    }

FIN:
    RTXLOG("Script loaded.");
    return 0;
ERR:
    RTXLOG("Script load failed!");
    return -1;
  }

  int Parser::make(ocl::Pipeline*& out) {
    out = nullptr;
    return -1;
  }


}}}
