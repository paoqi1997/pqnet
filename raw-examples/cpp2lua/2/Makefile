CXX = g++
CXXFLAGS = -g -Wall -fPIC
LIBS = -llua

SOS = libmylua.so

.PHONY: build_all

build_all: ${SOS}

mylua.o: mylua.cpp
	${CXX} ${CXXFLAGS} -c $< ${LIBS}

libmylua.so: mylua.o
	${CXX} ${CXXFLAGS} -shared $^ -o $@

.PHONY: clean

clean:
	rm ${SOS} *.o
