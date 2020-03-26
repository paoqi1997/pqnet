CXX = g++
CXXFLAGS = -g -Wall -std=c++11

INSTALL_INC_DIR = /usr/local/include/pqnet
INSTALL_LIB_DIR = /usr/local/lib

CORE_DIR = core
UTIL_DIR = util

CORE_SRCS = ${shell find ${CORE_DIR} -path ${CORE_DIR}/tests -prune -o -name '*.cpp' -print}
CORE_OBJS = ${CORE_SRCS:.cpp=.o}

UTIL_SRCS = ${shell find ${UTIL_DIR} -path ${UTIL_DIR}/tests -prune -o -name '*.cpp' -print}
UTIL_OBJS = ${UTIL_SRCS:.cpp=.o}

OBJS := ${CORE_OBJS}
OBJS += ${UTIL_OBJS}

LIB = libpqnet.a

.PHONY: build_all

build_all: ${LIB}

${LIB}: ${OBJS}
	ar -rcs ${LIB} ${OBJS}

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

install:
	mkdir -p ${INSTALL_INC_DIR}/core
	mkdir -p ${INSTALL_INC_DIR}/util
	cp ${CORE_DIR}/*.h ${INSTALL_INC_DIR}/core
	cp ${UTIL_DIR}/*.h ${INSTALL_INC_DIR}/util
	cp ${LIB} ${INSTALL_LIB_DIR}

uninstall:
	rm -rf ${INSTALL_INC_DIR} ${INSTALL_LIB_DIR}/${LIB}

.PHONY: clean

clean:
	rm ${LIB} ${OBJS}
