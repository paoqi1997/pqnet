CXX = g++
CXXFLAGS = -g -Wall

# 获取当前目录下所有cpp及cxx文件
CPP_SRCS := ${wildcard *.cpp}
CXX_SRCS := ${wildcard *.cxx}
# 将其后缀替换为.o
OBJS := ${patsubst %.cpp, %.o, ${CPP_SRCS}}
OBJS += ${patsubst %.cxx, %.o, ${CXX_SRCS}}

NAME = mylua
OBJS = ${NAME}_wrap.o
DLLS = ${NAME}.so

.PHONY: build_all

build_all: ${DLLS}

.PRECIOUS: %_wrap.cpp

%_wrap.cpp: %.i
	swig -c++ -lua -cppext cpp $<

%.o: %.cpp
	${CXX} ${CXXFLAGS} -fPIC -c $<

${DLLS}: ${OBJS}
	${CXX} ${CXXFLAGS} -shared $^ -o $@

.PHONY: clean

clean:
	rm ${DLLS} *.o *_wrap.cpp
