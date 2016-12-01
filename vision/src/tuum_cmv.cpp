
#include <stdio.h>
#include <sys/time.h>

double time_diff(struct timeval x , struct timeval y)
{
    double x_ms , y_ms , diff;

    x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
    y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;

    diff = (double)y_ms - (double)x_ms;

    return diff;
}

#include "tuum_cmv.hpp"

namespace tuum { namespace CMV {

  const size_t CMV_RUNLINE_MIN_LENGTH = 40;

  bool rl_t::isTouching(rl_t o) {
    if(abs(o.y - y) > 1) return false;

    if((o.x1 < x0) || (o.x0 > x1)) return false;

    return true;
  }

  int rle(uint8_t* data, size_t length, FilterBase& flt, BlobSet& out) {
    RunlineSet rlines;

    region_segment(data, length, flt, rlines);

    if(rlines.size() > 0)
      region_merge(rlines, out);

    return 0;
  }

  int rle(uint8_t* data, size_t length, BlobSet& out) {
    RunlinePtrSet rows;
    size_t t0, t1, t2;

    t0 = millis();
    region_segment(data, length, rows);
    t1 = millis();

    if(rows.size() > 0)
      region_group(rows, out);
    t2 = millis();

    printf("[tuum_cmv::rle - stat]segment %lums (%lun), merge %lums\n", (t1 - t0), rows.size(), (t2 - t1));

    return 0;
  }

  void color_debug(uint8_t* dat, size_t i, uint32_t clss) {
    // Threshold
    if(clss == 0) {
      dat[i] = 0;
      dat[i + 1] = 0;
      dat[i + 2] = 0;
    } else {
      if(clss == 0b1) { // Ball
      	dat[i] = 255;
      	dat[i + 1] = 128;
      	dat[i + 2] = 0;
            } else if(clss == 0b10) { // Blue goal
      	dat[i] = 0;
      	dat[i + 1] = 0;
      	dat[i + 2] = 255;
            } else if(clss == 0b100) { // Yellow goal
      	dat[i] = 255;
      	dat[i + 1] = 255;
      	dat[i + 2] = 51;
            } else { // Unclassified
      	dat[i] = 255;
      	dat[i + 1] = 51;
      	dat[i + 2] = 153;
      }
    }
  }

  void region_segment(uint8_t* dat, size_t length, FilterBase& flt, RunlineSet& out) {
    size_t i;
    uint8_t y, u, v;
    uint32_t clss = 0;

    size_t X = 0, Y = 0, r0 = 0, r1 = 0;

    rl_t blob_line;
    blob_line.cls = 0;

    for(i = 0; i < length; i += 3) {
      // Calculate image coordinates
      X++;
      if(X >= 1280) {
        Y++;
        X = 0;

        // End runline when entering new row
        if(blob_line.cls != 0) {
          if(blob_line.x1 - blob_line.x0 > CMV_RUNLINE_MIN_LENGTH)
            out.push_back(blob_line);

          blob_line.cls = 0;
        }
      }

      y = dat[i] * U8_16CLS_REDUCTION;
      u = dat[i + 1] * U8_32CLS_REDUCTION;
      v = dat[i + 2] * U8_32CLS_REDUCTION;

      clss = flt(y, u, v);

      // If new runline began close last
      if((blob_line.cls != 0) && (blob_line.cls != clss)) {
        blob_line.x1 = X;
        out.push_back(blob_line);

        blob_line.cls = 0;
      }

      if(blob_line.cls == 0) {
        // Start new runline
        blob_line.cls = clss;
        blob_line.x0  = X;
        blob_line.y  = Y;
      }

      color_debug(dat, i, clss);

    }
  }


  void region_segment(uint8_t* dat, size_t length, RunlinePtrSet& out) {
    size_t i;
    uint32_t clss = 0;

    size_t X = 0, Y = 0, r0 = 0, r1 = 0;

    rl_t* rl = new rl_t();
    rl->cls = 0;

    RunlineRow* row = new RunlineRow();

    for(i = 0; i < length; i += 3) {
      // Calculate image coordinates
      X++;
      if(X >= 1280) {
        Y++;
        X = 0;

        // End runline when entering new row
        if(rl->cls != 0) {
          if(rl->x1 - rl->x0 > CMV_RUNLINE_MIN_LENGTH) {
            if(rl->x1 > rl->x0) {
              row->push_back(rl);
              rl = new rl_t();
            }
          }

          rl->cls = 0;
        }

        out.push_back(row);
        row = new RunlineRow();
      }

      clss = dat[i];

      // If new runline began close last
      if((rl->cls != 0) && (rl->cls != clss)) {
        rl->x1 = X;

        if(rl->x1 - rl->x0 > CMV_RUNLINE_MIN_LENGTH) {
          if(rl->x1 > rl->x0) {
            row->push_back(rl);
            rl = new rl_t();
          }
        }

        rl->cls = 0;
      }

      if((clss != 0) && (rl->cls == 0)) {
        // Start new runline
        rl->cls = clss;
        rl->x0  = X;
        rl->y  = Y;
      }

      //color_debug(dat, i, clss);
    }

    delete(rl);
    delete(row);
  }

  size_t t_blob_union, t_rle_match, t_rl_append;
  double t2;


  struct timeval before, after;

  void blob_union(BlobSet::iterator& master, BlobSet::iterator& slave, BlobSet& set)
  {
    for(auto it = slave->rls.begin(); it < slave->rls.end(); it++) {
      master->addRunline(*it);
    }
    slave = set.erase(slave);
  }

  bool blob_rle_match(blob_t& blob, rl_t& rl)
  {
    for(auto it = blob.rls.rbegin(); it != blob.rls.rend(); it++ ) {
      if(it->isTouching(rl)) return true;
    }

    return false;
  }

  bool blob_match(rl_t& rl, BlobSet& out, BlobSet::iterator& last_blob)
  {
    bool blobbed = false;

    // Iterate over active blobs
    for(auto blob_it = out.begin(); blob_it < out.end();) {

      if(blob_rle_match(*blob_it, rl)) {
        // Runline touching blob, add it if classes fit
        if(blob_it->rls.back().cls == rl.cls) {
          if(!blobbed) {
            last_blob = blob_it;
            last_blob->addRunline(rl); // SUM 1.3ms
            blobbed = true;
          } else {
            // Runline matches another blob, unify both blobs
            blob_union(blob_it, last_blob, out); // SUM 6ms
            continue;
          }
        }
      }

      blob_it++;
    }



    return blobbed;
  }

  void region_merge(std::vector<rl_t>& rows, BlobSet& out) {
    t2 = 0.0;

    size_t Y = 0;
    bool blobbed;

    auto matched_blob = out.end();

    for(auto bline = rows.begin(); bline < rows.end(); bline++) {
      blobbed = false;
      matched_blob = out.end();

      // If new image row
      if(bline->y != Y) {
        Y = bline->y;
      }

      // Find active blobs into which insert runlines
      blobbed = blob_match(*bline, out, matched_blob);


      // If no blob was matched create new blob
      if(!blobbed) {
        blob_t blob;
        blob.addRunline(*bline);
        out.push_back(blob);
      }

    }

    printf("[tuum_cmv::region_merge - stat]blob_match subroutine %.0lfus \n", t2);
  }

  void rl_t::addChild(rl_t* chld) {
    if(children == nullptr)
      children = new std::vector<rl_t*>();

    if(chld->isRoot()) {
      chld->parent = this;
      children->push_back(chld);

      /*
      if(chld->children->size() > 0) {
        children->insert(children->end(), chld->children->begin(), chld->children->end());
        chld->children->clear();
      }*/
    } else {
      parent = chld->parent;
    }

  }

  void rl_add_children(blob_t& blob, rl_t* rl) {
    if(rl->children == nullptr) return;
    if(rl->children->size() == 0) return;

    //printf("add:%i,%i,%i\n", rl->x0, rl->x1, rl->y);
    for(auto it = rl->children->begin(); it != rl->children->end(); it++) {
      //printf("recv:%i,%i,%i\n", (*it)->x0, (*it)->x1, (*it)->y);

      blob.mergeRunline(*it);
      rl_add_children(blob, *it);
    }
  }

  void region_group(RunlinePtrSet& rows, BlobSet& out) {
    bool merged;
    size_t rl_N = 0, blob_N = 0;

    for(auto row_it = rows.begin(); row_it != rows.end(); row_it++) {
      if(row_it == rows.begin()) continue;
      RunlineRow* row = *row_it;

      if( row->size() == 0 ) continue;

      RunlineRow* p_row = *(row_it - 1);
      if( p_row->size() == 0 ) continue;

      for(auto rl_it = row->begin(); rl_it != row->end(); rl_it++) {
        rl_t* child = *rl_it;

        for(auto p_rl_it = p_row->begin(); p_rl_it != p_row->end(); p_rl_it++) {
          rl_t* parent = *p_rl_it;

          // If parent runline starts after us
          if(parent->x0 > parent->x1) break;

          // If parent runline ends before us
          if(parent->x1 < parent->x0) continue;

          //printf("pa:%i,%i,%i\n", parent->x0, parent->x1, parent->y);
          //printf("ch:%i,%i,%i\n", child->x0, child->x1, child->y);

          assert(parent->x1 > parent->x0);
          assert(child->x1 > child->x0);

          parent->parent->addChild(child);

        }

        rl_N++;
      }

    }

    for(auto row_it = rows.begin(); row_it != rows.end(); row_it++) {

      for(auto rl_it = (*row_it)->begin(); rl_it != (*row_it)->end(); rl_it++) {
        rl_t* ptr = *rl_it;
        if(!ptr->isRoot()) continue;
        if(ptr->children == nullptr) continue;

        blob_t blob;
        blob.mergeRunline(ptr);

        //printf("add:%i,%i,%i (%lu)\n", ptr->x0, ptr->x1, ptr->y, ptr->children->size());
        rl_add_children(blob, ptr);
        out.push_back(blob);

        blob_N++;
      }

    }

    printf("[region_group - stat]rl_N = %lu, blob_N = %lu (%lu)\n", rl_N, blob_N, out.size());

  }

}}
