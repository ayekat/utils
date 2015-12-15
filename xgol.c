#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define FATAL(...) { fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE); }
#define MAX(A, B) ((A) < (B) ? (B) : (A))
#define MIN(A, B) ((A) > (B) ? (B) : (A))

#define SIZE 10
#define DEAD false
#define ALIVE true

struct {
	Display *dpy;
	int screen;
	Window root;
	Pixmap pm;
	int depth;
	GC gc;

} xgol;

struct {
	int unsigned w, h;
} monitor;

struct {
	int unsigned w, h;
	bool **cur, **next;
} world;

static size_t count_neighbours(int unsigned cx, int unsigned cy);
static void step_generation(void);
static void draw_world(void);
static void init_x(void);
static void init_world(void);
static void term_x(void);
static void term_world(void);

static size_t
count_neighbours(int unsigned cx, int unsigned cy)
{
	size_t neighbours = 0;
	int x, y;

	for (x = (int signed) cx - 1; x <= (int signed) cx + 1; ++x)
		for (y = (int signed) cy - 1; y <= (int signed) cy + 1; ++y)
			if ((x != cx || y != cy)
			&& x >= 0 && x < world.w && y >= 0 && y < world.h)
				neighbours += world.cur[x][y];
	return neighbours;
}

static void
step_generation(void)
{
	int unsigned x, y;
	size_t neighbours;
	bool **tmp;

	for (x = 0; x < world.w; ++x) {
		for (y = 0; y < world.h; ++y) {
			neighbours = count_neighbours(x, y);
			if (neighbours == 3)
				world.next[x][y] = ALIVE;
			else if (neighbours == 2)
				world.next[x][y] = world.cur[x][y];
			else
				world.next[x][y] = DEAD;
		}
	}
	tmp = world.cur;
	world.cur = world.next;
	world.next = tmp;
}

static void
draw_world(void)
{
	int unsigned x, y;

	XSetForeground(xgol.dpy, xgol.gc, 0x123456);
	XFillRectangle(xgol.dpy, xgol.pm, xgol.gc, 0, 0, monitor.w, monitor.h);
	for (x = 0; x < world.w; ++x) {
		for (y = 0; y < world.h; ++y) {
			if (!world.cur[x][y])
				continue;
			XSetForeground(xgol.dpy, xgol.gc, 0x345678);
			XFillRectangle(xgol.dpy, xgol.pm, xgol.gc,
			               (int signed) x*SIZE, (int signed) y*SIZE,
			               SIZE, SIZE);
		}
	}
	XCopyArea(xgol.dpy, xgol.pm, xgol.root, xgol.gc,
	          0, 0, monitor.w, monitor.h, 0, 0);
	XSync(xgol.dpy, xgol.screen);
}

static void
init_x(void)
{
	XGCValues xgcv;

	xgol.dpy = XOpenDisplay(NULL);
	if (xgol.dpy == NULL)
		FATAL("could not open X");
	xgol.screen = DefaultScreen(xgol.dpy);
	monitor.w = (int unsigned) DisplayWidth(xgol.dpy, xgol.screen);
	monitor.h = (int unsigned) DisplayHeight(xgol.dpy, xgol.screen);
	world.w = monitor.w / SIZE;
	world.h = monitor.h / SIZE;
	xgol.root = RootWindow(xgol.dpy, xgol.screen);
	xgol.depth = DefaultDepth(xgol.dpy, xgol.screen);
	xgol.pm = XCreatePixmap(xgol.dpy, xgol.root, monitor.w, monitor.h,
	                        (int unsigned) xgol.depth);
	xgcv.graphics_exposures = False;
	xgol.gc = XCreateGC(xgol.dpy, xgol.root, GCGraphicsExposures, &xgcv);
}

static void
init_world(void)
{
	int unsigned x, y;

	world.cur = calloc(world.w, sizeof(bool *));
	world.next = calloc(world.w, sizeof(bool *));
	for (x = 0; x < world.w; ++x) {
		world.cur[x] = calloc(world.h, sizeof(bool));
		world.next[x] = calloc(world.h, sizeof(bool));
	}

	srand((unsigned int) time(NULL));
	for (x = 0; x < world.w; ++x)
		for (y = 0; y < world.h; ++y)
			world.cur[x][y] = rand() % 2;

}

static void
term_x(void)
{
	XFreePixmap(xgol.dpy, xgol.pm);
	XFreeGC(xgol.dpy, xgol.gc);
	XCloseDisplay(xgol.dpy);
}

static void
term_world(void)
{
	int unsigned y;

	for (y = 0; y < world.h; ++y) {
		free(world.cur[y]);
		free(world.next[y]);
	}
	free(world.cur);
	free(world.next);
}

int
main(void)
{
	init_x();
	init_world();

	while (true) {
		step_generation();
		draw_world();
		usleep(100000);
	}

	term_world();
	term_x();

	return EXIT_SUCCESS;
}
