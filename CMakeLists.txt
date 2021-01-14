cmake_minimum_required(VERSION 3.4)
project(pqnet)

set(CXX_FLAGS "-g -Wall -std=c++11")
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${CXX_FLAGS})

aux_source_directory(pqnet/core CORE_SRCS)
aux_source_directory(pqnet/platform PLAT_SRCS)
aux_source_directory(pqnet/util UTIL_SRCS)

set(SRCS ${CORE_SRCS} ${PLAT_SRCS} ${UTIL_SRCS})

add_library(pqnet_static STATIC ${SRCS})
add_library(pqnet SHARED ${SRCS})

# 将静态库 libpqnet_static.a 输出为 libpqnet.a
set_target_properties(pqnet_static PROPERTIES OUTPUT_NAME "pqnet")
# 为动态库设置版本号
set_target_properties(pqnet PROPERTIES VERSION 0.1.0 SOVERSION 0)

set(DIRS pqnet/core pqnet/platform pqnet/util)

# 安装pqnet.h
install(FILES pqnet/pqnet.h DESTINATION include/pqnet)
# 安装其他头文件
install(DIRECTORY ${DIRS}
    DESTINATION include/pqnet
    FILES_MATCHING PATTERN "*.h"
    PATTERN "tests" EXCLUDE
)
# 安装静态库及动态库
install(TARGETS pqnet_static pqnet DESTINATION lib)

# 以下命令用于 uninstall pqnet
# sudo xargs rm < install_manifest.txt
