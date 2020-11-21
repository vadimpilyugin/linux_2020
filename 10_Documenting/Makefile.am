AM_CFLAGS = -Wall -Werror -Wpedantic -Wno-unused-result
bin_PROGRAMS=guessing_game

@DX_RULES@

.PHONY: allclean
allclean:
	git clean -fd

.PHONY: retranslate
retranslate:
	xgettext -c -k_ -o guess.pot guessing_game.c
	msgmerge ru.po guess.pot -o ru.po
	vim ru.po
	mkdir -p ru_RU/LC_MESSAGES
	msgfmt ru.po -o ru_RU/LC_MESSAGES/guess.mo
