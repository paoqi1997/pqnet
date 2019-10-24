#ifndef MYLUA_H
#define MYLUA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

int luaopen_mylua(lua_State *L);

#ifdef __cplusplus
}
#endif

#endif // MYLUA_H
