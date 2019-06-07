CXX = g++
CXXFLAGS = -g -Wall
LIBS = -pthread

CORE_SRCS = ${shell find ../.. -path ../../tests -prune -o -name '*.cpp' -print}
CORE_OBJS = ${CORE_SRCS:.cpp=.o}

UTIL_OBJS = ../../../util/logger.o ../../../util/signal.o ../../../util/timestamp.o

BINS = echoclient_test echoserver_test

.PHONY: build_all

build_all: ${BINS}

echoclient_test: echoclient_test.o ${CORE_OBJS} ${UTIL_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

echoserver_test: echoserver_test.o ${CORE_OBJS} ${UTIL_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

.PHONY: clean

clean:
	rm ${BINS} *.o ${CORE_OBJS} ${UTIL_OBJS}
