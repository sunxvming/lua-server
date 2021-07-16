cmake_minimum_required(VERSION 3.0)

project (lua-server) 


# ============ 编译选项设置 ============ 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(use-gold-linker)
include(use-ccache)
include(utils)
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")


add_definitions(-Wno-builtin-macro-redefined)

# 打印编译时间
set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK "${CMAKE_COMMAND} -E time")

set(CXX_FLAGS
        -Wall
        )
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -save-temps")




# ============ 编译类型相关 Debug or Release ============ 
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


# ============ 常量定义 ============

# 定义工程根目录; CMAKE_SOURCE_DIR为内建变量，表示工程根目录的CMakeLists.txt文件路径
SET(ROOT_DIR ${CMAKE_SOURCE_DIR})
SET(STATIC_LIB_NAME ${PROJECT_NAME}-static)


# ============ 编译的源码文件 ============
file(GLOB SOURCES 
    "src/common/*.c" 
    "src/common/*.cpp" 
    "src/utils/*.c" 
    "src/utils/*.cpp" 
    "src/network/*.c" 
    "src/network/*.cpp" 
    "src/script/*.c" 
    "src/script/*.cpp"    
    )

file(GLOB_RECURSE MAIN_FILE src/script/main.cpp)
list(REMOVE_ITEM SOURCES ${MAIN_FILE})


# test的源码
file(GLOB_RECURSE TEST_SOURCE tests/*.cpp)
# message(${TEST_SOURCE})



# ============ find_package ============
find_package(Threads REQUIRED)



# ================= 编译目标 =============

# 静态库
add_library(${STATIC_LIB_NAME}  STATIC  
    ${SOURCES})


# 可执行程序    
add_executable(${PROJECT_NAME}
    ${MAIN_FILE})

target_link_libraries(${PROJECT_NAME}
    ${STATIC_LIB_NAME}
    Threads::Threads)



redefine_file_macro(${STATIC_LIB_NAME})


set(EXECUTABLE_OUTPUT_PATH bin)




# ============ 包含路径 ============
include_directories("${ROOT_DIR}/src/common")
include_directories("${ROOT_DIR}/src/network")
include_directories("${ROOT_DIR}/src/utils")
include_directories("${ROOT_DIR}/dependencies/")
include_directories("${ROOT_DIR}/dependencies/sol2/include")
include_directories("${ROOT_DIR}/dependencies/lua/src")
include_directories("${ROOT_DIR}/dependencies/libuv/include")


# ============ 静态库、动态库 ============
# target_link_libraries(${STATIC_LIB_NAME}   pthread ) 
target_link_libraries(${STATIC_LIB_NAME}   dl)



# 添加外部静态
add_library(lua STATIC IMPORTED)
set_property(TARGET lua PROPERTY IMPORTED_LOCATION ${ROOT_DIR}/libs/liblua.a)
target_link_libraries(${STATIC_LIB_NAME}  lua )


add_library(libuv STATIC IMPORTED)
set_property(TARGET libuv PROPERTY IMPORTED_LOCATION ${ROOT_DIR}/libs/libuv_a.a)
target_link_libraries(${STATIC_LIB_NAME}  libuv )

# ============ 子目录编译目标 ============
add_subdirectory(${ROOT_DIR}/example/cpp)


# ============ 单元测试 ============
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
    target_link_libraries(test  dl)
    target_link_libraries(test  Threads::Threads)
endif(GTEST_FOUND)



# ============ 文件复制工作 ============
file(COPY ${ROOT_DIR}/example/lua/
    DESTINATION ${CMAKE_BINARY_DIR}/example/lua/)


ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME}
    POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/bin/${PROJECT_NAME} ${CMAKE_BINARY_DIR}/example/lua
)
