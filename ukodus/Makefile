# Compiler:
CC ?= gcc

# Application name:
APPNAME = ukodus

# Files:
SRCDIR = src
BUILDDIR = build
MODULEDIR = modules
SOURCES = $(shell find ${SRCDIR} -name '*.c')
OBJECTS = $(SOURCES:${SRCDIR}/%.c=${BUILDDIR}/%.o)
DEPENDS = $(OBJECTS:%.o=%.d)

# Installation:
INSTALLDIR ?= /usr/local
BINDIR ?= ${INSTALLDIR}/bin
INCLUDEDIR ?= ${INSTALLDIR}/include/${APPNAME}

# ==============================================================================

# Compilation flags:
_CFLAGS = -std=c99
_CFLAGS += -W -Wall -Wextra -pedantic
_CFLAGS += -Wcast-align -Wcast-qual -Wconversion -Wwrite-strings -Wfloat-equal
_CFLAGS += -Wlogical-op -Wpointer-arith -Wformat=2
_CFLAGS += -Winit-self -Wuninitialized -Wmaybe-uninitialized -Wshadow
_CFLAGS += -Wstrict-prototypes -Wmissing-declarations -Wmissing-prototypes
#_CFLAGS += -Wpadded
_CFLAGS += -I${HOME}/.local/include

_CFLAGS_ASAN = -fsanitize=address -fno-omit-frame-pointer
_CFLAGS_DEBUG = -Werror -g -O1 -DMODE_DEBUG
_CFLAGS_RELEASE = -O2

# Libraries:
_LIBS =
_LIBS_ASAN =
_LIBS_DEBUG =
_LIBS_RELEASE =

# Linker flags:
_LDFLAGS = -Wl,--export-dynamic
_LDFLAGS_ASAN = -fsanitize=address
_LDFLAGS_DEBUG =
_LDFLAGS_RELEASE =

# ==============================================================================

# Configuration:
-include config.mk

# ==============================================================================

# Default: Release + Xinerama
.PHONY: all
all: release

# Release:
.PHONY: release
release: _CFLAGS += ${_CFLAGS_RELEASE}
release: _LIBS += ${_LIBS_RELEASE}
release: _LDFLAGS += ${_LDFLAGS_RELEASE}
release: $(APPNAME)

# Debug:
.PHONY: debug
debug: _CFLAGS += ${_CFLAGS_DEBUG}
debug: _LIBS += ${_LIBS_DEBUG}
debug: _LDFLAGS += ${_LDFLAGS_DEBUG}
debug: $(APPNAME)

# Address Sanitizer:
.PHONY: asan
asan: _CFLAGS += ${_CFLAGS_ASAN}
asan: _LIBS += ${_LIBS_ASAN}
asan: _LDFLAGS += ${_LDFLAGS_ASAN}
asan: debug

# ==============================================================================

# Build-dependencies:
-include ${DEPENDS}

# Compile:
$(BUILDDIR)/%.o: _CFLAGS += ${CFLAGS}
$(BUILDDIR)/%.o: ${SRCDIR}/%.c
	@printf "compiling \033[1m%s\033[0m ...\n" $@
	mkdir -p "$(shell dirname $@)"
	$(CC) ${_CFLAGS} -c $< -o $@
	$(CC) ${_CFLAGS} -MM -MT $@ $< > ${BUILDDIR}/$*.d

# Link:
$(APPNAME): _LIBS += ${LIBS}
$(APPNAME): _LDFLAGS += ${LDFLAGS}
$(APPNAME): $(OBJECTS)
	@printf "linking \033[1m%s\033[0m ...\n" $@
	$(CC) ${_LDFLAGS} ${OBJECTS} ${_LIBS} -o $@

# ==============================================================================

# Generic actions:
.PHONY: clean mrproper install uninstall
clean:
	rm -rf ${BUILDDIR}
mrproper: clean
	rm -f ${APPNAME}
install:
	install -D ${APPNAME} ${BINDIR}/${APPNAME}
install-headers:
	mkdir -p ${INCLUDEDIR}
	install -m 644 ${SRCDIR}/*.h ${INCLUDEDIR}/
uninstall:
	rm -f ${BINDIR}/${APPNAME}
	rm -rf ${INCLUDEDIR}

# Ctags:
.PHONY: ctags
ctags:
	rm -f tags
	find ${SRCDIR} -name '*.[ch]' | ctags --append -L -
