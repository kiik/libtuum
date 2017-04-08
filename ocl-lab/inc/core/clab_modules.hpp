
#ifndef CLAB_MODULES_H
#define CLAB_MODULES_H

#include "core/clab_lang.hpp"
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

    ParserModule(Parser* p, expr_t* expr):
      gParser(p),
      mExpr(expr)
    {

    }

    std::string getBuffer();
    std::string* getBufferPtr();

    int matchCustomSymbol(const std::string&);

    int read(expr_t*);
    int read(const TokenSet&, expr_t*);

    int enterScope(expr_t*);
    int parseScope(expr_t*);

    ScopeSignal scopeStep(Token&);
    int scopeEnter();

    void errUnexpSymbol(const Token&);

    virtual int parse() = 0;

  protected:
    Parser* gParser;
    expr_t* mExpr;
  };

}}}

#include "sys/clab_properties.hpp"
#include "sys/clab_kernel.hpp"
#include "sys/clab_procedure.hpp"
#include "sys/clab_pipeline.hpp"

#endif
