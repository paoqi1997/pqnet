CXX = g++
CXXFLAGS = -g -Wall

BINS = reader writer

.PHONY: build_all

build_all: ${BINS}

reader: reader.o
	${CXX} ${CXXFLAGS} $^ -o $@

writer: writer.o
	${CXX} ${CXXFLAGS} $^ -o $@

.PHONY: clean

clean:
	rm ${BINS} *.o
