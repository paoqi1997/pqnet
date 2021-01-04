cmake_minimum_required (VERSION 3.4)
project (libutil)

set (SRCS util.cpp)
add_library (util SHARED ${SRCS})
