CXX = g++
CXXFLAGS = -g -Wall
LIBS = -llua

BINS = lua2cpp

.PHONY: build_all

build_all: ${BINS}

lua2cpp: main.o
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

.PHONY: clean

clean:
	rm ${BINS} *.o
