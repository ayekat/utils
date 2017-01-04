-include config.mk

APPNAME ?= mtgpmk

INSTALLDIR ?= /usr/local
BINDIR ?= ${INSTALLDIR}/bin

all:
	# Nothing to do

install:
	mkdir -p ${BINDIR}
	install -m 755 ${APPNAME} ${BINDIR}/

uninstall:
	rm -f ${BINDIR}/${APPNAME}
