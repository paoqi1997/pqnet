#include "mylua.h"

const char *className = "Object";

int createObject(lua_State *L)
{
    auto p = static_cast<Object**>(lua_newuserdata(L, sizeof(Object*)));
    *p = new Object();
    luaL_getmetatable(L, className);
    lua_setmetatable(L, -2);
    return 1;
}

int deleteObject(lua_State *L)
{
    auto p = static_cast<Object**>(luaL_checkudata(L, 1, className));
    delete *p;
    return 0;
}

int getName(lua_State *L)
{
    auto p = static_cast<Object**>(luaL_checkudata(L, 1, className));
    luaL_argcheck(L, p != nullptr, 1, "null pointer");
    lua_settop(L, 0);
    lua_pushstring(L, (*p)->getName().c_str());
    return 1;
}

int getSum(lua_State *L)
{
    auto p = static_cast<Object**>(luaL_checkudata(L, 1, className));
    luaL_argcheck(L, p != nullptr, 1, "null pointer");
    lua_settop(L, 0);
    lua_pushnumber(L, (*p)->getSum());
    return 1;
}

int setName(lua_State *L)
{
    auto p = static_cast<Object**>(luaL_checkudata(L, 1, className));
    luaL_argcheck(L, p != nullptr, 1, "null pointer");
    luaL_checktype(L, -1, LUA_TSTRING);
    std::string name = lua_tostring(L, -1);
    (*p)->setName(name);
    return 0;
}

int setSum(lua_State *L)
{
    auto p = static_cast<Object**>(luaL_checkudata(L, 1, className));
    luaL_argcheck(L, p != nullptr, 1, "null pointer");
    luaL_checktype(L, -1, LUA_TNUMBER);
    int sum = lua_tonumber(L, -1);
    (*p)->setSum(sum);
    return 0;
}

struct luaL_Reg FnList[] = {
    {"Object", createObject},
    {nullptr, nullptr}
};

struct luaL_Reg FuncList[] = {
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
    luaL_newlib(L, FnList);
    return 1;
}
