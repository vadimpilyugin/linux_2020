%.o: %.c *.h
	cc -fPIC -c -o $@ $<

liboutput_static.a: fun.o const.o
	ar rcs $@ $^

liboutput.so: fun.o const.o
	cc -fPIC -shared $^ -o $@

prog: fun.o const.o prog.o
	cc -fPIC -o $@ $^

prog-a: prog.o liboutput_static.a
	cc -fPIC -o $@ $^

prog-so: prog.o liboutput.so
	cc -fPIC -o $@ prog.o -Wl,-rpath=$(shell pwd) -L . -loutput

.PHONY: test
test: prog prog-a prog-so
	./prog 2> file11
	./prog 1 2>&1 > file12
	./prog 1 2 3 2>&1 > file13
	./prog-a 2> file21
	./prog-a 1 2>&1 > file22
	./prog-a 1 2 3 2>&1 > file23
	./prog-so 2> file31
	./prog-so 1 2>&1 > file32
	./prog-so 1 2 3 2>&1 > file33
	# cmp file11 file21
	# cmp file21 file31
	cmp file12 file22
	cmp file22 file32
	cmp file13 file23
	cmp file23 file33

.PHONY: clean
clean:
	rm -f *.o *.a *.so prog-* prog file*