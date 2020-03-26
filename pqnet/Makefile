CXX = g++
CXXFLAGS = -g -Wall -std=c++11

ROOT_DIR = .
CORE_DIR = ${ROOT_DIR}/core
UTIL_DIR = ${ROOT_DIR}/util

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

.PHONY: clean

clean:
	rm ${LIB} ${OBJS}
