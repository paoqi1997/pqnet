CXX = g++
CXXFLAGS = -g -Wall
LIBS = -pthread

COM_SRCS = ${shell find .. -path ../tests -prune -o -name '*.cpp' -print}
COM_OBJS = ${COM_SRCS:.cpp=.o}

BINS = asynclog_test threadpool_test

.PHONY: build_all

build_all: ${BINS}

asynclog_test: asynclog_test.o ${COM_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

threadpool_test: threadpool_test.o ${COM_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

.PHONY: clean

clean:
	rm ${BINS} ${COM_OBJS}
