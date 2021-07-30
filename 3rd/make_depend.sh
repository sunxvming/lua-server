#!/bin/sh

echo "-- start make depend:"

SH_DIR=$(dirname `readlink -f $0`)
PACKAGE_DIR=${SH_DIR}
PROJECT_DIR=`dirname ${SH_DIR}`
DEPEND_DIR=${PROJECT_DIR}/dependencies
LIBS_DIR=${PROJECT_DIR}/libs

LUA_DIR=${DEPEND_DIR}/lua
SOL2_DIR=${DEPEND_DIR}/sol2
LIBUV_DIR=${DEPEND_DIR}/libuv
HIREDIS_DIR=${DEPEND_DIR}/hiredis
MYSQL_DIR=${DEPEND_DIR}/mysql
PROTOBUF_DIR=${DEPEND_DIR}/protobuf


LUA_PKG=${PACKAGE_DIR}/lua-5.4.3.tar.gz
SOL2_PKG=${PACKAGE_DIR}/sol2-3.2.3.zip
LIBUV_PKG=${PACKAGE_DIR}/libuv-1.34.0.zip
HIREDIS_PKG=${PACKAGE_DIR}/hiredis-1.0.0.tar.gz
MYSQL_PKG=${PACKAGE_DIR}/mysql-5.7.16-linux-glibc2.5-x86_64.zip
PROTOBUF_PKG=${PACKAGE_DIR}/protobuf-3.11.1.zip



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
    mkdir -p ${LUA_DIR}
    tar -xvf ${LUA_PKG} --strip-components 1 -C ${LUA_DIR} 
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
    mv ${SOL2_DIR}-3.2.3 ${SOL2_DIR}
fi


# libuv
if [ -d ${LIBUV_DIR} ]
then 
    echo "libuv ok"
else
    echo "libuv decompression && compile"
    unzip ${LIBUV_PKG} -d ${DEPEND_DIR}
    mv ${LIBUV_DIR}-1.34.0 ${LIBUV_DIR}
    cd ${LIBUV_DIR}
    mkdir build && cd build && cmake .. && make -j $(nproc)
    cp ${LIBUV_DIR}/build/libuv_a.a ${LIBS_DIR}
fi



# hiredis
if [ -d ${HIREDIS_DIR} ]
then 
    echo "hiredis ok"
else
    echo "hiredis decompression && compile"
    mkdir -p ${HIREDIS_DIR}
    tar -xvf ${HIREDIS_PKG} --strip-components 1 -C ${HIREDIS_DIR} 
    cd ${HIREDIS_DIR} && make -j $(nproc)
    cp ${HIREDIS_DIR}/libhiredis.a ${LIBS_DIR}
fi


# mysql
if [ -d ${MYSQL_DIR} ]
then 
    echo "mysql ok"
else
    echo "mysql header file decompression"
    unzip ${MYSQL_PKG} -d ${DEPEND_DIR}
    mv ${MYSQL_DIR}-5.7.16-linux-glibc2.5-x86_64 ${MYSQL_DIR}
    cp ${PACKAGE_DIR}/libmysqlclient.a ${LIBS_DIR}
fi



# protobuf
cd $SHDIR
if [ -d ${PROTOBUF_DIR} ]
then 
    echo "protobuf ok"
else
    unzip ${PROTOBUF_PKG} -d ${DEPEND_DIR}
    mv ${PROTOBUF_DIR}-3.11.1 ${PROTOBUF_DIR}
    cd ${PROTOBUF_DIR} && ./autogen.sh && ./configure && make -j$(nproc)
    cp ${PROTOBUF_DIR}/src/.libs/libprotobuf.a ${LIBS_DIR}
fi