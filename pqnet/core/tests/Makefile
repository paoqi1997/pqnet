CXX = g++
CXXFLAGS = -g -Wall -std=c++11 -I../../..

ROOT_DIR = ../..
CORE_DIR = ${ROOT_DIR}/core
UTIL_DIR = ${ROOT_DIR}/util

UTIL_OBJS = ${UTIL_DIR}/logger.o ${UTIL_DIR}/timestamp.o

BINS = buffer_test endian_test ipaddr_test timerqueue_test

.PHONY: build_all

build_all: ${BINS}

buffer_test: buffer_test.o ${CORE_DIR}/buffer.o
	${CXX} ${CXXFLAGS} $^ -o $@

endian_test: endian_test.o ${CORE_DIR}/endian.o
	${CXX} ${CXXFLAGS} $^ -o $@

ipaddr_test: ipaddr_test.o ${CORE_DIR}/endian.o ${CORE_DIR}/ipaddr.o
	${CXX} ${CXXFLAGS} $^ -o $@

timerqueue_test: timerqueue_test.o ${CORE_DIR}/eventloop.o \
	${CORE_DIR}/timerqueue.o ${CORE_DIR}/trigger.o ${UTIL_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@

.PHONY: clean

clean:
	rm ${BINS} *.o ${CORE_DIR}/*.o ${UTIL_OBJS}
