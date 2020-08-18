CXX = g++
CXXFLAGS := -g -Wall
CXXFLAGS += -I/usr/include/python3.6m

NAME = mypy
SRCS = ${NAME}.cpp
OBJS = ${NAME}.o
DLLS = ${NAME}.so

.PHONY: build_all

build_all: ${DLLS}

${OBJS}: ${SRCS}
	${CXX} ${CXXFLAGS} -fPIC -c $<

${DLLS}: ${OBJS}
	${CXX} ${CXXFLAGS} -shared $^ -o $@

.PHONY: clean

clean:
	rm ${DLLS} *.o
