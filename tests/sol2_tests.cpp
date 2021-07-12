#define XCTEST

#include <gtest/gtest.h>
#include <sol/sol.hpp>

struct vars {
    int boop = 0;
};



TEST(sol2, call_lua_function)
{

    sol::state lua;
    int x = 0;
    lua.set_function("beep", [&x]{ ++x; });
    lua.script("beep()");
    EXPECT_EQ(x, 1);
    
}


TEST(sol2, new_usertype)
{
    sol::state lua;
    lua.new_usertype<vars>("vars", "boop", &vars::boop);
    lua.script("beep = vars.new()\n"
               "beep.boop = 1");
    EXPECT_EQ(lua.get<vars>("beep").boop,  1);
    
}