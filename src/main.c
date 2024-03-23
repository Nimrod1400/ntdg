#define TB_IMPL
#include "termbox2.h"

#include <stdio.h>
#include <stdint.h>

#define MIN_WIDTH  200
#define MIN_HEIGHT 60

#define BARS_CHAR  0
#define BARS_COLOR 0

typedef struct {
	float x;
	float y;
} vector2;

typedef struct {
	vector2 pos;
	vector2 size;
} rectangle;

typedef struct {
	uintattr_t color;
	uint32_t symbol;
} pixel;

typedef struct {
  int swidth, sheight;
  int rwidth, rheight;
  int mleft, mright, mtop, mbottom;
	pixel *pixels;
} screen;

void screen_init(screen *scr) {
	int w = tb_width();
	int h = tb_height(); 

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
		mr = (w - MIN_WIDTH + 1) / 2;
	}

	if ((h - MIN_HEIGHT) % 2 == 0) {
		mt = (h - MIN_HEIGHT) / 2;
		mb = mt;
	}
	else {
		mt = (h - MIN_HEIGHT + 1) / 2;
		mb = (h - MIN_HEIGHT - 1) / 2; 
	}

	scr->mleft = ml;
	scr->mright = mr;
	scr->mtop = mr;
	scr->mbottom = mb;

	scr->pixels = calloc(MIN_WIDTH * MIN_HEIGHT, sizeof(pixel));
}

void screen_draw(screen *scr) {
  int rw = scr->rwidth;
  int rh = scr->rheight;

  for (int x = scr->mleft + 1; x < rw; ++x) {
		for (int y = scr->mtop + 1; y < rh; ++y) {
  		pixel p = scr->pixels[y * rw + x];
  		tb_set_cell(x, y, p.symbol, p.color, 0);
		}
  }

  for (int x = 0; x < scr->mleft; ++x) {
		for (int y = scr->mtop + 1; y < rh; ++y) {
  		pixel p = scr->pixels[y * rw + x];
  		tb_set_cell(x, y, p.symbol, p.color, 0);
		}
  }

	tb_present(); 
}

int main(void) {
  struct tb_event ev; 
	tb_init();

	int w = tb_width();
	int h = tb_height(); 

	if (w < MIN_WIDTH ||
	    h < MIN_HEIGHT) {
		tb_shutdown();
		fprintf(stderr, "Error: terminal is too small! Required %dx%d, found %dx%d.",
						MIN_WIDTH, MIN_HEIGHT, w, h);
		exit(-1);
	}

	// uint32_t rect = 9646;

	pixel *ps = malloc(w * h * sizeof(pixel));
	for (int i = 0; i < w * h; ++i) {
		ps[i].color = TB_BLUE;
		ps[i].symbol = '+';
	}

	screen scr = { w, h, ps, };

	draw_screen(&scr);
	
	tb_present();

	tb_poll_event(&ev);
	tb_shutdown();

	return 0;
}
