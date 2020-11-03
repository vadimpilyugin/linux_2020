wrapper.so: wrapper.c
	cc -o wrapper.so -shared -fPIC wrapper.c -ldl
.PHONY: clean
clean:
	git clean -fd
.PHONY: test
test: wrapper.so
	touch fooFIX foo
	ls
	rm foo fooFIX
	echo 'Deleted both'
	ls
	touch fooFIX foo
	ls
	-LD_PRELOAD=wrapper.so rm foo fooFIX
	echo 'Deleted only one'
	ls
	rm fooFIX
