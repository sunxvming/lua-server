#!/bin/sh

echo "-- start make depend:"

SHDIR=$(dirname `readlink -f $0`)
PROJECT_DIR = $(dirname ${SHDIR})
# 脚本的执行目录在build中，所有cd到根目录
echo "make_depend.sh execute dir:" $SHDIR
