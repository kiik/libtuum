
#ifndef CLAB_LANG_H
#define CLAB_LANG_H

#include "core/clab_types.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  extern SymbolMap gSymbolMap, gOperatorMap;
  extern KeywordMap gKeywordMap;

  // Matces the end of given buffer to a symbol type.
  SymbolType match_symbol(const std::string*);
  SymbolType match_symbol(const std::string*, const SymbolSet&);

  // Simpler overloads using 'Reader' member buffer.
  SymbolType match_symbol(const SymbolSet&);
  SymbolType match_symbol();

  // Symbol subtype parsing.
  SymbolType match_operator(const std::string&);

  // Keyword / dynamic symbol matching.
  SymbolType match_keyword(const std::string&, KeywordType&);
  SymbolType match_keyword(const std::string&);

}}}

#endif
