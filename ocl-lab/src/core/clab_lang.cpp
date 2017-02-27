
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include "core/clab_lang.hpp"

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


  SymbolMap gSymbolMap = {
    {TupleBegin, SymbolType::ST_Operator},
    {TupleEnd,   SymbolType::ST_Operator},

    {std::string(" "), SymbolType::ST_Terminator},
    {std::string(";"), SymbolType::ST_Terminator},
    {NewLine, SymbolType::ST_LineFeed},

    {DQuote, SymbolType::ST_StringLiteral},

    {ScopeBegin, SymbolType::ST_ScopeBegin},
    {ScopeEnd,   SymbolType::ST_ScopeEnd},

    {std::string("//"), SymbolType::ST_Comment},
    {std::string("/*"), SymbolType::ST_CommentBlock},
    {std::string("*/"), SymbolType::ST_CommentBlockE}
  };

  SymbolMap gOperatorMap = {
    {TupleBegin, SymbolType::ST_TupleBegin},
    {TupleEnd,   SymbolType::ST_TupleEnd},

    {ScopeBegin, SymbolType::ST_ScopeBegin},
    {ScopeEnd,   SymbolType::ST_ScopeEnd},
  };

  KeywordMap gKeywordMap = {
    {std::string("Pipeline"), KeywordType::KW_Pipeline},

    {std::string("Properties"), KeywordType::KW_Properties},

    {std::string("Kernel"), KeywordType::KW_Kernel},
    {std::string("Procedure"), KeywordType::KW_Procedure},

    {std::string("Procedure"), KeywordType::KW_Procedure}
  };


  SymbolType match_symbol(const std::string* buf)
  {
    SymbolType out = SymbolType::ST_Unknown;

    for(auto it = gSymbolMap.begin(); it != gSymbolMap.end(); it++) {
      if(boost::algorithm::ends_with(buf->c_str(), it->first)) {
        out = it->second;
        break;
      }
    }

    return out;
  }

  SymbolType match_symbol(const std::string* buf, const SymbolSet& set)
  {
    SymbolType out = match_symbol(buf);

    for(auto it = set.begin(); it != set.end(); it++) {
      if(out == *it)
        return out;
    }

    return SymbolType::ST_Unknown;
  }

  SymbolType match_operator(const std::string& buf) {
    SymbolType out = SymbolType::ST_Unknown;

    for(auto it = gOperatorMap.begin(); it != gOperatorMap.end(); it++) {
      if(boost::algorithm::ends_with(buf.c_str(), it->first)) {
        out = it->second;
        break;
      }
    }

    return out;
  }

  SymbolType match_keyword(const std::string& buf, KeywordType& kw_out) {
    SymbolType out = SymbolType::ST_Symbol;

    for(auto it = gKeywordMap.begin(); it != gKeywordMap.end(); it++) {
      if(boost::algorithm::ends_with(buf.c_str(), it->first)) {
        out = SymbolType::ST_Keyword;
        kw_out = it->second;
        break;
      }
    }

    return out;
  }

  SymbolType match_keyword(const std::string& buf) {
    KeywordType kwt;
    return match_keyword(buf, kwt);
  }

  int parse_tuple(const char* dptr, expr_t& expr) {

  }

  int parse_expression(const std::string& data, expr_t& expr) {
    char c;

    // expr_t {type: ST_Operator}

    for(size_t ix = 0; ix < data.size(); ix++) {

    }

    return 0;
  }

}}}
