
#include "tuum_cmv.hpp"

namespace tuum { namespace CMV {

  const size_t BLOB_MIN_AREA = 1;

  int rle(uint8_t* data, size_t length, const Filter& flt, BlobSet& out) {
    RunlineSet rlines;

    region_segment(data, length, flt, rlines);

    if(rlines.size() > 0)
      region_merge(rlines, out);

    return 0;
  }

  void region_segment(uint8_t* dat, size_t length, const Filter& flt, RunlineSet& out) {
    size_t i, clss;
    uint8_t y, u, v;

    size_t X = 0, Y = 0, r0 = 0, r1 = 0;

    rl_t blob_line;
    blob_line.cls = 0;

    for(i = 0; i < length; i += 3) {
      X++;
      if(X >= 640) {
        Y++;
        X = 0;

        if(blob_line.cls != 0) {
          if(blob_line.x1 - blob_line.x0 > 10)
            out.push_back(blob_line);
          blob_line.cls = 0;
        }
      }

      y = dat[i] * U8_16CLS_REDUCTION;
      u = dat[i + 1] * U8_32CLS_REDUCTION;
      v = dat[i + 2] * U8_32CLS_REDUCTION;

      clss = flt.clss_Y[y] && flt.clss_U[u] && flt.clss_V[v];

      if((blob_line.cls != 0) && (blob_line.cls != clss)) {
        blob_line.x1 = X;
        out.push_back(blob_line);

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
  }


  void region_merge(std::vector<rl_t> rows, BlobSet& closed_blobs) {
    BlobSet active_blobs;

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
  }

}}


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
