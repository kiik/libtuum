
#ifndef CLAB_LANG_H
#define CLAB_LANG_H

#include "core/clab_types.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  // word -> is container token? -> is keyword ? -> symbol
  enum Token {
    TK_Unknown = -2,
    TK_Invalid = -1,

    // Atomic
    TK_Keyword = 1, // Properties, Kernel, Buffer
    TK_Symbol,      // Unknown keyword (customly handled or reference to another)
    TK_Integer,

    // Encapsulating tokens
    TK_String,
    TK_Tuple, TK_TupleE,
    TK_Array, TK_ArrayE,

    // Syntax
    TK_Scope, TK_ScopeE,
    TK_CommentBlock, TK_CommentBlockE,

    TK_Namespace,

    TK_Comment,
    TK_Terminator,
    TK_LineFeed,
  };

  enum TokenGroup {
    TG_Literal,   // String, Integer
    TG_Container  // Tuple, Array
  };

  // Keyword - word mapped to a specialized handler
  enum Keyword {
    KW_Unknown = -2,
    KW_Invalid = -1,

    KW_Pipeline,

    KW_Properties,

    KW_Kernel,
    KW_Procedure
  };

  /*

  readSymbol -> {", Pipeline, (}

  deduce token -> {T_String, T_Keyword, T_Tuple}

  evaluate token -> {handleString("), handleKeyword(deduce kw), handleTuple}
  -> {expr_t(T_String, "xxx"), new PipelineParser()->exec(), expr_t(T_Tuple, {<elements[exprset_t]>})}


    expr_t(TK_Keyword, "Kernel")
    expr_t(TK_Keyword, "Call", {expr_t(TK_Symbol, "Kernel::ImageDecode"), expr_t(TK_Symbol, "Properties::Input_YUYV")})
  */

  enum SymbolType {
    ST_Unknown = 0,
    ST_Reference,
    ST_Function,
    ST_BoundFunction,
    ST_Class,
    ST_Object,
  };

  typedef size_t SymbolId;

  extern SymbolId symbol_id_seq;

  struct expr_t;

  struct symbol_t {
    SymbolId id;
    SymbolType type;
    std::string name;

    // Object/function upper class/namespace
    symbol_t* parent = nullptr;

    // Expression source
    expr_t* source = nullptr;

    symbol_t(SymbolType t):
      type(t)
    {
      id = symbol_id_seq++;
    }

    symbol_t(SymbolType t, std::string n):
      type(t), name(n)
    {
      id = symbol_id_seq++;
    }

    void setClass(symbol_t* ptr) { parent = ptr; }
    symbol_t* getClass() { return parent; }

    void setSource(expr_t* ptr) { source = ptr; }

  };

  typedef std::map<std::string, symbol_t*> SymbolTable;

  extern size_t expr_id_seq;

  struct expr_t {
    size_t id;

    Token type;
    TokenGroup group;

    Keyword kw;

    // Literal members
    std::string str_val;
    int int_val;

    // Container members
    expr_t* parent = nullptr;
    std::vector<expr_t*> children;

    // Scope data
    SymbolTable symbols;

    expr_t()
    {
      id = expr_id_seq++;
      //printf("new expr_t(id=%lu)\n", id);
    }

    ~expr_t()
    {
      for(auto it = symbols.begin(); it != symbols.end(); it++) {
        if(it->second->source != nullptr) {
          delete(it->second);
        }
      }

      for(auto it = children.begin(); it != children.end(); it++) {
        delete(*it);
      }

      //printf("~expr_t(id=%lu, %i, '%s', %i)\n", id, type, str_val.c_str(), int_val);
    }

    void setParent(expr_t* ptr) { parent = ptr; }
    void addChild(expr_t* ptr) { ptr->parent = this; children.push_back(ptr); }

    void setSymbolTable(const SymbolTable& in) { symbols = in; }

    int addSymbol(symbol_t* ptr) {
      /*
      if(parent == nullptr) {
        printf("expr_t: Error - root scope symbol table is static.\n");
        return -1;
      }
      */

      symbols[ptr->name] = ptr;
      return 1;
    }

    expr_t* getScope() {
      if(parent == nullptr)
        if(type == Token::TK_Scope) return this;

      if(parent->type == Token::TK_Scope) return parent;

      return parent->getScope();
    }

    //TODO: Traverse full scope tree
    symbol_t* findSymbol(const std::string& name)
    {
      auto it = symbols.find(name);
      if(it == symbols.end()) return symbols["0"];
      return it->second;
    }

    // Symbol references
    expr_t* _ref = nullptr;     // Reference to another expression
    std::vector<expr_t*> _refs; // Dependant expressions

    void setReference(expr_t* ref) {
      ref->addReference(this);
    }

    void addReference(expr_t* ref) {
      ref->_ref = this;
      _refs.push_back(ref);
    }

    void debugPrint() {
      std::string type_name;

      switch(type) {
        case Token::TK_Keyword:
          printf("<Keyword %s>\n", str_val.c_str());
          break;
        case Token::TK_Symbol:
          printf("<Symbol %s>\n", str_val.c_str());
          break;
        case Token::TK_String:
          printf("<String '%s'>\n", str_val.c_str());
          break;
        case Token::TK_Integer:
          printf("<Integer %i>\n", int_val);
          break;
        case Token::TK_Scope:
          printf("<Scope {\n");

          for(auto it = symbols.begin(); it != symbols.end(); it++)
            printf("  '%s': symbol_t(id=%lu, t=%i),\n", it->first.c_str(), it->second->id, it->second->type);

          printf("}>\n");
          break;
        case Token::TK_Tuple:
          printf("<Tuple (\n");

          for(auto it = children.begin(); it != children.end(); it++) {
            expr_t* ptr = *it;
            printf("  expr_t(id=%lu, t=%i, str='%s', int=%i),\n", ptr->id, ptr->type, ptr->str_val.c_str(), ptr->int_val);
          }

          printf(")>\n");
          break;
        case Token::TK_TupleE:
          printf("<Tuple End Token>\n");
          break;
        default:
          printf("<expr_t(unhandled) type=%i,str_val=%s>\n", type, str_val.c_str());
          break;
      }
    }

    void debugTreePrint(std::string intent = "") {
      std::string buf = str_val.size() > 20 ? "/.../" : str_val;
      printf("%sexpr_t(id=%lu, t=%i, str='%s', int=%i)", intent.c_str(), id, type, buf.c_str(), int_val);

      if(type == Token::TK_Scope || children.size() > 0) printf(" {\n");
      else printf(",\n");

      if(type == Token::TK_Scope) {
        printf("%s  Symbols: {\n", intent.c_str());
        for(auto it = symbols.begin(); it != symbols.end(); it++)
        printf("%s    '%s': symbol_t(id=%lu, t=%i),\n", intent.c_str(), it->first.c_str(), it->second->id, it->second->type);
        printf("%s  }\n", intent.c_str());
      }

      if(children.size() > 0) {
        printf("%s  Children: (\n", intent.c_str());
        for(auto it = children.begin(); it != children.end(); it++) {
          expr_t* ptr = *it;
          ptr->debugTreePrint(intent + "    ");
        }
        printf("%s  )\n", intent.c_str());
      }

      if(type == Token::TK_Scope || children.size() > 0) printf("%s}\n", intent.c_str());
    }

  };


  typedef std::map<const std::string, Token> TokenMap;
  typedef std::vector<Token> TokenSet;

  typedef std::map<const std::string, Keyword> KeywordMap;

  extern TokenMap gTokenMap;
  extern KeywordMap gKeywordMap;

  extern symbol_t sym_Unknown;

  // Matces the end of given buffer to a token.
  Token match_token(const std::string*);
  Token match_token(const std::string*, const TokenSet&);

  // Keyword matching.
  Keyword match_keyword(const std::string&);

  int parse_numeric(expr_t*);

}}}

#endif
