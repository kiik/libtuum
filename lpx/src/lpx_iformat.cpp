
#include <png.h>
#include <vector>
#include <iostream>
#include <stdlib.h>

#include "lpx_iformat.hpp"

namespace tuum { namespace lpx {

  //Glib::RefPtr<Gdk::Pixbuf> img;


  //encode and write PNG to memory (std::vector) with libpng on C++

  typedef unsigned char ui8;
  #define ASSERT_EX(cond, error_message) do { if (!(cond)) { std::cerr << error_message; exit(1);} } while(0)

  static void PngWriteCallback(png_structp  png_ptr, png_bytep data, png_size_t length) {
      std::vector<ui8> *p = (std::vector<ui8>*)png_get_io_ptr(png_ptr);
      p->insert(p->end(), data, data + length);
  }

  template <class T>
  void wrapArrayInVector( T *sourceArray, size_t arraySize, std::vector<T, std::allocator<T> > &targetVector ) {
    typename std::_Vector_base<T, std::allocator<T> >::_Vector_impl *vectorPtr =
      (typename std::_Vector_base<T, std::allocator<T> >::_Vector_impl *)((void *) &targetVector);
    vectorPtr->_M_start = sourceArray;
    vectorPtr->_M_finish = vectorPtr->_M_end_of_storage = vectorPtr->_M_start + arraySize;
  }

  template <class T>
  void releaseVectorWrapper( std::vector<T, std::allocator<T> > &targetVector ) {
    typename std::_Vector_base<T, std::allocator<T> >::_Vector_impl *vectorPtr =
          (typename std::_Vector_base<T, std::allocator<T> >::_Vector_impl *)((void *) &targetVector);
    vectorPtr->_M_start = vectorPtr->_M_finish = vectorPtr->_M_end_of_storage = NULL;
  }

  struct TPngDestructor {
      png_struct *p;
      TPngDestructor(png_struct *p) : p(p)  {}
      ~TPngDestructor() { if (p) {  png_destroy_write_struct(&p, NULL); } }
  };

  image_t rgb_to_png(image_t in) {
    // Initialize output.

    // Use 'vector' memory management.
    std::vector<ui8> vOut;
    //wrapArrayInVector<ui8>((ui8*)out->data, out->getSize(), vOut);

    size_t w = in->frm.width, h = in->frm.height, bpp = in->frm.bytesPerPixel;

    // Prepare png context
    png_structp p = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    ASSERT_EX(p, "png_create_write_struct() failed");

    TPngDestructor destroyPng(p);
    png_infop info_ptr = png_create_info_struct(p);
    ASSERT_EX(info_ptr, "png_create_info_struct() failed");

    ASSERT_EX(0 == setjmp(png_jmpbuf(p)), "setjmp(png_jmpbuf(p) failed");

    png_set_IHDR(p, info_ptr, w, h, 8,
            PNG_COLOR_TYPE_RGB,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT,
            PNG_FILTER_TYPE_DEFAULT);
    //png_set_compression_level(p, 1);

     // Store row pointers.
    std::vector<ui8*> rows(h);
    for (size_t y = 0; y < h; ++y)
        rows[y] = (ui8*)in->data + y * w * bpp;

    png_set_rows(p, info_ptr, &rows[0]);

    // Do write.
    png_set_write_fn(p, &vOut, PngWriteCallback, NULL);
    png_write_png(p, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    // Savely release 'vector' memory management object.
    //releaseVectorWrapper(vOut);

    image_t out(new img_buf_t());
    out->init(vOut.size());
    out->setFormat(*in);

    for(size_t i=0; i < vOut.size(); i++) {
      uint8_t* p = (uint8_t*)out->data;
      p[i] = vOut[i];
    }

    return out;
  }

  //FIXME: Pixbuf pthread error on NUC
  image_t rgb_to_jpg(image_t in) {
    //img = Gdk::Pixbuf::create_from_data((const guint8*)in->data, Gdk::COLORSPACE_RGB, false, 8, in->frm.width, in->frm.height, in->frm.stride);

    image_t out(new img_buf_t());
    out->setFormat(*in);
    //img->save_to_buffer((gchar*&)out->data, out->size, "jpeg");

    return out;
  }

}}
