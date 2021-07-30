extern "C" {
#include "lua.h"  
#include "lauxlib.h"  
#include "lualib.h"  
}

#include <stdio.h>
#include <stdlib.h>

#include "sol/sol.hpp"


extern void luabind_basepacket(sol::state & lua);
extern void luabind_netserver(sol::state & lua);
extern void luabind_kcpserver(sol::state & lua);
extern void luabind_websocket(sol::state & lua);
extern void luabind_eventloop(sol::state & lua);
extern void luabind_httpserver(sol::state & lua);
extern void luabind_mysql(sol::state & lua);
extern void luabind_redis(sol::state & lua);
extern void luabind_common(sol::state & lua);
extern void luabind_csvpar(sol::state & lua);
extern void luabind_json(sol::state & lua);

extern int luaopen_protolua(lua_State* L);

int main(int argc, char* argv[])
{
	sol::state lua;
	lua.open_libraries();

	// bind c++ && lua
	luabind_basepacket(lua);
	luabind_netserver(lua);
	luabind_kcpserver(lua);
	luabind_websocket(lua);
	luabind_eventloop(lua);
	luabind_httpserver(lua);
	luabind_mysql(lua);
	luabind_redis(lua);
	luabind_common(lua);
	luabind_csvpar(lua);
	luabind_json(lua);

	luaopen_protolua(lua.lua_state());

	if (luaL_dofile(lua.lua_state(), argv[1]) == 1) {
		if (lua_isstring(lua.lua_state(), -1)) {
			printf("load lua file error:%s\n", lua_tostring(lua.lua_state(), -1));
		}
	}

	system("pause");
	return 0;
}