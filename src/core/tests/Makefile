CXX = g++
CXXFLAGS = -g -Wall
LIBS = -pthread

COM_SRCS = ${shell find .. -path ../tests -prune -o -name '*.cpp' -print}
COM_OBJS = ${COM_SRCS:.cpp=.o}

CORES = ../buffer.o ../endian.o ../ipaddr.o
UTILS = ../../util/logger.o ../../util/timestamp.o

BINS = buffer_test endian_test ipaddr_test echoserver_test echoclient_test

.PHONY: build_all

build_all: ${BINS}

buffer_test: buffer_test.o ${CORES}
	${CXX} ${CXXFLAGS} $^ -o $@

endian_test: endian_test.o ${CORES}
	${CXX} ${CXXFLAGS} $^ -o $@

ipaddr_test: ipaddr_test.o ${CORES}
	${CXX} ${CXXFLAGS} $^ -o $@

echoserver_test: echoserver_test.o ${COM_OBJS} ${UTILS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

echoclient_test: echoclient_test.o ${COM_OBJS} ${UTILS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

.PHONY: clean

clean:
	rm ${BINS} ${COM_OBJS}
