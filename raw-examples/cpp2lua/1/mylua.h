#ifndef MYLUA_H
#define MYLUA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

#ifdef __cplusplus
}
#endif

int lua_add(lua_State *L);

#endif // MYLUA_H
