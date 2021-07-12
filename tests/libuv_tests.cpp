#define XCTEST

#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include "libuv/include/uv.h"

TEST(libuv, hello_world)
{

    uv_loop_t *loop = static_cast<uv_loop_t*>(malloc(sizeof(uv_loop_t)));
    uv_loop_init(loop);

    printf("Now quitting.\n");
    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
    free(loop);
    
}


