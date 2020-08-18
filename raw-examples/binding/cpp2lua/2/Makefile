CXX = g++
CXXFLAGS = -g -Wall
LIBS = -llua

NAME = mylua
SRCS = ${NAME}.cpp
OBJS = ${NAME}.o
DLLS = ${NAME}.so

.PHONY: build_all

build_all: ${DLLS}

${OBJS}: ${SRCS}
	${CXX} ${CXXFLAGS} -fPIC -c $< ${LIBS}

${DLLS}: ${OBJS}
	${CXX} ${CXXFLAGS} -shared $^ -o $@

.PHONY: clean

clean:
	rm ${DLLS} *.o
