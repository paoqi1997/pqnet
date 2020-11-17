CXX = g++
CXXFLAGS = -g -Wall

BINS = daemon_test mmap_test timerfd_test ucontext_test

.PHONY: build_all

build_all: ${BINS}

daemon_test: daemon_test.o
	${CXX} ${CXXFLAGS} $^ -o $@

mmap_test: mmap_test.o
	${CXX} ${CXXFLAGS} $^ -o $@

timerfd_test: timerfd_test.o
	${CXX} ${CXXFLAGS} $^ -o $@

ucontext_test: ucontext_test.o
	${CXX} ${CXXFLAGS} $^ -o $@

.PHONY: clean

clean:
	rm ${BINS} *.o
