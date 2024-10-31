-- https://xmake.io/#/zh-cn/manual/project_target
target("app")
    set_kind("binary")
    set_symbols("debug")
    add_files("main.cpp")
