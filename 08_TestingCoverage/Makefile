libbuf.so: buf.h buf.c
	cc -o libbuf.so -fPIC -shared -O0 -g buf.c
tests: tests.c libbuf.so
	cc -o tests -fPIC -O0 -g -L. tests.c -lbuf
.PHONY: test
test: tests
	./tests