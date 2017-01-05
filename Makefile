-include config.mk

PREFIX ?= /usr/local
INSTALLDIR ?= ${PREFIX}

all:
	cd mpdconsume && PREFIX="${PREFIX}" make
	cd mtgpmk && PREFIX="${PREFIX}" make
	cd open && PREFIX="${PREFIX}" make
	cd ukodus && PREFIX="${PREFIX}" make
	cd xor && PREFIX="${PREFIX}" make

install:
	cd mpdconsume && INSTALLDIR="${INSTALLDIR}" make install
	cd mtgpmk && INSTALLDIR="${INSTALLDIR}" make install
	cd open && INSTALLDIR="${INSTALLDIR}" make install
	cd ukodus && INSTALLDIR="${INSTALLDIR}" make install
	cd xor && INSTALLDIR="${INSTALLDIR}" make install
