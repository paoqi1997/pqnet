CXX = g++
CXXFLAGS = -g -Wall
LIBS = -llua -ldl

BINS = cpp2lua

.PHONY: build_all

build_all: ${BINS}

${BINS}: main.o mylua.o
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

.PHONY: clean

clean:
	rm ${BINS} *.o
