# Compiler:
CC ?= gcc

# Installation prefix:
PREFIX ?= /usr/local

# Application name:
APPNAME = xgol

# Compilation flags:
XGOL_CFLAGS = -std=gnu99
XGOL_CFLAGS += -W -Wall -Wextra -pedantic
XGOL_CFLAGS += -Wcast-align -Wcast-qual -Wconversion -Wwrite-strings -Wfloat-equal
XGOL_CFLAGS += -Wlogical-op -Wpointer-arith -Wformat=2
XGOL_CFLAGS += -Winit-self -Wuninitialized -Wmaybe-uninitialized
XGOL_CFLAGS += -Wstrict-prototypes -Wmissing-declarations -Wmissing-prototypes
XGOL_CFLAGS += -Wshadow #-Wpadded
XGOL_CFLAGS += $(shell pkg-config --cflags x11)

XGOL_CFLAGS_ASAN = -fsanitize=address -fno-omit-frame-pointer
XGOL_CFLAGS_DEBUG = -Werror -g -O1 -DMODE_DEBUG
XGOL_CFLAGS_RELEASE = -O2
XGOL_CFLAGS_XINERAMA = $(shell pkg-config --cflags xinerama) -DXINERAMA

# Libraries:
XGOL_LIBS = $(shell pkg-config --libs x11)
XGOL_LIBS_ASAN =
XGOL_LIBS_DEBUG =
XGOL_LIBS_RELEASE =
XGOL_LIBS_XINERAMA = $(shell pkg-config --libs xinerama)

# Linker flags:
XGOL_LDFLAGS =
XGOL_LDFLAGS_ASAN = -fsanitize=address
XGOL_LDFLAGS_DEBUG =
XGOL_LDFLAGS_RELEASE =
XGOL_LDFLAGS_XINERAMA =

# File names:
SRCDIR = .
BUILDDIR = build
SOURCES = $(wildcard ${SRCDIR}/*.c ${SRCDIR}/layouts/*.c)
OBJECTS = $(SOURCES:${SRCDIR}/%.c=${BUILDDIR}/%.o)
DEPENDS = $(OBJECTS:%.o=%.d)
XINITRC = xinitrc

-include config.mk

# Default: Release + Xinerama
.PHONY: all
all: release_xinerama

# Release:
.PHONY: release
release: XGOL_CFLAGS += ${XGOL_CFLAGS_RELEASE}
release: XGOL_LIBS += ${XGOL_LIBS_RELEASE}
release: XGOL_LDFLAGS += ${XGOL_LDFLAGS_RELEASE}
release: build

# Release + Xinerama:
.PHONY: release_xinerama
release_xinerama: XGOL_CFLAGS += ${XGOL_CFLAGS_XINERAMA}
release_xinerama: XGOL_LIBS += ${XGOL_LIBS_XINERAMA}
release_xinerama: XGOL_LDFLAGS += ${XGOL_LDFLAGS_XINERAMA}
release_xinerama: release

# Debug:
.PHONY: debug
debug: XGOL_CFLAGS += ${XGOL_CFLAGS_DEBUG}
debug: XGOL_LIBS += ${XGOL_LIBS_DEBUG}
debug: XGOL_LDFLAGS += ${XGOL_LDFLAGS_DEBUG}
debug: build

# Debug + Xinerama:
.PHONY: debug_xinerama
debug_xinerama: XGOL_CFLAGS += ${XGOL_CFLAGS_XINERAMA}
debug_xinerama: XGOL_LIBS += ${XGOL_LIBS_XINERAMA}
debug_xinerama: XGOL_LDFLAGS += ${XGOL_LDFLAGS_XINERAMA}
debug_xinerama: debug

# Address Sanitizer:
.PHONY: asan
asan: XGOL_CFLAGS += ${XGOL_CFLAGS_ASAN}
asan: XGOL_LIBS += ${XGOL_LIBS_ASAN}
asan: XGOL_LDFLAGS += ${XGOL_LDFLAGS_ASAN}
asan: debug

# Address Sanitizer + Xinerama:
.PHONY: asan_xinerama
asan_xinerama: XGOL_CFLAGS += ${XGOL_CFLAGS_XINERAMA}
asan_xinerama: XGOL_LIBS += ${XGOL_LIBS_XINERAMA}
asan_xinerama: XGOL_LDFLAGS += ${XGOL_LDFLAGS_XINERAMA}
asan_xinerama: asan

# Basic actions:
.PHONY: build run clean mrproper install uninstall
build: $(APPNAME)
run:
	./${APPNAME}
clean:
	rm -rf ${BUILDDIR}
mrproper: clean
	rm -f ${APPNAME}
install:
	install ${APPNAME} ${PREFIX}/bin/${APPNAME}
uninstall:
	rm -f ${PREFIX}/bin/${APPNAME}

# Build dependencies:
-include ${DEPENDS}

# Compile:
$(BUILDDIR)/%.o: XGOL_CFLAGS += ${CFLAGS}
$(BUILDDIR)/%.o: ${SRCDIR}/%.c
	@printf "compiling \033[1m%s\033[0m ...\n" $@
	mkdir -p "$(shell dirname $@)"
	$(CC) ${XGOL_CFLAGS} -c $< -o $@
	$(CC) ${XGOL_CFLAGS} -MM -MT $@ $< > ${BUILDDIR}/$*.d

# Link:
$(APPNAME): XGOL_LIBS += ${LIBS}
$(APPNAME): XGOL_LDFLAGS += ${LDFLAGS}
$(APPNAME): $(OBJECTS)
	@printf "linking \033[1m%s\033[0m ...\n" $@
	$(CC) ${XGOL_LDFLAGS} ${OBJECTS} ${XGOL_LIBS} -o $@
