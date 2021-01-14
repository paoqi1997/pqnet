CXX = g++
CXXFLAGS = -g -Wall -std=c++11

INSTALL_INC_DIR = /usr/local/include/pqnet
INSTALL_LIB_DIR = /usr/local/lib

CORE_DIR = core
PLAT_DIR = platform
UTIL_DIR = util

CORE_SRCS = ${shell find ${CORE_DIR} -path ${CORE_DIR}/tests -prune -o -name '*.cpp' -print}
CORE_OBJS = ${CORE_SRCS:.cpp=.o}

PLAT_SRCS = ${shell find ${PLAT_DIR} -path ${PLAT_DIR}/tests -prune -o -name '*.cpp' -print}
PLAT_OBJS = ${PLAT_SRCS:.cpp=.o}

UTIL_SRCS = ${shell find ${UTIL_DIR} -path ${UTIL_DIR}/tests -prune -o -name '*.cpp' -print}
UTIL_OBJS = ${UTIL_SRCS:.cpp=.o}

STATIC_OBJS := ${CORE_OBJS}
STATIC_OBJS += ${PLAT_OBJS}
STATIC_OBJS += ${UTIL_OBJS}

SHARED_OBJS = ${STATIC_OBJS:.o=.oo}

STATIC_LIB = libpqnet.a
SHARED_LIB = libpqnet.so.0.1.0

.PHONY: build_all

build_all: ${STATIC_LIB} ${SHARED_LIB}

${STATIC_LIB}: ${STATIC_OBJS}
	ar -rcs ${STATIC_LIB} ${STATIC_OBJS}

${SHARED_LIB}: ${SHARED_OBJS}
	${CXX} ${CXXFLAGS} -shared $^ -o $@
	ln -s ${SHARED_LIB} libpqnet.so
	ln -s ${SHARED_LIB} libpqnet.so.0

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

%.oo: %.cpp
	${CXX} ${CXXFLAGS} -fPIC -c $< -o $@

install:
	mkdir -p ${INSTALL_INC_DIR}/core
	mkdir -p ${INSTALL_INC_DIR}/core/http
	mkdir -p ${INSTALL_INC_DIR}/platform
	mkdir -p ${INSTALL_INC_DIR}/util
	cp pqnet.h ${INSTALL_INC_DIR}
	cp ${CORE_DIR}/*.h ${INSTALL_INC_DIR}/core
	cp ${CORE_DIR}/http/*.h ${INSTALL_INC_DIR}/core/http
	cp ${PLAT_DIR}/*.h ${INSTALL_INC_DIR}/platform
	cp ${UTIL_DIR}/*.h ${INSTALL_INC_DIR}/util
	cp ${STATIC_LIB} ${SHARED_LIB} ${INSTALL_LIB_DIR}
	ln -s ${SHARED_LIB} ${INSTALL_LIB_DIR}/libpqnet.so
	ln -s ${SHARED_LIB} ${INSTALL_LIB_DIR}/libpqnet.so.0

uninstall:
	rm -rf ${INSTALL_LIB_DIR}/libpqnet.* ${INSTALL_INC_DIR}

.PHONY: clean

clean:
	rm libpqnet.*
	rm ${STATIC_OBJS}
	rm ${SHARED_OBJS}
