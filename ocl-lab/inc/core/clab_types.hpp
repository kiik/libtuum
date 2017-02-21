
#ifndef CLAB_TYPES_H
#define CLAB_TYPES_H

#include <string>
#include <vector>
#include <map>

#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120

#include <CL/cl2.hpp>

namespace tuum {
namespace ocl {

  typedef std::string name_t;

  typedef std::map<name_t, cl::Buffer*> BufferSet;



  class Pipeline;
  class Pass;

  typedef std::vector<Pass> PassSet;



  // Script related structures
  namespace lab {
    struct script_ctx_t {
      Pipeline* ppl;

      BufferSet buffers;
      PassSet passes;
    };

    struct property_t {
      std::string varName, name, typeName;
    };


    class Reader;
    class Parser;
    class ParserModule;

    class PipelineParser;
    class KernelParser;


    enum KeywordType {
      KW_Unknown = -2,
      KW_Invalid = -1,

      KW_Pipeline,

      KW_Properties,
      KW_Kernel,
      KW_Pass,
      KW_Procedure,
    };

    enum SymbolType {
      ST_Unknown = -2,
      ST_Invalid = -1,

      // Atomic types
      ST_Keyword,
      ST_Literal,
      ST_StringLiteral,

      // Basic types
      ST_Terminator,
      ST_LineFeed,
      ST_Operator,

      ST_TupleBegin,
      ST_TupleEnd,
      ST_ScopeBegin,
      ST_ScopeEnd,

      ST_Comment,
      ST_CommentBlock,
      ST_CommentBlockE,


      // High level types
      ST_Identifier,
      ST_Namespace,

      // Complex types
      ST_Call,
    };


    struct expr_t {
      std::string data;
      SymbolType type;
    };

    struct expr_call_t : public expr_t {
      expr_t fname;

      expr_t* args = nullptr;
      size_t argc = 0;
    };

  }

}}

#endif
