/** @file lpx_iformat.hpp
 *  @brief Misc image transformation functions.
 *
 *  @author Meelik Kiik ( kiik.meelik@gmail.com )
 */

#ifndef TUUM_LPX_IFORMAT_H
#define TUUM_LPX_IFORMAT_H

#include <memory>

#include <glib.h>
#include <gdkmm/pixbuf.h>

#include "Camera.hpp"

namespace tuum { namespace lpx {

  image_t rgb_to_jpg(image_t);
  image_t rgb_to_png(image_t);

}}

#endif
