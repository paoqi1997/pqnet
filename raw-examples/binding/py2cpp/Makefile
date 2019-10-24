CXX = g++
CXXFLAGS = -g -Wall
INCS = -I/usr/include/python3.6
LIBS = -lpython3.6m

CXXFLAGS += ${INCS}

BINS = py2cpp

.PHONY: build_all

build_all: ${BINS}

${BINS}: main.o
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

.PHONY: clean

clean:
	rm ${BINS} *.o
