CXX = g++
CXXFLAGS = -g -Wall -std=c++11 -I../..
LIBS = -pthread

ROOT_DIR = ../../pqnet
CORE_DIR = ${ROOT_DIR}/core
PLAT_DIR = ${ROOT_DIR}/platform
UTIL_DIR = ${ROOT_DIR}/util

CORE_SRCS = ${shell find ${CORE_DIR} -path ${CORE_DIR}/tests -prune -o -name '*.cpp' -print}
CORE_OBJS = ${CORE_SRCS:.cpp=.o}
PLAT_OBJS = ${PLAT_DIR}/base.o
UTIL_OBJS = ${UTIL_DIR}/logger.o \
            ${UTIL_DIR}/random.o \
            ${UTIL_DIR}/signal.o \
            ${UTIL_DIR}/timestamp.o

BINS = receiver sender

.PHONY: build_all

build_all: ${BINS}

receiver: receiver.o ${CORE_OBJS} ${PLAT_OBJS} ${UTIL_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

sender: sender.o ${CORE_OBJS} ${PLAT_OBJS} ${UTIL_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

.PHONY: clean

clean:
	rm ${BINS} *.o
	rm ${CORE_OBJS} ${PLAT_OBJS} ${UTIL_OBJS}
