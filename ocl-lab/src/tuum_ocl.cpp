
#include <iostream>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

#include "tuum_logger.hpp"

#include "tuum_fs.hpp"
#include "tuum_hal.hpp"

#include "tuum_ocl.hpp"

#include "sys/clab_script.hpp"

namespace tuum {

  namespace ocl {
    cl::Platform gPlatform;
    cl::Device gDevice;
  }


  int ocl_init_platform() {
    std::vector<cl::Platform> all_platforms;

    cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0) {
      std::cout<<" No platforms found. Check OpenCL installation!\n";
      exit(1);
    }

    std::cout << all_platforms.size() << " platforms found." << std::endl;

    ocl::gPlatform = all_platforms[0];

    std::cout << "Using platform: "<<ocl::gPlatform.getInfo<CL_PLATFORM_NAME>()<<"\n";

    return 0;
  }

  int ocl_init_device() {
    std::vector<cl::Device> all_devices;

    ocl::gPlatform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size()==0){
      std::cout<<" No devices found. Check OpenCL installation!\n";
      exit(1);
    }

    ocl::gDevice = all_devices[0];
    std::cout<< "Using device: "<<ocl::gDevice.getInfo<CL_DEVICE_NAME>()<<"\n";

    return 0;
  }

  ImageStream* strm = nullptr;

  image_t frame, oframe;

  bool test_init = false;



  void ocl_run_tests() {

  }


  int ocl_init() {
    ocl_init_platform();
    ocl_init_device();

    //ocl_run_tests();
  }

  void ocl_setup() {
    Env::spawnBuffer(frame);
    Env::spawnBuffer(oframe);

    strm = hal::hw.getCamera()->getStream();
    strm->initBuf(frame);
    frame->id = 0;

    ocl::Pipeline* ppl_ptr = nullptr;
    ocl::loadScript("./assets/ocl_tests.cls", ppl_ptr);
  }

  bool test = false;

  void ocl_process() {
    if(test) {
      strm->read(frame);
      if(frame->id <= oframe->id) return;

      ocl_run_tests();

      /*
      size_t t0 = millis(), t1;
      image_t png = lpx::rgb_to_png(oframe);
      File* file = tuum::NewFile("./frame.png");
      file->bwrite((char*)png->data, png->getSize());
      delete(file);
      t1 = millis();
      RTXLOG(format("fs %lums", (t1 - t0)));*/

      test = true;
      oframe->id = frame->id;
    }
  }

}


namespace tuum {
namespace ocl {

  int loadScript(const char* path, ocl::Pipeline*& out) {
    lab::PipelineScript scr(path);

    RTXLOG("Loading script...");

    if(scr.load() < 0) return -1;

    RTXLOG("Creating pipeline...");

    Pipeline *ppl = nullptr;
    if(scr.createPipeline(ppl) < 0) return -2;

    RTXLOG("Tests done.");

    if(ppl != nullptr)
      delete(ppl);

    RTXLOG("Cleanup done.");

    return -100;
  }

}}
