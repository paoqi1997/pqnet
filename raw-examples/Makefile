CXX = g++
CXXFLAGS = -g -Wall

BINS = timerfd_test

.PHONY: build_all

build_all: ${BINS}

timerfd_test: timerfd_test.o
	${CXX} ${CXXFLAGS} $^ -o $@

.PHONY: clean

clean:
	rm ${BINS} *.o
