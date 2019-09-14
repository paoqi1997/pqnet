#include <iostream>

#include "mylua.h"

using std::cout;
using std::endl;

int main()
{
    auto L = luaL_newstate();

    if (luaL_loadfile(L, "main.lua") != LUA_OK) {
        cout << lua_tostring(L, -1) << endl;
        lua_close(L);
        return 1;
    }

    if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
        cout << lua_tostring(L, -1) << endl;
        lua_close(L);
        return 1;
    }

    // s
    lua_getglobal(L, "s");
    auto s = lua_tostring(L, -1);
    cout << s << endl;

    lua_getglobal(L, "obj");
    // obj.name
    lua_getfield(L, -1, "name");
    auto name = lua_tostring(L, -1);
    cout << name << endl;

    lua_pop(L, 1);
    // obj.sum
    lua_getfield(L, -1, "sum");
    int sum = lua_tonumber(L, -1);
    cout << sum << endl;

    // function add
    lua_getglobal(L, "add");
    lua_pushnumber(L, 1);
    lua_pushnumber(L, 2);
    if (lua_pcall(L, 2, 1, 0) != LUA_OK) {
        cout << lua_tostring(L, -1) << endl;
        lua_close(L);
        return 1;
    } else {
        int result = lua_tonumber(L, -1);
        cout << result << endl;
    }

    lua_close(L);

    return 0;
}
