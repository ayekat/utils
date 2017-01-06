-include config.mk

PREFIX ?= /usr/local
INSTALLDIR ?= ${PREFIX}

SCRIPTS = aacfy abs ansicolours aur ayeinput bleep ccols checkrestart \
          clawsmail_notify cmdcharts demote dotbloat dpass gnuprint guitarrig \
          ix keyboard lcdadjust pacbloat pacwhy pdfcat photo_import \
          pkgbuilddep prtscr random rdp scr scrlock sprunge sum tmx vidcat vpn \
          wttr xvim

all:
	# Nothing to be done

install:
	mkdir -p ${INSTALLDIR}/bin
	@for script in ${SCRIPTS}; do \
		install -vm 755 $$script ${INSTALLDIR}/bin/$$script; \
	done
	ln -s scr ${INSTALLDIR}/bin/dp
	ln -s scr ${INSTALLDIR}/bin/hdmi
	ln -s scr ${INSTALLDIR}/bin/vga
	ln -s sum ${INSTALLDIR}/bin/avg
