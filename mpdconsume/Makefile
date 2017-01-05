-include config.mk

PREFIX ?= /usr/local

all:
	gcc -std=c99 -W -Wall -Wextra -pedantic -Wcast-align -Wcast-qual\
	    -Wconversion -Wwrite-strings -Wfloat-equal -Wlogical-op -Wpointer-arith\
	    -Wformat=2 -Winit-self -Wuninitialized -Wmaybe-uninitialized -Wshadow\
	    -Wstrict-prototypes -Wmissing-declarations -Wmissing-prototypes -Werror\
	    mpdconsume.c -lmpdclient -o mpdconsume

install:
	install -D -m 755 mpdconsume ${PREFIX}/bin/mpdconsume
