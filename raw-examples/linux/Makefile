CXX = g++
CXXFLAGS = -g -Wall
LIBS = -ldl

BINS = daemon_test  \
       dlopen_test  \
       mmap_test    \
       syscall_test \
       timerfd_test \
       ucontext_test

.PHONY: build_all

build_all: ${BINS}

daemon_test: daemon_test.o
	${CXX} ${CXXFLAGS} $^ -o $@

dlopen_test: dlopen_test.o
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

mmap_test: mmap_test.o
	${CXX} ${CXXFLAGS} $^ -o $@

syscall_test: syscall_test.o
	${CXX} ${CXXFLAGS} $^ -o $@

timerfd_test: timerfd_test.o
	${CXX} ${CXXFLAGS} $^ -o $@

ucontext_test: ucontext_test.o
	${CXX} ${CXXFLAGS} $^ -o $@

.PHONY: clean

clean:
	rm ${BINS} *.o
