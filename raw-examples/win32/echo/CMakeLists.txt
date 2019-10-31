cmake_minimum_required (VERSION 3.4)
project (winServer)

set (SRCS server.cpp)

if (WIN32)
    add_executable (winServer ${SRCS})
    target_link_libraries (winServer ws2_32)
endif ()
