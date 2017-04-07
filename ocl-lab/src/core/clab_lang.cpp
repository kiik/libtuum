
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include "tuum_logger.hpp"

#include "core/clab_lang.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  size_t symbol_id_seq = 1;
  size_t expr_id_seq = 1;

  /** @name gTokenMap
  *  @brief Mapping of symbol literals to token types.
  */
  TokenMap gTokenMap = {
    {std::string("\""), Token::TK_String},

    {std::string("("),   Token::TK_Tuple}, {")",   Token::TK_TupleE},
    {std::string("{"),   Token::TK_Scope}, {"}",   Token::TK_ScopeE},

    {std::string(" "), Token::TK_Terminator},
    {std::string(";"), Token::TK_Terminator},
    {std::string(","), Token::TK_Terminator},

    {std::string("\n"), Token::TK_LineFeed},

    {std::string("//"), Token::TK_Comment},
    {std::string("/*"), Token::TK_CommentBlock},
    {std::string("*/"), Token::TK_CommentBlockE}
  };

  /** @name gKeywordMap
   *  @brief Mapping of static script keywords.
   */
  KeywordMap gKeywordMap = {
    {std::string("Pipeline"), Keyword::KW_Pipeline},

    {std::string("Properties"), Keyword::KW_Properties},

    {std::string("Kernel"), Keyword::KW_Kernel},
    {std::string("Procedure"), Keyword::KW_Procedure}
  };



  Token match_token(const std::string* buf)
  {
    Token out = Token::TK_Unknown;

    for(auto it = gTokenMap.begin(); it != gTokenMap.end(); it++) {
      if(boost::algorithm::ends_with(buf->c_str(), it->first)) {
        out = it->second;
        break;
      }
    }

    return out;
  }

  Token match_token(const std::string* buf, const TokenSet& set)
  {
    Token out = match_token(buf);

    for(auto it = set.begin(); it != set.end(); it++) {
      if(out == *it)
        return out;
    }

    return Token::TK_Unknown;
  }

  Keyword match_keyword(const std::string& buf) {
    Keyword out = Keyword::KW_Unknown;

    for(auto it = gKeywordMap.begin(); it != gKeywordMap.end(); it++) {
      if(boost::algorithm::ends_with(buf.c_str(), it->first)) {
        out = it->second;
        break;
      }
    }

    return out;
  }


  /**
   *  expr_t { symbol_t{ST_Operator, ST_TupleBegin}, "(...)", args={expr_t(...), ...} }
   *
   */
   /*
  int parse_expression(char* dptr, expr_t& expr, SymbolType EOS = SymbolType::ST_Unknown) {
    symbol_t sy;
    if(read_symbol(sy.type, sy.subType, dptr) < 0) return -1;

    switch(sy.type) {
      case SymbolType::ST_Operator:
        switch(sy.subType) {
          case SymbolType::ST_TupleBegin:
            //expr.type = type;
            //expr.subType = sub_type;
            printf("ST_TupleBegin\n");
            break;
          case SymbolType::ST_TupleEnd:
            printf("ST_TupleEnd\n");
            break;
        }
        break;
      default:
        return -100;
    }

    switch(EOS) {
      case SymbolType::ST_Unknown:
        EOS = SymbolType::ST_Operator;

        //parse_expression(data, );
        break;
      default:
        // Return symbol finish
        break;
    }

  }*/

  int parse_numeric(expr_t* expr) {
    try {
      expr->int_val = boost::lexical_cast<int>( expr->str_val );
      expr->type = Token::TK_Integer;
    } catch( boost::bad_lexical_cast const& ) {
      return -1;
    }

    return 1;
  }


}}}
