AM_CFLAGS = -Wall -Werror -Wpedantic -Wno-unused-result -DLOCALEDIR='"$(localedir)"'
bin_PROGRAMS=guessing_game
pkgdata_DATA=guess.mo
pkgdatadir = $(localedir)/ru/LC_MESSAGES

.PHONY: allclean
allclean:
	git clean -fd

guess.mo:
	xgettext -c -k_ -o guess.pot guessing_game.c
	msgmerge ru.po guess.pot -o ru.po
	vim ru.po
	msgfmt ru.po -o guess.mo
