-include config.mk

PREFIX ?= /usr/local
INSTALLDIR ?= ${PREFIX}

all:
	# Nothing to do

install:
	install -D -m 755 inotifywatchdog ${INSTALLDIR}/bin/inotifywatchdog
	install -D -m 644 inotifywatchdog.service ${INSTALLDIR}/share/doc/inotifywatchdog/inotifywatchdog.service
