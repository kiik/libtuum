/** @file tuum_visualizer.hpp
 *  @brief Static utility class providing data visualization methods.
 *
 *  @author Meelik Kiik ( kiik.meelik@gmail.com )
 *  @version 0.1
 *  @date 2. May 2017
 */

#ifndef TUUM_VISUALIZER_H
#define TUUM_VISUALIZER_H

#include "tuum_buff.hpp"
#include "lpx_iformat.hpp"

#include "Vec.hpp"

#include "tuum_fs.hpp"

namespace tuum {

  const std::vector<Vec2i> ArrowSymbol = {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0},
                                      {4, 1}, {3, 2}, {2, 3}, {4, -1}, {3, -2}, {2, -3}};


  class Visualizer
  {
  public:

    static image_t NewCanvas(size_t X, size_t Y) {
      image_t out;

      img_prop_t prop(X, Y);
      Env::spawnBuffer(out, prop.getSize());
      out->setFormat(prop);

      return out;
    }

    /** :param Container: 2D vector set STL input **/
    template<typename Container>
    static image_t Visualize2D(Container& input, size_t X = 500, size_t Y = 500, double Scale = 1.0)
    {
      image_t out = Visualizer::NewCanvas(X, Y);

      return Visualizer::Visualize2D(input, out, X, Y, Scale);
    }

    template<typename Container>
    static image_t Visualize2D(Container& input, image_t out, size_t X, size_t Y, double Scale = 1.0)
    {
      if(input.size() == 0) return out;

      auto buf = input[0];  // Catch 'Container::value_type'
      char* data = (char*)out->data;

      int cX = X / 2, cY = Y / 2, ix;

      /*
      printf("range: x: -%i...%i, y: -%i...%i.\n", cX, cX, cY, cY);
      printf("scale: %.2f meters.\n", 1.0 / Scale);
      */

      int x, y;
      for(auto it = input.begin(); it != input.end(); it++) {
        buf = *it;

        x = buf.x * Scale;
        y = buf.y * Scale;

        if((-cX <= x) & (x < cX)) {
          if((-cY <= y) & (y < cY)) {
            ix = out->frm.toPixelIndex(x + cX, y + cY);
            data[ix] = 255;
          }
        }
      }

      for(auto it = ArrowSymbol.begin(); it != ArrowSymbol.end(); it++) {
        data[out->frm.toPixelIndex(cX + it->x, cX + it->y) + 1] = 255;
      }

      return out;
    }

    static int SaveImage(image_t in, const char* fn = "./visualiser_test.tmp.png") {
      image_t buf = tuum::lpx::rgb_to_png(in);

      File* fp = NewFile(fn);
      fp->bwrite((char*)buf->data, buf->getSize());

      return 0;
    }

  };

}

#endif
