#define TB_IMPL
#include "termbox2.h"

#include <stdio.h>

#define MIN_WIDTH  200
#define MIN_HEIGHT 60

struct vector2 {
	float x;
	float y;
};

struct rectangle {
	struct vector2 pos;
	struct vector2 size;
};

struct pixel {
	unsigned byte color;
	char symbol;
};

struct screen {
  int w, h;
	pixel *pixels;
};

void draw_screen(*struct screen scr) {
  for (int x = 0; x < scr->width; ++x) {
		for (int y = 0; y < scr->height; ++y) {
			tb_printf(x, y, scr->pixels[y * scr->width + x].color, 0, scr->pixels[y * scr->width + x].symbol);
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

	tb_printf(0, 0, 0, 0, "█");

	tb_present(); 
	tb_poll_event(&ev);

	tb_shutdown();

	return 0;
}
