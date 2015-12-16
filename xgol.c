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
	int xfd;
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
static void init_signals(void);
static void init_x(void);
static void init_world(void);
static void xevent(void);
static void block(struct timeval interrupt);
static void run(void);
static void term_world(void);
static void term_x(void);

static size_t
count_neighbours(int unsigned cx, int unsigned cy)
{
	size_t neighbours = 0;
	int x, y;

	for (x = (int signed) cx - 1; x <= (int signed) cx + 1; ++x) {
		for (y = (int signed) cy - 1; y <= (int signed) cy + 1; ++y) {
			if ((x != (int signed) cx || y != (int signed) cy)
			&& x >= 0 && x < (int signed) world.w
			&& y >= 0 && y < (int signed) world.h)
				neighbours += world.cur[x][y];
		}
	}
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
init_signals(void)
{
	/* TODO */
}

static void
init_x(void)
{
	XGCValues xgcv;

	/* basic X */
	xgol.dpy = XOpenDisplay(NULL);
	if (xgol.dpy == NULL)
		FATAL("could not open X");
	xgol.xfd = ConnectionNumber(xgol.dpy);
	xgol.screen = DefaultScreen(xgol.dpy);
	xgol.root = RootWindow(xgol.dpy, xgol.screen);
	xgol.depth = DefaultDepth(xgol.dpy, xgol.screen);

	/* pixmap */
	monitor.w = (int unsigned) DisplayWidth(xgol.dpy, xgol.screen);
	monitor.h = (int unsigned) DisplayHeight(xgol.dpy, xgol.screen);
	xgol.pm = XCreatePixmap(xgol.dpy, xgol.root, monitor.w, monitor.h,
	                        (int unsigned) xgol.depth);

	/* graphics context */
	xgcv.graphics_exposures = False;
	xgol.gc = XCreateGC(xgol.dpy, xgol.root, GCGraphicsExposures, &xgcv);

	/* events */
	//XSelectInput(xgol.dpy, xgol.root, ButtonPressMask);
}

static void
init_world(void)
{
	int unsigned x, y;

	world.w = monitor.w / SIZE;
	world.h = monitor.h / SIZE;

	/* allocate */
	world.cur = calloc(world.w, sizeof(bool *));
	world.next = calloc(world.w, sizeof(bool *));
	for (x = 0; x < world.w; ++x) {
		world.cur[x] = calloc(world.h, sizeof(bool));
		world.next[x] = calloc(world.h, sizeof(bool));
	}

	/* populate */
	srand((unsigned int) time(NULL));
	for (x = 0; x < world.w; ++x)
		for (y = 0; y < world.h; ++y)
			world.cur[x][y] = rand() % 2;
}

static void
xevent(void)
{
	XEvent xev;

	if (!XCheckMaskEvent(xgol.dpy, ExposureMask|ButtonPressMask, &xev)) {
		fprintf(stderr, "unmaked X event\n");
		return;
	}

	switch (xev.type) {
	case Expose:
		/* TODO */
		printf("ExposeEvent!\n");
		break;
	case ButtonPressMask:
		/* TODO */
		printf("ButtonPressEvent!\n");
		break;
	default:
		fprintf(stderr, "masked but unhandled X event: %d", xev.type);
	}
}

static void
block(struct timeval interval)
{
	fd_set fds;
	int s;

	do {
		FD_ZERO(&fds);
		FD_SET(xgol.xfd, &fds);
		s = select(FD_SETSIZE, &fds, NULL, NULL, &interval);
		if (s <= 0)
			break;
		if (FD_ISSET(xgol.xfd, &fds))
			xevent();
	} while (interval.tv_usec > 0);
}

static void
run(void)
{
	struct timeval interval = {
		.tv_sec = 0,
		.tv_usec = 100000
	};

	while (true) {
		step_generation();
		draw_world();
		block(interval);
	}
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
	init_signals();
	init_x();
	init_world();
	run();
	term_world();
	term_x();

	return EXIT_SUCCESS;
}
