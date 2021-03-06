CXX = g++
CXXFLAGS = -g -Wall -std=c++11 -I../../..
LIBS = -pthread

ROOT_DIR = ../..
PLAT_DIR = ${ROOT_DIR}/platform
UTIL_DIR = ${ROOT_DIR}/util

PLAT_SRCS = ${shell find ${PLAT_DIR} -path ${PLAT_DIR}/tests -prune -o -name '*.cpp' -print}
PLAT_OBJS = ${PLAT_SRCS:.cpp=.o}

UTIL_SRCS = ${shell find ${UTIL_DIR} -path ${UTIL_DIR}/tests -prune -o -name '*.cpp' -print}
UTIL_OBJS = ${UTIL_SRCS:.cpp=.o}

BINS = asynclog_test logger_test random_test threadpool_test timestamp_test

.PHONY: build_all

build_all: ${BINS}

asynclog_test: asynclog_test.o ${PLAT_OBJS} ${UTIL_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

logger_test: logger_test.o ${PLAT_OBJS} ${UTIL_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

random_test: random_test.o ${PLAT_OBJS} ${UTIL_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

threadpool_test: threadpool_test.o ${PLAT_OBJS} ${UTIL_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

timestamp_test: timestamp_test.o ${PLAT_OBJS} ${UTIL_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

.PHONY: clean

clean:
	rm ${BINS} *.o ${PLAT_OBJS} ${UTIL_OBJS}
