-include config.mk

PREFIX ?= /usr/local
INSTALLDIR ?= ${PREFIX}

all:
	cd inotifywatchdog && make PREFIX="${PREFIX}"
	cd mpdconsume && make PREFIX="${PREFIX}"
	cd mtgpmk && make PREFIX="${PREFIX}"
	cd open && make PREFIX="${PREFIX}"
	cd scripts && make PREFIX="${PREFIX}"
	cd ukodus && make PREFIX="${PREFIX}"
	cd xor && make PREFIX="${PREFIX}"

install:
	cd inotifywatchdog && make INSTALLDIR="${INSTALLDIR}" install
	cd mpdconsume && make INSTALLDIR="${INSTALLDIR}" install
	cd mtgpmk && make INSTALLDIR="${INSTALLDIR}" install
	cd open && make INSTALLDIR="${INSTALLDIR}" install
	cd scripts && make INSTALLDIR="${INSTALLDIR}" install
	cd ukodus && make INSTALLDIR="${INSTALLDIR}" install
	cd xor && make INSTALLDIR="${INSTALLDIR}" install
