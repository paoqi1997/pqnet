#include "mylua.h"

namespace mylua {

int lua_add(lua_State *L) {
    int n = lua_gettop(L);
    lua_Number sum = 0;
    for (int i = 0; i <= n; ++i) {
        sum += lua_tonumber(L, i);
    }
    lua_pushnumber(L, sum);
    return 1;
}

} // namespace mylua

struct luaL_Reg FuncList[] = {
    {"add", mylua::lua_add},
    {nullptr, nullptr}
};

int luaopen_mylua(lua_State *L)
{
    lua_newtable(L);
    luaL_setfuncs(L, FuncList, 0);
    return 1;
}
