
#include "tuum_logger.hpp"

#include "core/clab_parser.hpp"

#include "sys/clab_script.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  PipelineScript::PipelineScript():
    mContextReady(false)
  {

  }

  PipelineScript::PipelineScript(const char* path):
    mContextReady(false)
  {
    load(path);
  }

  int PipelineScript::load() {
    //TODO
    mContextReady = false;
    Parser p;

    if(p.load(mPath.c_str()) < 0) {
      RTXLOG("Script load failed!");
      return -1;
    }

    if(p.parse(&mCtx) < 0) {
      RTXLOG("Parsing failed!");
      return -2;
    }

    mContextReady = true;

    return 1;
  }

  int PipelineScript::load(const char* path) {
    mPath = path;
  }

  int PipelineScript::createPipeline(Pipeline* ppl)
  {

  }

}}}
