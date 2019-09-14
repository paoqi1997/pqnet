#include "mylua.h"

int lua_add(lua_State *L)
{
    int n = lua_gettop(L);
    lua_Number sum = 0;
    for (int i = 0; i <= n; ++i) {
        sum += lua_tonumber(L, i);
    }
    lua_pushnumber(L, sum);
    return 1;
}

luaL_Reg funcList[] = {
    {"add", lua_add},
    {nullptr, nullptr}
};

int luaopen_libmylua(lua_State *L)
{
    //lua_register(L, "mylua", funcList);
    lua_newtable(L);
    luaL_setfuncs(L, funcList, 0);
    return 1;
}
