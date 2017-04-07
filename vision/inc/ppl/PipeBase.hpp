
#ifndef TUUM_PIPEBASE_H
#define TUUM_PIPEBASE_H

#include "tuum_resource.hpp"
#include "tuum_buff.hpp"

namespace Glip {
  namespace CoreGL {
    class HdlTexture;
    class HdlTextureFormat;
  }

  namespace CorePipeline {
    class Pipeline;
  }

  namespace Modules {
    class LayoutLoader;
  }
}

namespace tuum { namespace vision {

  static const char* DEF_PPL = "./assets/placeholder.ppl";

  typedef Glip::CoreGL::HdlTexture Texture;
  typedef Glip::CoreGL::HdlTextureFormat TxFormat;
  typedef Glip::Modules::LayoutLoader LayoutLoader;
  typedef Glip::CorePipeline::Pipeline Pipeline;

  LayoutLoader* create_layout_loader();

  class PipeBase : public rsc::Dynamic
  {
  public:
    PipeBase():
      m_glPipe(nullptr),
      mLdr(nullptr)
    {

    };

    virtual int init() {};
    virtual int pass(image_t& io) {};

    virtual bool isReady() { return false; }

    int load(const char*, const char*);

    Pipeline* get() { return m_glPipe; }

    void use(LayoutLoader* ldr) { mLdr = ldr; }

  protected:
    Pipeline* m_glPipe;
    LayoutLoader* mLdr;
  };

}}

#endif
