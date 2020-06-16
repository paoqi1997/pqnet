cmake_minimum_required (VERSION 3.4)
project (iocp)

set (CLI_SRCS ../client.cpp)
set (SERV_SRCS server.cpp)

if (WIN32)
    add_executable (winClient ${CLI_SRCS})
    add_executable (winServer ${SERV_SRCS})
    target_link_libraries (winClient ws2_32)
    target_link_libraries (winServer ws2_32)
endif ()
