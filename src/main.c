#define TB_IMPL
#include "termbox2.h"

#include <assert.h>
#include <stdio.h>
#include <stdint.h>

#define MIN_WIDTH  200
#define MIN_HEIGHT 60

#define BARS_CHAR 0
#define BARS_FG   0
#define BARS_BG   0 

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

#if 0
	assert(w > 0);
	assert(h > 0);
#endif

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
	scr->pixels = calloc(scr->swidth * scr->sheight, sizeof(pixel));
}

void screen_draw_bars(const screen *scr) {
	// Draw left bar
	for (int x = 0; x < scr->mleft; ++x) {
		for (int y = 0; y < scr->sheight; ++y) {
			tb_set_cell(x, y, BARS_CHAR, BARS_FG, BARS_BG);
		}
	}

	// Draw top bar
	for (int x = 0; x < scr->swidth; ++x) {
		for (int y = 0; y < scr->mtop; ++y) {
			tb_set_cell(x, y, BARS_CHAR, BARS_FG, BARS_BG);
		}
	}

	// Draw right bar
	for (int x = scr->swidth - scr->mright; x < scr->swidth; ++x) {
		for (int y = 0; y < scr->sheight; ++y) {
			tb_set_cell(x, y, BARS_CHAR, BARS_FG, BARS_BG);
		}
	}

	// Draw bottom bar
	for (int x = 0; x < scr->swidth; ++x) {
		for (int y = scr->sheight - scr->mbottom; y < scr->sheight; ++y) {
			tb_set_cell(x, y, BARS_CHAR, BARS_FG, BARS_BG);
		}
	}

	tb_present();
}

void screen_draw_content(const screen *scr) {
  int rw = scr->rwidth;
  int rh = scr->rheight;

	for (int x = scr->mleft + 1; x < rw; ++x) {
		for (int y = scr->mtop + 1; y < rh; ++y) {
			pixel p = scr->pixels[y * rw + x];
			tb_set_cell(x, y, p.symbol, p.color, 0);	
		}
	}

	tb_present(); 
}

void screen_fill(screen *scr, pixel *p) {
  int rw = scr->rwidth;
  int rh = scr->rheight;

	for (int i = 0; i < rw * rh; ++i) {
		scr->pixels[i].symbol = p->symbol;
		scr->pixels[i].color = p->color;
	}
}

int main(void) {
  struct tb_event ev; 

	tb_init();

	int w = tb_width();
	int h = tb_height(); 

#if 0
	if (w < MIN_WIDTH ||
	    h < MIN_HEIGHT) {
		tb_shutdown();
		fprintf(stderr, "Error: terminal is too small! Required %dx%d, found %dx%d.",
						MIN_WIDTH, MIN_HEIGHT, w, h);
		exit(-1);
	}
#endif 
	uint32_t r = 9646;
	// TODO: Fix bars size
	screen scr;
	screen_init(&scr);

	pixel p = { TB_BLUE, r, };
	screen_fill(&scr, &p);

	screen_draw_bars(&scr);
	screen_draw_content(&scr);

	tb_poll_event(&ev);
	tb_shutdown();

	return 0;
}
