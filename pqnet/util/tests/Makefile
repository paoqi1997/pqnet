CXX = g++
CXXFLAGS = -g -Wall
INCS = -I../../..
LIBS = -pthread

CXXFLAGS += ${INCS}

ROOT_DIR = ../..
UTIL_DIR = ${ROOT_DIR}/util

UTIL_SRCS = ${shell find ${UTIL_DIR} -path ${UTIL_DIR}/tests -prune -o -name '*.cpp' -print}
UTIL_OBJS = ${UTIL_SRCS:.cpp=.o}

BINS = asynclog_test logger_test random_test threadpool_test timestamp_test

.PHONY: build_all

build_all: ${BINS}

asynclog_test: asynclog_test.o ${UTIL_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

logger_test: logger_test.o ${UTIL_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

random_test: random_test.o ${UTIL_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

threadpool_test: threadpool_test.o ${UTIL_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

timestamp_test: timestamp_test.o ${UTIL_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

.PHONY: clean

clean:
	rm ${BINS} *.o ${UTIL_OBJS}
