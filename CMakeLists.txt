cmake_minimum_required(VERSION 3.0)

project (lua-server) 

set(CMAKE_CXX_STANDARD 17)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(use-gold-linker)
include(use-ccache)


# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")


# 编译类型相关 Debug or Release
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -rdynamic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -rdynamic")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -rdynamic")
set(CMAKE_C_FLAGS_RELEASE  "${CMAKE_C_FLAGS_RELEASE} -rdynamic")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

message("CMAKE_BUILD_TYPE is:${CMAKE_BUILD_TYPE}")

# message("CMAKE_CXX_FLAGS_DEBUG :================")
# message(${CMAKE_CXX_FLAGS_DEBUG})

# message("CMAKE_CXX_FLAGS_RELEASE  :================")
# message(${CMAKE_CXX_FLAGS_RELEASE})

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    message(FATAL_ERROR "CMAKE_BUILD_TYPE must be Debug or Release!!!")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
endif()


if(CMAKE_BUILD_TYPE STREQUAL "Release")
endif()



add_definitions(-Wno-builtin-macro-redefined)

# 打印编译时间
set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CMAKE_COMMAND} -E time")

set(CXX_FLAGS
        -Wall
        )
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -save-temps")

# 定义工程根目录; CMAKE_SOURCE_DIR为内建变量，表示工程根目录的CMakeLists.txt文件路径
SET(ROOT_DIR ${CMAKE_SOURCE_DIR})



# 编译那些源码
file(GLOB SOURCES 
    "src/script/*.cpp" 
    )

file(GLOB_RECURSE MAIN_FILE src/script/main.cpp)
list(REMOVE_ITEM SOURCES ${MAIN_FILE})

# message("source:================")
# message(${SOURCES})
# message("MAIN_FILE:================")
# message(${MAIN_FILE})

# test的源码
file(GLOB_RECURSE TEST_SOURCE tests/*.cpp)
# message(${TEST_SOURCE})




# 编译目标
add_executable(${PROJECT_NAME}  ${MAIN_FILE} ${SOURCES})
include (utils)
redefine_file_macro(${PROJECT_NAME})


set(EXECUTABLE_OUTPUT_PATH bin)

# 包含路径
include_directories("${ROOT_DIR}/dependencies/")
include_directories("${ROOT_DIR}/dependencies/sol2/include")
include_directories("${ROOT_DIR}/dependencies/lua-5.4.3/src")
include_directories("${ROOT_DIR}/dependencies/libuv/include")

target_link_libraries(${PROJECT_NAME}   pthread ) 
target_link_libraries(${PROJECT_NAME}   dl)




# 添加外部静态
add_library(lua STATIC IMPORTED)
set_property(TARGET lua PROPERTY IMPORTED_LOCATION ${ROOT_DIR}/dependencies/lua-5.4.3/src/liblua.a)
target_link_libraries(${PROJECT_NAME}  lua )


add_library(libuv STATIC IMPORTED)
set_property(TARGET libuv PROPERTY IMPORTED_LOCATION ${ROOT_DIR}/libs/libuv_a.a)
target_link_libraries(${PROJECT_NAME}  libuv )





# test
find_package(GTest)
if(GTEST_FOUND)
    add_executable(test EXCLUDE_FROM_ALL ${SOURCES} ${TEST_SOURCE})
    
    # 头文件路径
    # GTEST_INCLUDE_DIRS:/usr/local/include
    # message("GTEST_INCLUDE_DIRS:${GTEST_INCLUDE_DIRS}")
    include_directories(${GTEST_INCLUDE_DIRS})

    include_directories("${ROOT_DIR}/dependencies/")
    include_directories("${ROOT_DIR}/dependencies/sol2/include")
    include_directories("${ROOT_DIR}/dependencies/lua-5.4.3/src")
        
    # 依赖库
    # GTEST_BOTH_LIBRARIES:/usr/local/lib64/libgtest.a;/usr/local/lib64/libgtest_main.a
    # message("GTEST_BOTH_LIBRARIES:${GTEST_BOTH_LIBRARIES}")
    # CMAKE_THREAD_LIBS_INIT:-lpthread, gtest依赖于线程库
    # message("CMAKE_THREAD_LIBS_INIT:${CMAKE_THREAD_LIBS_INIT}")    
    target_link_libraries(test ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
    
    target_link_libraries(test  lua )
    target_link_libraries(test  libuv)
    target_link_libraries(test  pthread ) 
    target_link_libraries(test  dl)
    
    
endif(GTEST_FOUND)


