CXX = g++
CXXFLAGS = -g -Wall -fPIC
INCS = -I/usr/include/python3.6
LIBS = -lpython3.6m

CXXFLAGS += ${INCS}

NAME = mypy
SRCS = ${NAME}.cpp
OBJS = ${NAME}.o
DLLS = ${NAME}.so

.PHONY: build_all

build_all: ${DLLS}

${OBJS}: ${SRCS}
	${CXX} ${CXXFLAGS} -c $< ${LIBS}

${DLLS}: ${OBJS}
	${CXX} ${CXXFLAGS} -shared $^ -o $@

.PHONY: clean

clean:
	rm ${DLLS} *.o
