CXX = g++
CXXFLAGS := -g -Wall
CXXFLAGS += -I/usr/include/python3.6m
LIBS = -lpython3.6m

BINS = py2cpp

.PHONY: build_all

build_all: ${BINS}

${BINS}: main.o
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

.PHONY: clean

clean:
	rm ${BINS} *.o
