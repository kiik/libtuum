
#include "tuum_threshold.hpp"

namespace tuum { namespace vision {

  size_t BALL_MASK = 0b1;

  struct CMVClass {
    uint8_t mn[3], mx[3];
  };

  CMVClass BALL = {51, 100, 148, 140, 123, 165};

  CMVClass RGB_BALL = {130, 44, 30, 198, 112, 98};

  struct CMVFilter {
    uint32_t clss_Y[16], clss_U[32], clss_V[32];

    size_t id_seq;

    CMVFilter():
      id_seq(1)
    {
      size_t i;
      for(i = 0; i < 16; i++) clss_Y[i] = 0;
      for(i = 0; i < 32; i++) {
        clss_U[i] = 0;
        clss_V[i] = 0;
      }

      uint32_t id;
      addClass(BALL, id);
    }

    int addClass(CMVClass cls, uint32_t& out) {
      if(getId(out) < 0) return -1;
      for(size_t i = cls.mn[0] * U8_16CLS_REDUCTION; i <= cls.mx[0] * U8_16CLS_REDUCTION; i++) clss_Y[i] |= out;
      for(size_t i = cls.mn[1] * U8_32CLS_REDUCTION; i <= cls.mx[1] * U8_32CLS_REDUCTION; i++) clss_U[i] |= out;
      for(size_t i = cls.mn[2] * U8_32CLS_REDUCTION; i <= cls.mx[2] * U8_32CLS_REDUCTION; i++) clss_V[i] |= out;
      return 0;
    }

    int getId(uint32_t& out) {
      if(id_seq >= (1 << 31)) return -1;
      out = id_seq;
      id_seq = id_seq << 1;
      return 0;
    }

  };

  CMVFilter gFlt;

  class Blob {
  public:
    // Runline type
    struct rl_t {
      size_t x0 = 0, x1 = 0, y = 0;
      uint32_t cls = 0;

      bool isTouching(rl_t o) {
        if(abs(o.y - y) > 1) return false;
        if((o.x1 < x0) || (o.x0 > x1)) return false;
        return true;
      }
    };

  };

  struct rect_t {
    size_t x0 = 0, y0 = 0, x1 = 0, y1 = 0;

    size_t getArea() {
      return (y1 - y0) * (x1 - x0);
    }

  };

  struct blob_t {
    std::vector<Blob::rl_t> rls; // Runlines

    rect_t rect;

    void addRunline(Blob::rl_t rl) {
      if(rl.x0 < rect.x0) rect.x0 = rl.x0;
      if(rl.x1 > rect.x1) rect.x1 = rl.x1;

      if(rls.size() == 0) {
        rect.x0 = rl.x0;
        rect.y0 = rl.y;
        rect.y1 = rl.y;
      }
      else
      {
        if(rl.y > rect.y1) rect.y1 = rl.y;
      }

      rls.push_back(rl);
    }

    size_t getArea() {
      return rect.getArea();
    }

  };

  struct blob_img_info_t {
    std::vector<blob_t> data;
  };

  Timer debugTimer;
  std::vector<blob_t> closed_blobs;
  std::vector<blob_t> active_blobs;

  const size_t BLOB_MIN_AREA = 1;

  void rle(std::vector<Blob::rl_t>& rows) {
    // Reuse reserved memory from last run
    closed_blobs.clear();
    active_blobs.clear();

    size_t Y = 0;
    bool blobbed;
    for(auto bline = rows.begin(); bline < rows.end(); bline++) {
      blobbed = false;

      // If new image row
      if(bline->y != Y) {
        Y = bline->y;
      }

      // Find active blobs into which insert runlines
      for(auto blob_it = active_blobs.begin(); blob_it < active_blobs.end();) {
        if(blob_it->rls.back().isTouching(*bline)) {
          // Runline touching blob, add it if classes fit
          if(blob_it->rls.back().cls == bline->cls) {
            blob_it->addRunline(*bline);
            blobbed = true;
          }
        } else {
          // If row changed and blob wasn't updated it is closed
          if(blob_it->rls.back().y != Y) {
            if(blob_it->getArea() > BLOB_MIN_AREA)
              closed_blobs.push_back(*blob_it);
            blob_it = active_blobs.erase(blob_it);
            continue;
          }
        }

        blob_it++;
      }

      if(!blobbed) {
        blob_t blob;
        blob.addRunline(*bline);
        active_blobs.push_back(blob);
      }

    }

    // Close rest of active blobs
    if(active_blobs.size() > 0) {
      for(auto blob_it = active_blobs.begin(); blob_it < active_blobs.end(); blob_it++)
        if(blob_it->getArea() > BLOB_MIN_AREA) closed_blobs.push_back(*blob_it);
    }

    /*
    if(debugTimer.isTime()) {
      double avg = 0.0;
      if(closed_blobs.size() > 0) {
        size_t n = 0, S;
        for(auto blob_it = closed_blobs.begin(); blob_it < closed_blobs.end(); blob_it++) {
          S = blob_it->getArea();
          avg += S;
          n++;
          printf("<Blob S=%lu, R=(%lu, %lu, %lu, %lu)>\n", S, blob_it->rect.x0, blob_it->rect.y0, blob_it->rect.x1, blob_it->rect.y1);
        }

        avg = avg / n;
      }

      printf("rows=%lu, blobs=%lu; S.avg=%.2f\n", rows.size(), closed_blobs.size(), avg);
      debugTimer.start(1000);
    }*/
  }

  void classify_colors(uint8_t* dat, size_t length) {
    size_t i, clss;
    uint8_t y, u, v;

    size_t X = 0, Y = 0, r0 = 0, r1 = 0;

    Blob::rl_t blob_line;
    blob_line.cls = 0;

    std::vector<Blob::rl_t> rows;

    for(i = 0; i < length; i += 3) {
      X++;
      if(X >= 640) {
        Y++;
        X = 0;

        if(blob_line.cls != 0) {
          if(blob_line.x1 - blob_line.x0 > 10)
            rows.push_back(blob_line);
          blob_line.cls = 0;
        }
      }

      y = dat[i] * U8_16CLS_REDUCTION;
      u = dat[i + 1] * U8_32CLS_REDUCTION;
      v = dat[i + 2] * U8_32CLS_REDUCTION;

      clss = gFlt.clss_Y[y] && gFlt.clss_U[u] && gFlt.clss_V[v];

      if((blob_line.cls != 0) && (blob_line.cls != clss)) {
        blob_line.x1 = X;
        rows.push_back(blob_line);

        blob_line.cls = 0;
      }

      if(blob_line.cls == 0) {
        blob_line.cls = clss;
        blob_line.x0  = X;
        blob_line.y  = Y;
      }

      // Threshold
      if(clss == 0) {
        dat[i] = 0;
        dat[i + 1] = 0;
        dat[i + 2] = 0;
      }

    }

    rle(rows);

  }

}}
