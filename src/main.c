#define TB_IMPL
#include "termbox2.h"

#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#define MIN_WIDTH  300
#define MIN_HEIGHT 90
#define BARS_CHAR  0
#define BARS_FG    0
#define BARS_BG    0x101030

typedef uintattr_t pixel;

typedef struct {
  int swidth, sheight; // actual terminal sizes
  int rwidth, rheight; // rendered part of the terminal
  int mleft, mright, mtop, mbottom; // margins
  pixel *pixels; // array of pixels
} screen;

static struct tb_event ev = { 0 };
static screen scr = { 0 };

void screen_init() {
  int w = tb_width();
  int h = tb_height(); 

  scr.swidth = w;
  scr.sheight = h;

  scr.rwidth = MIN_WIDTH;
  scr.rheight = MIN_HEIGHT;

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

  scr.mleft = ml;
  scr.mright = mr;
  scr.mtop = mt;
  scr.mbottom = mb;

  int len = scr.rwidth * scr.rheight;
  scr.pixels = malloc(len * sizeof(pixel));
}

void screen_draw_bars() {
  // Left margin bar
  for (int y = 0; y < scr.sheight; ++y) {
    for (int x = 0; x < scr.mleft; ++x) {
      tb_set_cell(x, y, BARS_CHAR, BARS_FG, BARS_BG);
    }
  }

  // Top margin bar
  for (int y = 0; y < scr.mtop; ++y) {
    for (int x = 0; x < scr.swidth; ++x) {
      tb_set_cell(x, y, BARS_CHAR, BARS_FG, BARS_BG);
    }
  }

  // Right margin bar
  for (int y = 0; y < scr.sheight; ++y) {
    for (int x = scr.mright + scr.rwidth; x < scr.swidth; ++x) {
      tb_set_cell(x, y, BARS_CHAR, BARS_FG, BARS_BG);
    }
  }

  // Bottom margin bar
  for (int y = scr.mtop + scr.rheight; y < scr.sheight; ++y) {
    for (int x = 0; x < scr.swidth; ++x) {
      tb_set_cell(x, y, BARS_CHAR, BARS_FG, BARS_BG);
    }
  }

  tb_present();
}

void screen_draw_content() {
  int rw = scr.rwidth;
  int i = 0;

  for (int y = scr.mtop; y < scr.mtop + scr.rheight; ++y) {
    for (int x = scr.mleft; x < scr.mleft + scr.rwidth; ++x) {
      pixel p = scr.pixels[i++];
      tb_set_cell(x, y, 0, 0, p);
    }
  }

  tb_present(); 
}

void screen_fill_content(pixel p) {
  int rw = scr.rwidth;
  int rh = scr.rheight;

  for (int i = 0; i < rw * rh; ++i) {
    scr.pixels[i] = p;
  }
}

// Is using macros for this kind of one-line operations cool?
// Like, instead of function call it will be inline operation, so it will be more efficient?
#define screen_fill_pixel(p, x, y) scr.pixels[(y) * scr.rwidth + (x)] = (p);

void screen_fill_rectangle(pixel p, int x1, int y1, int x2, int y2) {
  for (int y = y1; y < y2; ++y) {
    for (int x = x1; x < x2; ++x) {
      screen_fill_pixel(p, x, y);
    }
  }
}

// Check if terminal is big enough
void validate_term_size() {
  int w = tb_width();
  int h = tb_height(); 

  if (w < MIN_WIDTH ||
      h < MIN_HEIGHT) {
    tb_shutdown();
    fprintf(stderr, 
            "Error: The terminal is too small! Required %dx%d, found %dx%d.",
            MIN_WIDTH, MIN_HEIGHT, w, h);
    exit(-1);
  }
}

void receive_input() {
  tb_poll_event(&ev);
  if (ev.type == TB_EVENT_KEY) {

    if (ev.key == TB_KEY_CTRL_C || ev.key == TB_KEY_ESC) {
      tb_shutdown();
      exit(0);
    }
  }
}

void update_state() {

}

void render() {
  for (int y = 0; y < MIN_HEIGHT; ++y) {
    for (int x = 0; x < MIN_WIDTH; ++x) {
      screen_fill_pixel((x + 1) * (y + 1) * (0xffffff / 27000) % 0xffffff, x, y);
    }
  }

  screen_draw_content();
}

int main(void) {
  struct tb_event ev; 

  tb_init();

  validate_term_size();

  tb_set_output_mode(TB_OUTPUT_TRUECOLOR);

  screen scr;
  screen_init();

  screen_draw_bars();
  render();

  // Main loop
  for (;;) {
    receive_input(&ev);
    update_state();
    render(&scr);
  }

  tb_shutdown();

  return 0;
}
