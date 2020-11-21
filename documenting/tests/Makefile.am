TESTS = stest
check_PROGRAMS = stest
CFLAGS = @CHECK_CFLAGS@ -I../src -O0 -g
LDADD = $(top_builddir)/src/libbuf.la @CHECK_LIBS@

.ts.c:
	checkmk $< > $@
