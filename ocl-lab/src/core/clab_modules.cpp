
#include <boost/algorithm/string/predicate.hpp>

#include "tuum_logger.hpp"

#include "core/clab_types.hpp"
#include "core/clab_parser.hpp"

#include "core/clab_modules.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  int ParserModule::matchCustomSymbol(const std::string& match) {
    if(boost::algorithm::ends_with(getBufferPtr()->c_str(), match.c_str())) return 1;
    return -1;
  }

  std::string ParserModule::getBuffer() { return gParser->getBuffer(); }
  std::string* ParserModule::getBufferPtr() { return gParser->getBufferPtr(); }

  /*
  ParserModule::ScopeSignal ParserModule::scopeStep(SymbolType& type) {
    if(gParser->readSymbol(type) < 0) return ScopeSignal::ScopeError;

    switch(type) {
      case Token::TK_ScopeEnd:
        mScopeSeq--;
        if(mScopeSeq == 0) return ScopeSignal::OutOfScope;
        return ScopeSignal::Exit;
      case SymbolType::ST_ScopeBegin:
        mScopeSeq++;
        return ScopeSignal::Enter;
    }

    return ScopeSignal::Continue;
  }

  int ParserModule::scopeEnter() {
    SymbolType type;

    if(scopeStep(type) != ScopeSignal::Enter) {
      RTXLOG(format("Error - expected scope, got <symbol '%s'(%i)>", gParser->getBuffer().c_str(), type));
      return -2;
    }

    return 1;
  }*/

  int ParserModule::read(expr_t* out) {
    return gParser->readExpression(out);
  }

  int ParserModule::read(const TokenSet& filter, expr_t* out) {
    if(gParser->readExpression(out) < 0) return -1;

    for(auto it = filter.begin(); it != filter.end(); it++) {
      if(*it == out->type) return 1;
    }

    printf("Error - unexpected token %i ('%s')\n", out->type, out->str_val.c_str());

    return -2;
  }

  int ParserModule::enterScope(expr_t* out) {
    if(read({Token::TK_Scope}, out) < 0) return -1;
    return 1;
  }

  int ParserModule::parseScope(expr_t* out) {
    return gParser->parseScope(out);
  }

  void ParserModule::errUnexpSymbol(const Token& type) {
    RTXLOG(format("Error - unexpected symbol '%s'(%i)", gParser->getBuffer().c_str(), type));
  }

}}}
