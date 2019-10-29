CXX = g++
CXXFLAGS = -g -Wall
LIBS = -pthread

BINS = chrono_test future_test

.PHONY: build_all

build_all: ${BINS}

chrono_test: chrono_test.o
	${CXX} ${CXXFLAGS} $^ -o $@

future_test: future_test.o
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

.PHONY: clean

clean:
	rm ${BINS} *.o
