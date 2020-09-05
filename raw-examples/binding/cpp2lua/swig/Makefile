CXX = g++
CXXFLAGS = -g -Wall

NAME = mylua
OBJS = ${NAME}_wrap.o
DLLS = ${NAME}.so

.PHONY: build_all

build_all: ${DLLS}

# 不删除中间文件
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
