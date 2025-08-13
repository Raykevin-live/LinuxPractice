.PHONY:all
all:servercal clientcal

FLAG=#-DMyself=1 
LIB=-ljsoncpp
servercal:ServerCal.cc
	g++ -o $@ $^ -std=c++11 $(LIB) $(FLAG)
clientcal:ClientCal.cc
	g++ -o $@ $^ -std=c++11 $(LIB) $(FLAG)

.PHONY:clean 
clean:
	rm -f clientcal servercal
