extern "C" {
#include "lua.h"  
#include "lauxlib.h"  
#include "lualib.h"  
}

#include <stdio.h>
#include <stdlib.h>

#include "sol/sol.hpp"


int main(int argc, char* argv[])
{
	sol::state lua;
	lua.open_libraries();

	if (luaL_dofile(lua.lua_state(), argv[1]) == 1) {
		if (lua_isstring(lua.lua_state(), -1)) {
			printf("load lua file error:%s\n", lua_tostring(lua.lua_state(), -1));
		}
	}

	return 0;
}