CXX = g++
CXXFLAGS = -g -Wall

BINS = buffer_test endian_test ipaddr_test timerqueue_test

# TIME_OBJS = ../../timer.o ../../timerqueue.o
UTIL_OBJS = ../../../util/logger.o ../../../util/timestamp.o

.PHONY: build_all

build_all: ${BINS}

buffer_test: buffer_test.o ../../buffer.o
	${CXX} ${CXXFLAGS} $^ -o $@

endian_test: endian_test.o ../../endian.o
	${CXX} ${CXXFLAGS} $^ -o $@

ipaddr_test: ipaddr_test.o ../../endian.o ../../ipaddr.o
	${CXX} ${CXXFLAGS} $^ -o $@

timerqueue_test: timerqueue_test.o ../../timer.o ../../timerqueue.o ${UTIL_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@

.PHONY: clean

clean:
	rm ${BINS} *.o ../../*.o ${UTIL_OBJS}
