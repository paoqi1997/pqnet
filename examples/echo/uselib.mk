CXX = g++
CXXFLAGS = -g -Wall -I../.. -L../../pqnet
LIBS = -lpqnet -pthread

BINS = echoclient echoserver

.PHONY: build_all

build_all: ${BINS}

echoclient: echoclient.o
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

echoserver: echoserver.o
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

.PHONY: clean

clean:
	rm ${BINS} *.o
