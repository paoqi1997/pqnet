CXX = g++
CXXFLAGS = -g -Wall -std=c++11 -I../..
LIBS = -pthread

ROOT_DIR = ../../pqnet
CORE_DIR = ${ROOT_DIR}/core
UTIL_DIR = ${ROOT_DIR}/util

CORE_SRCS = ${shell find ${CORE_DIR} -path ${CORE_DIR}/tests -prune -o -name '*.cpp' -print}
CORE_OBJS = ${CORE_SRCS:.cpp=.o}
UTIL_OBJS = ${UTIL_DIR}/logger.o \
            ${UTIL_DIR}/signal.o \
            ${UTIL_DIR}/timestamp.o

BINS = httpserver

.PHONY: build_all

build_all: ${BINS}

httpserver: httpserver.o ${CORE_OBJS} ${UTIL_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

.PHONY: clean

clean:
	rm ${BINS} *.o ${CORE_OBJS} ${UTIL_OBJS}