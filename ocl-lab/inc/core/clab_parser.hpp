
#ifndef CLAB_PARSER_H
#define CLAB_PARSER_H

#include "core/clab_lang.hpp"
#include "core/clab_types.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  class Parser
  {
    friend class lab::ParserModule;

    friend class lab::PipelineParser;
    friend class lab::PropertyParser;
    friend class lab::KernelParser;
    friend class lab::ProcedureParser;

  public:
    Parser();

    int load(const char*);
    int parse(script_ctx_t*);

    std::string getBuffer() { return *mBuffer; }
    std::string* getBufferPtr() { return mBuffer; }

    script_ctx_t* getScriptContext() { return mCtx; }

  protected:
    Token matchToken(const TokenSet&);
    Token matchToken();

    int readUntil(Token&, const TokenSet&);

    int parseComment();
    int parseString(expr_t&);
    int parseTuple(expr_t*);
    int parseScope(expr_t*);
    int parseNamespace(expr_t*);

    int readExpression(expr_t*);
    int readContainer(expr_t*, const Token);
    int readScopeAsString(expr_t&);

    int handleKeyword(expr_t*);
    int handleSymbol(expr_t*);
    int handleExpression(expr_t*);

    int read(const TokenSet&, expr_t*);

  private:
    script_ctx_t* mCtx;

    Reader* mReader;

    std::string mSource, *mBuffer;
    const char* mDataPtr;
  };

}}}

#endif
