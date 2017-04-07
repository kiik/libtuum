
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


  class PipelineScript;


  // Script related structures
  namespace lab {
    struct property_t {
      static size_t idSeq;

      size_t id;
      std::string varName, typeName;

      property_t(const std::string& vName, const std::string& tName):
        varName(vName), typeName(tName)
      {
        id = property_t::idSeq++;
      }
    };

    struct buffer_t : public property_t {
      std::string name;

      buffer_t(const std::string& varName, const std::string& bufName):
        property_t(varName, "Buffer"), name(bufName)
      {

      }
    };

    struct kernel_t : public property_t {
      std::string body;

      kernel_t(const std::string& vName, const std::string& bData):
        property_t(vName, "Kernel"), body(bData)
      {

      }
    };

    struct procedure_t {
      size_t range[3];
      size_t kernelID;

      size_t argc; // Buffer argument count
      size_t *argv; // Buffer ids

      procedure_t(size_t kID, size_t r1, size_t r2, size_t r3):
        kernelID(kID),
        argc(0), argv(nullptr)
      {
        range[0] = r1; range[1] = r2, range[2] = r3;
      }

      void clearArgs() {
        if(argv != nullptr) delete(argv);
        argv = nullptr;
        argc = 0;
      }

      void initArgc(size_t count) {
        clearArgs();
        argc = count;
        argv = new size_t[argc];
      }

    };

    struct pass_t {
      std::vector<procedure_t> procedures;
    };

    struct script_ctx_t {
      //Pipeline* ppl; #TODO

      std::vector<buffer_t> buffers;
      std::vector<kernel_t> kernels;
      std::vector<pass_t> passes;

      void add(kernel_t kernel) {

      }
    };



    class Reader;
    class Parser;
    class ParserModule;

    class PipelineParser;
    class PropertyParser;
    class KernelParser;
    class ProcedureParser;

  }

}}

#endif
