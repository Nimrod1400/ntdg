#define TB_IMPL
#include "termbox2.h"

#include <stdio.h>
#include <stdint.h>

#define MIN_WIDTH  200
#define MIN_HEIGHT 60

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
  int width, height;
	pixel *pixels;
} screen;

void draw_screen(screen *scr) {
  int w = scr->width;
  int h = scr->height;

  for (int x = 0; x < w; ++x) {
		for (int y = 0; y < h; ++y) {
  		pixel p = scr->pixels[y * scr->width + x];
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
