"Magic: the Gathering" Proxy Card Make
======================================

A simple tool for generating PDFs containing *Magic: the Gathering* card images
suited for being printed out and being used as quick and dirty card proxies for
testing purposes.


install
-------

	make install

If you want *mtgpmk* to be installed in a custom location, either pass something
like `INSTALLDIR=~/.local` to it, or create a *config.mk* in the project root
that will be included by the Makefile.


dependencies
------------

*mtgpmk* requires the texlive suite to be installed on your system (providing
the `pdflatex` command), and GNU coreutils.


usage
-----

*mtgpmk* is like `make`, but for instead of Makefiles it uses *Proxyfiles*. A
Proxyfile may look like this:

	"Black Lotus"
	Mox Emerald
	Mox Sapphire
	Mox Jet
	Mox Pearl
	Mox Ruby
	Time Walk
	Ancestral Recall
	Timetwister

In the same directory as the Proxyfile, run

	mtgpmk

and it will fetch the corresponding images from
[magiccards.info](http://magiccards.info/) by scraping the relevant parts of its
website, generate a LaTeX file, and finally compile a PDF containing the images
in the right size to be used as cheap proxies for testing purposes, when printed
out.


caching
-------

In order to reduce network bandwidth, each image and its corresponding ID is
cached in `~/.local/cache/mtgpmk` once it is successfully fetched. Subsequent
uses of the same image will use the local cache.


limitations
-----------

There are a few minor limitations, which may be fixed if there are people who
actually use it and might find it useful:

* The card edition cannot be chosen; it automatically fetches the latest
  edition.
* The amount of copies cannot be specified; copy-pasting is the name of the
  game.
* Due to quite inelegantly parsing the raw HTML, in case of a format change,
  things may horribly break.


copyright & thanks
------------------

*Magic: the Gathering* is a trademark and copyrighted by *Wizards of the Coast,
Inc*, a subsidiary of *Hasbro, Inc*. All rights reserved. This project is
unaffiliated.

At this place I would like to thank WotC for creating this game, and guys from
magiccards.info for generating the card Database.
