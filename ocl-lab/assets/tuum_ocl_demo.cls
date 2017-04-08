/** @file tuum_ocl_demo.cls
 *  @file OpenCL Tuum demo script.
 *
 *  @author Meelik Kiik (kiik.meelik@gmail.com)
 *  @date 14. February 2017
 *  @version 0.1
 */

// Test comment

Pipeline "TuumVisionPpl" {

  // Public properties
  Properties {
    Buffer Input_YUYV ( "Input YUYV", 1280, 860, 1 );
    Buffer Data_YUV ( "Data YUV", 1280, 860, 1 );
    Buffer Output_RGB ( "Output RGB", 1280, 860, 1 );
  }

  /*
  Kernel "stdlib" {
    typedef struct iformat_t {
      unsigned long int width, height;
      unsigned long int bytesPerPixel, bytesPerLine; // bytesPerLine == stride
    };
  }*/

  Kernel "ImageDecode" {

    void kernel YUYV_to_YUV(global const char* src, global char* dst) {
      const unsigned int id = get_global_id(0);

      const unsigned int in_ix = id * 2 * 2;
      const unsigned int out_ix = id * 3 * 2;

      dst[out_ix] = src[in_ix];
      dst[out_ix + 1] = src[in_ix + 1];
      dst[out_ix + 2] = src[in_ix + 3];

      dst[out_ix + 3] = src[in_ix + 2];
      dst[out_ix + 4] = src[in_ix + 1];
      dst[out_ix + 5] = src[in_ix + 3];
    }

  }

  Kernel "ToHumanColor" {

    void kernel YUV_to_RGB(global const char* src, global char* dst) {
      const unsigned int id = get_global_id(0);

      dst[id] = src[id]; // TODO
    }

  }

  Procedure {
    Pass {
      Range (0, 460800, 0 ) // 1280*720/2
      Call (ImageDecode, Input_YUYV, Data_YUV );
    }

    Pass {
      Range (0, 921600, 0 ) // 1280*720
      Call (ToHumanColor, Data_YUV, Output_RGB );
    }
  }

}
