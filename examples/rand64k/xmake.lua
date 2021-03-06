-- https://xmake.io/#/zh-cn/manual/project_target
target("receiver")
    set_kind("binary")
    add_options("conf")
    add_files("receiver.cpp")
    add_files("../../pqnet/core/*.cpp")
    add_files("../../pqnet/platform/base.cpp")
    add_files("../../pqnet/util/logger.cpp")
    add_files("../../pqnet/util/random.cpp")
    add_files("../../pqnet/util/signal.cpp")
    add_files("../../pqnet/util/timestamp.cpp")

target("sender")
    set_kind("binary")
    add_options("conf")
    add_files("sender.cpp")
    add_files("../../pqnet/core/*.cpp")
    add_files("../../pqnet/platform/base.cpp")
    add_files("../../pqnet/util/logger.cpp")
    add_files("../../pqnet/util/random.cpp")
    add_files("../../pqnet/util/signal.cpp")
    add_files("../../pqnet/util/timestamp.cpp")

-- https://xmake.io/#/zh-cn/manual/configuration_option
option("conf")
    add_includedirs("../..")
    add_links("pthread")
    add_cxxflags("-g", "-Wall", "-std=c++11")
