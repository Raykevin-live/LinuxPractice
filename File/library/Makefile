dy-lib=libmethod.so
static-lib=libmymath.a

.PHONY:all
all: $(dy-lib) $(static-lib)

$(dy-lib):mylog.o myprint.o
	gcc -o $@ $^ -shared
mylog.o:mylog.c
	gcc -fPIC -c $^
myprint.o:myprint.c
	gcc -fPIC -c $^

$(static-lib):mymath.o
	ar -rc $@ $^
mymath.o:mymath.c
	gcc -c $^


.PHONY:clean
clean:
	rm -rf *.a *.o mylib *.so
.PHONY:output
output:
	mkdir -p mylib/include
	mkdir -p mylib/lib
	cp *.h mylib/include
	cp *.a mylib/lib
	cp *.so mylib/lib

