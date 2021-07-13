#!/bin/sh

echo "-- start make depend:"

SH_DIR=$(dirname `readlink -f $0`)
PACKAGE_DIR=${SH_DIR}
PROJECT_DIR=`dirname ${SH_DIR}`
DEPEND_DIR=${PROJECT_DIR}/dependencies
LIBS_DIR=${PROJECT_DIR}/libs

LUA_DIR=${DEPEND_DIR}/lua-5.4.3
SOL2_DIR=${DEPEND_DIR}/sol2-develop
LIBUV_DIR=${DEPEND_DIR}/libuv-1.x

LUA_PKG=${PACKAGE_DIR}/lua-5.4.3.tar.gz
SOL2_PKG=${PACKAGE_DIR}/sol2-develop.zip
LIBUV_PKG=${PACKAGE_DIR}/libuv-1.x.zip



if [ ! -d ${DEPEND_DIR} ]
then
    echo "create dependencies dir"
    mkdir ${DEPEND_DIR}
fi    

if [ ! -d ${LIBS_DIR} ]
then
    echo "create libs dir"
    mkdir ${LIBS_DIR}
fi    


# lua
if [ -d ${LUA_DIR} ]
then 
    echo "lua ok"
else
    echo "lua decompression && compile"
    tar -xvf ${LUA_PKG} -C ${DEPEND_DIR}
    cd ${LUA_DIR} && make -j $(nproc)
    cp ${LUA_DIR}/src/liblua.a ${LIBS_DIR}
fi


# sol2
if [ -d ${SOL2_DIR} ]
then 
    echo "sol2 ok"
else
    echo "sol2 decompression"
    unzip ${SOL2_PKG} -d ${DEPEND_DIR}
fi


# libuv
if [ -d ${LIBUV_DIR} ]
then 
    echo "libuv ok"
else
    echo "libuv decompression && compile"
    unzip ${LIBUV_PKG} -d ${DEPEND_DIR}
    cd ${LIBUV_DIR}
    mkdir build && cd build && cmake .. && make -j $(nproc)
    cp ${LIBUV_DIR}/build/libuv_a.a ${LIBS_DIR}
fi

