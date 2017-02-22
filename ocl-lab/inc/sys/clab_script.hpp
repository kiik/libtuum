
#ifndef CLAB_SCRIPT_H
#define CLAB_SCRIPT_H

#include "core/clab_types.hpp"

#include "tuum_ocl_ppl.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  class PipelineScript
  {
  public:
    PipelineScript();
    PipelineScript(const char*);

    int load();
    int load(const char*);

    int createPipeline(Pipeline*);

  private:
    std::string mPath;
    script_ctx_t mCtx;

    bool mContextReady;
  };

}}}

#endif
