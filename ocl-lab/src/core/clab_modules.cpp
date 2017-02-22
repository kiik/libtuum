
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

  ParserModule::ScopeSignal ParserModule::scopeStep(SymbolType& type) {
    if(gParser->readSymbol(type) < 0) return ScopeSignal::ScopeError;

    switch(type) {
      case SymbolType::ST_ScopeEnd:
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
  }

  void ParserModule::errUnexpSymbol(const SymbolType& type) {
    RTXLOG(format("Error - unexpected symbol '%s'(%i)", gParser->getBuffer().c_str(), type));
  }

}}}
