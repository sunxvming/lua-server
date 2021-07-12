option(USE_LD_GOLD "Use GNU gold linker"  ON)

message("use-gold-linker===========")
# message("CMAKE_C_COMPILER_ID:${CMAKE_C_COMPILER_ID}")

message("USE_LD_GOLD:${USE_LD_GOLD}")

if(USE_LD_GOLD AND "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
  execute_process(COMMAND ${CMAKE_C_COMPILER} -fuse-ld=gold -Wl,--version OUTPUT_VARIABLE stdout ERROR_QUIET)
  # message("stdout:${stdout}")
  if("${stdout}" MATCHES "GNU gold")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fuse-ld=gold")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fuse-ld=gold")
  else()
    message(WARNING "GNU gold linker isn't available, using the default system linker.")
  endif()
endif()
