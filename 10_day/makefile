
libmath.so:myadd.o mysub.o
	g++ -shared -o $@ $<
myadd.o:myadd.cc
	g++ -fPIC -c $<
mysub.o:mysub.cc
	g++ -fPIC -c $<
.PHONY:out
out:
	mkdir -p mylib/include
	mkdir -p mylib/lib
	cp *.h mylib/include
	cp *.so mylib/lib
.PHONY:clean
clean:
	rm -rf *.o mylib *.so
