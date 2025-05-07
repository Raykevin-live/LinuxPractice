.PHONY:all
all: processa processb

processa:processa.cc
	g++ -o $@ $^ -std=c++11
processb:processb.cc
	g++ -o $@ $^ -std=c++11

.PHONY:clean
clean:
	rm -r processa processb