.PHONY:all
all:myother mycommand
mycommand:mycommand.c
	gcc -o mycommand mycommand.c -std=c99
myother:otherExe.cpp
	g++ -o $@ $^ -std=c++11

	
.PHONY:clean
clean:
	rm -f mycommand myother
.PHONY:run
run:
	./mycommand

