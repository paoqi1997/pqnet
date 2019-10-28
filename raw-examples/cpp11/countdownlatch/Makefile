CXX = g++
CXXFLAGS = -g -Wall
LIBS = -pthread

BINS = app

.PHONY: build_all

build_all: ${BINS}

app: main.o
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

.PHONY: clean

clean:
	rm ${BINS} *.o
