#include <iostream>

#include "mylua.h"

using std::cout;
using std::endl;

int main()
{
    auto L = luaL_newstate();

    // ldd cpp2lua1?
    // Makefile: I need -ldl.
    luaL_openlibs(L);

    lua_register(L, "add", lua_add);
    if (luaL_dofile(L, "main.lua") != LUA_OK) {
        cout << lua_tostring(L, -1) << endl;
        lua_close(L);
        return 1;
    }

    lua_close(L);

    return 0;
}
