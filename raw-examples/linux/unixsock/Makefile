CXX = g++
CXXFLAGS = -g -Wall

BINS = client server

.PHONY: build_all

build_all: ${BINS}

client: client.o
	${CXX} ${CXXFLAGS} $^ -o $@

server: server.o
	${CXX} ${CXXFLAGS} $^ -o $@

.PHONY: clean

clean:
	rm ${BINS} *.o
