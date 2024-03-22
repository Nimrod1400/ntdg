#define TB_IMPL
#include "termbox2.h"
#include <stdio.h>

#define MIN_WIDTH  200
#define MIN_HEIGHT 60

typedef struct {
	float x;
	float y;
} vector2;

typedef struct {
	struct vector2 pos;
	struct vector2 size;
} rectangle;

typedef struct {
	unsigned byte color;
	char symbol;
} pixel;

typedef struct {
  int w, h;
	pixel *pixels;
} screen;

void draw_screen(*struct screen scr) {
  for (int x = 0; x < scr->width; ++x) {
		for (int y = 0; y < scr->height; ++y) {
			tb_printf(x, y, scr->pixels[y * scr->width + x].color, 0, scr->pixels[y * scr->width + x].symbol);
		}
  }

	tb_present(); 
}

int main(void) {
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

	tb_shutdown();

	return 0;
}
