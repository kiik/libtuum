
#ifndef CLAB_PASS_H
#define CLAB_PASS_H

#include "core/clab_types.hpp"

namespace tuum {
namespace ocl {

  class Pass {
  public:
    struct procedure_t {
      cl::NDRange gOffset = cl::NullRange, gSize = cl::NullRange, lSize = cl::NullRange;
      std::vector<name_t> args;
      cl::Kernel* kernel;
    };

    typedef std::vector<procedure_t> ProcedureSet;

    Pass();

    void add(const procedure_t);
    void clear();

    ProcedureSet* get() { return &mProcedures; }

    int bind(Pipeline*, cl::CommandQueue*);

  protected:
    ProcedureSet mProcedures;
  };

}}

#endif
