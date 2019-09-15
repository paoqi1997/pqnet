#include "mylua.h"

const char *className = "Object";

int createObject(lua_State *L)
{
    auto obj = static_cast<Object**>(lua_newuserdata(L, sizeof(Object*)));
    *obj = new Object();
    luaL_getmetatable(L, className);
    lua_setmetatable(L, -2);
    return 1;
}

int deleteObject(lua_State *L)
{
    auto obj = static_cast<Object**>(luaL_checkudata(L, 1, className));
    delete *obj;
    return 0;
}

int getName(lua_State *L)
{
    auto obj = static_cast<Object**>(luaL_checkudata(L, 1, className));
    lua_settop(L, 0);
    lua_pushstring(L, (*obj)->getName().c_str());
    return 1;
}

int getSum(lua_State *L)
{
    auto obj = static_cast<Object**>(luaL_checkudata(L, 1, className));
    lua_settop(L, 0);
    lua_pushnumber(L, (*obj)->getSum());
    return 1;
}

int setName(lua_State *L)
{
    auto obj = static_cast<Object**>(luaL_checkudata(L, 1, className));
    std::string name = lua_tostring(L, -1);
    (*obj)->setName(name);
    return 0;
}

int setSum(lua_State *L)
{
    auto obj = static_cast<Object**>(luaL_checkudata(L, 1, className));
    int sum = lua_tonumber(L, -1);
    (*obj)->setSum(sum);
    return 0;
}

struct luaL_Reg FuncList[] = {
    {"Object", createObject},
    {"__gc", deleteObject},
    {"getName", getName},
    {"getSum", getSum},
    {"setName", setName},
    {"setSum", setSum},
    {nullptr, nullptr}
};

int luaopen_mylua(lua_State *L)
{
    luaL_newmetatable(L, className);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, FuncList, 0);
    return 1;
}
