CXX = g++
CXXFLAGS = -g -Wall

BINS = buffer_test endian_test ipaddr_test

.PHONY: build_all

build_all: ${BINS}

buffer_test: buffer_test.o ../../buffer.o
	${CXX} ${CXXFLAGS} $^ -o $@

endian_test: endian_test.o ../../endian.o
	${CXX} ${CXXFLAGS} $^ -o $@

ipaddr_test: ipaddr_test.o ../../endian.o ../../ipaddr.o
	${CXX} ${CXXFLAGS} $^ -o $@

.PHONY: clean

clean:
	rm ${BINS} *.o ../../*.o
