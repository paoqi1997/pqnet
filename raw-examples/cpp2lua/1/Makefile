CXX = g++
CXXFLAGS = -g -Wall
LIBS = -llua -ldl

BINS = cpp2lua1

.PHONY: build_all

build_all: ${BINS}

cpp2lua1: main.o
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

.PHONY: clean

clean:
	rm ${BINS} *.o
