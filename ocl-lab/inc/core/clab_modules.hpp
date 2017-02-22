
#ifndef CLAB_MODULES_H
#define CLAB_MODULES_H

#include "core/clab_types.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  class ParserModule {
  public:
    enum ScopeSignal {
      ScopeError = -1,

      OutOfScope = 0,
      Exit,
      Continue,
      Enter
    };

    ParserModule(Parser* p):
      gParser(p),
      mScopeSeq(0)
    {

    }

    std::string getBuffer();
    std::string* getBufferPtr();

    int matchCustomSymbol(const std::string&);

    ScopeSignal scopeStep(SymbolType&);
    int scopeEnter();

    void errUnexpSymbol(const SymbolType&);

    virtual int parse() = 0;

  protected:
    Parser* gParser;
    size_t mScopeSeq;
  };

}}}

#include "sys/clab_properties.hpp"
#include "sys/clab_kernel.hpp"
#include "sys/clab_procedure.hpp"
#include "sys/clab_pipeline.hpp"

#endif
