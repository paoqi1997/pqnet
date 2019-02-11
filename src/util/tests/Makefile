CXX = g++
CXXFLAGS = -g -Wall
LIBS = -pthread

SRCS = ${shell find .. -name '*.cpp'}
OBJS = ${SRCS:.cpp=.o}

threadpool_test: ${OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

.PHONY: clean

clean:
	rm threadpool_test ${OBJS}
