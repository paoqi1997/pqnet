CXX = g++
CXXFLAGS = -g -Wall

# 获取当前目录下所有cpp文件
SRCS := ${wildcard *.cpp}
# 将其后缀替换为.o
OBJS := ${patsubst %.cpp, %.o, ${SRCS}}

BINS = app

.PHONY: build_all

build_all: ${BINS}

app: ${OBJS}
	${CXX} ${CXXFLAGS} $^ -o $@

.PHONY: clean

clean:
	rm ${BINS} *.o
