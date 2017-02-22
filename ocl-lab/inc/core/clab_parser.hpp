
#ifndef CLAB_PARSER_H
#define CLAB_PARSER_H

#include "core/clab_types.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  class Parser
  {
    friend class lab::PipelineParser;
    friend class lab::KernelParser;

  public:

    //readExpression(expr_t);

    // expr_t("Kernel::XXX_TO_YYY(Buffer::B1, Buffer::B2)", ST_Call)
    // NSP::FN_NAME(ARGS) - ST_FunctionCall -> {ST_Namespace, ST_FunctionName}

    typedef std::map<const std::string, SymbolType> SymbolMap;
    typedef std::vector<SymbolType> SymbolSet;

    typedef std::map<const std::string, KeywordType> KeywordMap;

    typedef std::string Symbol_t;
    typedef std::vector<Symbol_t> SymbolSet_t;

    static SymbolMap gSymbolMap, gOperatorMap;
    static KeywordMap gKeywordMap;

    Parser();

    int load(const char*);
    int parse();
    int make(ocl::Pipeline*&);

    std::string getBuffer() { return *mBuffer; }

    script_ctx_t* getScriptContext() { return &mCtx; }

  protected:
    // Matces the end of given buffer to a symbol type.
    SymbolType matchSymbol(const std::string*);
    SymbolType matchSymbol(const std::string*, const SymbolSet&);

    // Simpler overloads using 'Reader' member buffer.
    SymbolType matchSymbol(const SymbolSet&);
    SymbolType matchSymbol();

    // Symbol subtype parsing.
    SymbolType matchOperator(const std::string&);

    // Keyword / dynamic symbol matching.
    SymbolType matchKeyword(const std::string&, KeywordType&);
    SymbolType matchKeyword(const std::string&);

    // Symbol iteration methods.
    int readUntil(SymbolType&, const SymbolSet&);

    int readSymbol(SymbolType&);
    int readSymbol(SymbolType&, const SymbolSet&);

    // Keyword (special symbol) handling.
    int handleKeyword(const KeywordType&);

    // Expression iteration methods.
    int readExpression(expr_t&);
    int handleExpression(const expr_t&);

    // Misc
    int handleScopeLiteral(const char);
    int readScopeAsString(expr_t&);

    // Integrated atomic parse modules
    int parseScope();

    int parseComment();
    int parseString();
    int parseOperator();

    int readKeyword();


  private:
    size_t scope_seq = 0;
    size_t comment_type = 0;

    script_ctx_t mCtx;

    Reader* mReader;

    std::string mSource, *mBuffer;
    const char* mDataPtr;
  };

}}}

#endif
