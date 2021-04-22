CXX = g++
CXXFLAGS = -g -Wall -std=c++11 -I../..
LIBS = -lprotobuf -pthread

ROOT_DIR = ../../pqnet
CORE_DIR = ${ROOT_DIR}/core
PLAT_DIR = ${ROOT_DIR}/platform
UTIL_DIR = ${ROOT_DIR}/util

CORE_SRCS = ${shell find ${CORE_DIR} -path ${CORE_DIR}/tests -prune -o -name '*.cpp' -print}
CORE_OBJS = ${CORE_SRCS:.cpp=.o}
PLAT_OBJS = ${PLAT_DIR}/base.o
UTIL_OBJS = ${UTIL_DIR}/logger.o \
            ${UTIL_DIR}/random.o \
            ${UTIL_DIR}/signal.o \
            ${UTIL_DIR}/timestamp.o

PROTOS := ${wildcard *.proto}
PROTO_OBJS := ${patsubst %.proto, %.pb.o, ${PROTOS}}

BINS = receiver sender

.PHONY: build_all

build_all: ${PROTO_OBJS} ${BINS}

# 不删除中间文件
.PRECIOUS: %.pb.cc

%.pb.cc: %.proto
	protoc $< --cpp_out=.

receiver: receiver.o ${CORE_OBJS} ${PLAT_OBJS} ${UTIL_OBJS} ${PROTO_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

sender: sender.o ${CORE_OBJS} ${PLAT_OBJS} ${UTIL_OBJS} ${PROTO_OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@ ${LIBS}

.PHONY: clean

clean:
	rm *.pb.*
	rm ${BINS} *.o
	rm ${CORE_OBJS} ${PLAT_OBJS} ${UTIL_OBJS}
