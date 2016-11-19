
#include "tuum_vision.hpp"

#include "tuum_VisionFilter.hpp"

namespace tuum {

  //CMV::YUVFilter::ColorClass YUV_BALL = {17, 122, 122, 35, 130, 124};

  //CMV::YUVFilter::ColorClass YUV_BALL = {51, 100, 148, 140, 123, 165}; // [59.5,110,160.5]

  // [51.01686096191406,115,143.50048828125]> <[51,115,144]>
  //CMV::YUVFilter::ColorClass YUV_BALL = {51, 110, 140, 140, 120, 150};

  // [153,90,197]> <[153,90,197]> <[153,94,197]> <[153,98,197]> <[162,98,189]> <[98,98,193]

  // <[121.125,98,172]> [119,98,164]> <[119,98,164][178.5,98,172]
  // [136,98,197]> <[136,98,197]
  CMV::YUVFilter::ColorClass YUV_BALL = {122, 97, 170, 187, 99, 198};

  //<[138,90,140]> <[136,90,139]> <[136,90,144]> <[136,90,144]'
  // [136,90.125,139]> <[136,90,139]> <[128,90,139]> <[120,90,139]> <[136,90,148]> <[153,90,148]>
  CMV::YUVFilter::ColorClass YUV_GOAL_YEL = {110, 90, 139, 140, 90, 150};


  CMV::YUVFilter::ColorClass YUV_FIELD_GREEN = {100, 97, 97, 120, 105, 107};

  CMV::UVFilter::ColorClass UV_BALL, UV_GOAL_YEL;


  CMV::YUVFilter::ColorClass RGB_BALL = {130, 44, 30, 198, 112, 98};

  const size_t BLOB_MIN_AREA = 50;
  const double BLOB_MIN_DENSITY = 0.2;
  const double BLOB_MAX_DENSITY = 1.0;

  uint32_t ID_GOAL_YEL, ID_BALL;

  void VisionFilter::initClassifiers()
  {
    // [102,90,90,139,139]> <[119,90,90,139,139]> <[136,90,90,139,139]> <[153,90,90,148,148]
    // <[102,98,98,139,139]> <[119,90,90,139,139]> <[136,90,90,139,139]> <[153,90,90,148,148]>
    // [119,82,82,148,148]> <[136,82,82,148,148]
    // <[85,98,98,139,139]> <[102,90,90,148,148]>
    UV_GOAL_YEL.clss.push_back({85,98,98,139,139});
    UV_GOAL_YEL.clss.push_back({102,90,98,139,148});
    UV_GOAL_YEL.clss.push_back({119,82,90,139,148});
    UV_GOAL_YEL.clss.push_back({136,82,90,139,148});
    UV_GOAL_YEL.clss.push_back({153,90,90,148,148});


    // <[102,106,106,180,180]> <[119,98,98,189,189]> <[136,98,98,197,197]> <[153,98,98,205,205]> <[170,98,98,189,189]>
    // <[85,106,106,180,180]> <[102,98,98,189,189]> <[119,98,98,197,197]> <[136,98,98,197,197]> <[153,98,98,189,189]> <[170,98,98,189,189]>
    // <[68,106,106,164,164]> <[85,106,106,180,180]> <[102,98,98,180,180]> <[119,98,98,189,189]> <[136,98,98,189,189]> <[153,98,98,197,197]> <[170,98,98,189,189]>
    // <[51,115,115,148,148]> <[68,115,115,164,164]> <[85,106,106,172,172]> <[102,98,98,180,180]> <[119,98,98,180,180]>
    // <[68,106,106,164,164]> <[85,106,106,172,172]> <[102,98,98,189,189]> <[119,98,98,189,189]> <[136,98,98,189,189]> <[170,98,98,189,189]>
    // <[68,106,106,164,164]> <[85,106,106,172,172]> <[102,98,98,180,180]> <[119,98,98,180,180]> <[136,98,98,189,189]> <[170,106,106,189,189]>
    // <[68,106,106,172,172]> <[102,98,98,180,180]>
    /*
    UV_BALL.clss.push_back({51,115,115,148,148});
    UV_BALL.clss.push_back({68,106,115,164,172});
    UV_BALL.clss.push_back({85,106,106,172,180});
    UV_BALL.clss.push_back({87,106,106,156,156});
    UV_BALL.clss.push_back({102,98,106,180,189});
    UV_BALL.clss.push_back({119,98,98,180,197}); //x
    UV_BALL.clss.push_back({136,98,98,189,197});
    UV_BALL.clss.push_back({153,98,98,189,205});
    UV_BALL.clss.push_back({170,98,98,189,189});
    */

    //<[87,106,106,156,156]> <[97,106,106,162,162]> <[102,106,106,164,164]> <[108,98,98,170,170]>
    // <[119,98,98,174,174]> <[131,98,98,180,180]> <[153,104,104,189,189]> <[157,106,106,160,160]> <[161,106,106,180,180]>
    // <[170,98,98,180,180]> <[172,106,106,178,178]> <[195,106,106,164,164]>
    // <[102,98,98,180,180]> <[106,98,98,180,180]> <[112,98,98,180,180]> <[116,98,98,180,180]> <[119,98,98,180,180]> <[123,98,98,182,182]> <[129,98,98,182,182]> <[136,106,106,180,180]>
    UV_BALL.clss.push_back({87,106,106,156,156});
    UV_BALL.clss.push_back({97,106,106,162,162});
    UV_BALL.clss.push_back({102,98,106,164,180});
    UV_BALL.clss.push_back({106,98,106,164,180});
    UV_BALL.clss.push_back({108,98,98,170,170});
    UV_BALL.clss.push_back({108,98,98,170,180});
    UV_BALL.clss.push_back({119,98,98,174,180});
    UV_BALL.clss.push_back({123,98,98,174,182});
    UV_BALL.clss.push_back({129,98,98,174,182});
    UV_BALL.clss.push_back({131,98,98,180,180});
    UV_BALL.clss.push_back({136,106,106,180,180});
    UV_BALL.clss.push_back({153,104,104,189,189});
    UV_BALL.clss.push_back({157,106,106,160,160});
    UV_BALL.clss.push_back({161,106,106,180,180});
    UV_BALL.clss.push_back({170,98,98,180,180});
    UV_BALL.clss.push_back({172,106,106,178,178});
    UV_BALL.clss.push_back({195,106,106,164,164});
  }

  VisionFilter::VisionFilter()
  {
    initClassifiers();

    //uint32_t id;
    //mFilter.addClass(YUV_GOAL_YEL, ID_GOAL_YEL);
    //mFilter.addClass(YUV_BALL, ID_BALL);
    //mFilter.addClass(YUV_FIELD_GREEN, id);

    mFilter.addClass(UV_BALL);
    //mFilter.addClass(YUV_GOAL_YEL);

    mDebugTmr.start(1000);
  }

  int VisionFilter::apply(image_t in)
  {
    mBlobBuffer.clear();
    vision::parse_blobs(in, mFilter, mBlobBuffer);

    mBlobs.clear();
    for(auto it = mBlobBuffer.begin(); it != mBlobBuffer.end(); it++) {
      if(it->rect.getArea() < BLOB_MIN_AREA) continue;
      if(it->getDensity() < BLOB_MIN_DENSITY) continue;
      if(it->getDensity() > BLOB_MAX_DENSITY) continue;


      mBlobs.push_back(*it);
    }

    if(mDebugTmr.isTime()) {
      printf("%lu\n", mBlobs.size());

      for(auto it = mBlobs.begin(); it != mBlobs.end(); it++) {
        printf("d:%.2f\n", it->getDensity());
      }

      mDebugTmr.start(1000);
    }
    return 0;
  }

  size_t to_img_offset(size_t x, size_t y, size_t stride) {
    return ((y * stride) + x) * 3;
  }

  void draw_point(size_t x, size_t y, uint8_t* dat, size_t stride) {
    size_t os = to_img_offset(x, y, stride);
    dat[os] = 255;
    dat[os + 1] = 0;
    dat[os + 2] = 0;
  }

  enum Colors {
    RED,
    GREEN,
    BLUE,
  };

  void draw_line(image_t out, CMV::rect_t r, Colors c = Colors::RED) {
    uint8_t* dat = (uint8_t*)out->data;
    size_t os, W = out->frm.width;

    double y;
    double dy = (r.y1 - r.y0), dx = (r.x1 - r.x0);

    size_t len = (size_t)sqrt(dy*dy + dx*dx);

    for(double z = 0; z < len; z++) {
      double p = z / len;
      os = to_img_offset(r.x0 + (size_t)(p * dx), r.y0 + (size_t)(p * dy), W);

      switch(c) {
        case RED:
          dat[os] = 255;
          dat[os + 1] = 0;
          dat[os + 2] = 0;
          break;
        case GREEN:
          dat[os] = 0;
          dat[os + 1] = 255;
          dat[os + 2] = 0;
          break;
      }

    }
  }

  void draw_rect(image_t out, CMV::rect_t r) {
    uint8_t* dat = (uint8_t*)out->data;

    Colors c = Colors::RED;

    if(r.getArea() > 100) {
      c = Colors::GREEN;
    }

    draw_line(out, {r.x0, r.y0, r.x1, r.y0}, c);
    draw_line(out, {r.x0, r.y1, r.x1, r.y1}, c);

    draw_line(out, {r.x0, r.y0, r.x0, r.y1}, c);
    draw_line(out, {r.x1, r.y0, r.x1, r.y1}, c);
  }

  void VisionFilter::addBlobDebugLayer(image_t out)
  {

    CMV::rect_t r;
    r.x0 = 100;
    r.x1 = 200;

    r.y0 = 100;
    r.y1 = 200;
    size_t lim = 0;

    //draw_rect(out, r);

    for(auto it = mBlobs.begin(); it < mBlobs.end(); it++) {
      draw_rect(out, it->rect);

      /*
      if(it->rect.getArea() > 200) {
        for(auto rl_it = it->rls.begin(); rl_it != it->rls.end(); rl_it++) {
          draw_line(out, {rl_it->x0, rl_it->y, rl_it->x1, rl_it->y}, Colors::BLUE);
        }
      }*/

      if(lim < 4) {
        //printf("%lu, %lu, %lu, %lu (%lu)\n", it->rect.x0, it->rect.y0, it->rect.x1, it->rect.y1, lim);
      }

      lim++;
    }
  }

}
