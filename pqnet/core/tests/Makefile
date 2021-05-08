CXX = g++
CXXFLAGS = -g -Wall -std=c++11 -I../../..

ROOT_DIR = ../..
CORE_DIR = ${ROOT_DIR}/core
PLAT_DIR = ${ROOT_DIR}/platform
UTIL_DIR = ${ROOT_DIR}/util

BINS = buffer_test endian_test ipaddr_test timerqueue_test

.PHONY: build_all

build_all: ${BINS}

buffer_test: buffer_test.o ${CORE_DIR}/buffer.o
	${CXX} ${CXXFLAGS} $^ -o $@

endian_test: endian_test.o ${CORE_DIR}/endian.o
	${CXX} ${CXXFLAGS} $^ -o $@

ipaddr_test: ipaddr_test.o ${CORE_DIR}/ipaddr.o ${CORE_DIR}/endian.o
	${CXX} ${CXXFLAGS} $^ -o $@

timerqueue_test: timerqueue_test.o ${CORE_DIR}/timerqueue.o ${UTIL_DIR}/timestamp.o
	${CXX} ${CXXFLAGS} $^ -o $@

.PHONY: clean

clean:
	rm ${BINS} *.o ${CORE_DIR}/*.o ${UTIL_DIR}/*.o
