CXX = g++
CXXFLAGS = -g -Wall

BINS = chrono_test timerfd_test

.PHONY: build_all

build_all: ${BINS}

chrono_test: chrono_test.o
	${CXX} ${CXXFLAGS} $^ -o $@

timerfd_test: timerfd_test.o
	${CXX} ${CXXFLAGS} $^ -o $@

.PHONY: clean

clean:
	rm ${BINS} *.o
