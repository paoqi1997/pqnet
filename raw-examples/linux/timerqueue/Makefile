CXX = g++
CXXFLAGS = -g -Wall

BINS = app

.PHONY: build_all

build_all: ${BINS}

app: main.o timerqueue.o
	${CXX} ${CXXFLAGS} $^ -o $@

.PHONY: clean

clean:
	rm ${BINS} *.o
