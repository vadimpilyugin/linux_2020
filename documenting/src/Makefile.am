$(info COND_GCOV is $(COND_GCOV))

if COND_GCOV
CFLAGS += -fprofile-arcs -ftest-coverage -O0 -g
else
CFLAGS += -fprofile-arcs -ftest-coverage -O0 -g -Wno-unused
endif



lib_LTLIBRARIES = libbuf.la

gcov:
	gcov .libs/libbuf
