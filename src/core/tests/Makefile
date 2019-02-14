CXX = g++
CXXFLAGS = -g -Wall

COM_SRCS = ${shell find .. -path ../tests -prune -o -name '*.cpp' -print}
COM_OBJS = ${COM_SRCS:.cpp=.o}

BINS = buffer_test endian_test

.PHONY: build_all

build_all: ${BINS}

buffer_test: buffer_test.o ${COM_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@

endian_test: endian_test.o ${COM_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@

.PHONY: clean

clean:
	rm ${BINS} ${COM_OBJS}
