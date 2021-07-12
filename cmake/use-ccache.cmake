option(CCACHE "Use ccache if available" ON)

find_program(CCACHE_PROGRAM ccache)
message("use-ccache===========")
message("CCACHE:${CCACHE}")
# message("CCACHE_PROGRAM:${CCACHE_PROGRAM}")
if(CCACHE AND CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")

  # message("CMAKE_C_COMPILER:${CMAKE_C_COMPILER}")
  # message("CMAKE_CXX_COMPILER:${CMAKE_CXX_COMPILER}")
  SET(CMAKE_CXX_COMPILER_ARG1 "${CMAKE_CXX_COMPILER}")
  SET(CMAKE_CXX_COMPILER      "ccache")
endif()
