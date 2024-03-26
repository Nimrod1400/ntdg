#define TB_IMPL
#include "termbox2.h"

#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#define MIN_WIDTH  300
#define MIN_HEIGHT 90
#define BARS_CHAR  0
#define BARS_FG    0
#define BARS_BG    TB_YELLOW 

typedef uintattr_t pixel;

typedef struct {
  int swidth, sheight; // actual terminal sizes
  int rwidth, rheight; // rendered part of the terminal
  int mleft, mright, mtop, mbottom; // margins
  pixel *pixels; // array of pixels
} screen;

void screen_init(screen *scr) {
  int w = tb_width();
  int h = tb_height(); 

  assert(w > 0);
  assert(h > 0);

  scr->swidth = w;
  scr->sheight = h;

  scr->rwidth = MIN_WIDTH;
  scr->rheight = MIN_HEIGHT;

  int ml, mr, mt, mb;

  if ((w - MIN_WIDTH) % 2 == 0) {
    ml = (w - MIN_WIDTH) / 2; 
    mr = ml;
  }
  else {
    ml = (w - MIN_WIDTH - 1) / 2;
    mr = ml + 1;
  }

  if ((h - MIN_HEIGHT) % 2 == 0) {
    mt = (h - MIN_HEIGHT) / 2;
    mb = mt;
  }
  else {
    mt = (h - MIN_HEIGHT + 1) / 2;
    mb = mt - 1; 
  }

  scr->mleft = ml;
  scr->mright = mr;
  scr->mtop = mt;
  scr->mbottom = mb;
  int len = scr->rwidth * scr->rheight;
  scr->pixels = malloc(len * sizeof(pixel));
}

void screen_draw_bars(const screen *scr) {
  // Left margin
  for (int x = 0; x < scr->mleft; ++x) {
    for (int y = 0; y < scr->sheight; ++y) {
      tb_set_cell(x, y, BARS_CHAR, BARS_FG, BARS_BG);
    }
  }

  // Top margin
  for (int x = 0; x < scr->swidth; ++x) {
    for (int y = 0; y < scr->mtop; ++y) {
      tb_set_cell(x, y, BARS_CHAR, BARS_FG, BARS_BG);
    }
  }

  // Right margin
  for (int x = scr->swidth - scr->mright; x < scr->swidth; ++x) {
    for (int y = 0; y < scr->sheight; ++y) {
      tb_set_cell(x, y, BARS_CHAR, BARS_FG, BARS_BG);
    }
  }

  // Bottom margin
  for (int x = 0; x < scr->swidth; ++x) {
    for (int y = scr->sheight - scr->mbottom; y < scr->sheight; ++y) {
      tb_set_cell(x, y, BARS_CHAR, BARS_FG, BARS_BG);
    }
  }

  tb_present();
}

void screen_draw_content(const screen *scr) {
  int rw = scr->rwidth;
  for (int x = scr->mleft; x < scr->swidth - scr->mright; ++x) {
    for (int y = scr->mtop; y < scr->sheight - scr->mbottom; ++y) {
      pixel p = scr->pixels[y * rw + x];
      tb_set_cell(x, y, 0, 0, p);
    }
  }

  tb_present(); 
}

void screen_fill_content(screen *scr, pixel p) {
  int rw = scr->rwidth;
  int rh = scr->rheight;

  for (int i = 0; i < rw * rh; ++i) {
    scr->pixels[i] = p;
  }
}

int main(void) {
  struct tb_event ev; 

  tb_init();

  tb_set_output_mode(TB_OUTPUT_TRUECOLOR);

  int w = tb_width();
  int h = tb_height(); 

  if (w < MIN_WIDTH ||
      h < MIN_HEIGHT) {
    tb_shutdown();
    fprintf(stderr, 
            "Error: terminal is too small! Required %dx%d, found %dx%d.",
            MIN_WIDTH, MIN_HEIGHT, w, h);
    exit(-1);
  }

  screen scr;
  screen_init(&scr);

  screen_fill_content(&scr, 0x12);

  screen_draw_bars(&scr);
  screen_draw_content(&scr);

  tb_poll_event(&ev);
  tb_shutdown();

  return 0;
}
