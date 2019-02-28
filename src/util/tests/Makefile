CXX = g++
CXXFLAGS = -g -Wall
LIBS = -pthread

CORE_SRCS = ${shell find .. -path ../tests -prune -o -name '*.cpp' -print}
CORE_OBJS = ${CORE_SRCS:.cpp=.o}

BINS = asynclog_test logger_test threadpool_test timestamp_test

.PHONY: build_all

build_all: ${BINS}

asynclog_test: asynclog_test.o ${CORE_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

logger_test: logger_test.o ${CORE_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

threadpool_test: threadpool_test.o ${CORE_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

timestamp_test: timestamp_test.o ${CORE_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

.PHONY: clean

clean:
	rm ${BINS} *.o ${CORE_OBJS}
