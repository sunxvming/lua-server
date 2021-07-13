

## git tag

### cmake_linux
此标签的编译模式只适用于linux系统，方式如下：
利用shell脚本把程序依赖包进行解压然后再通过make、cmake编译成.a静态库文件并复制到libs目录下，程序编译的时候再去链接静态库文件



## 采用cmake方式编译依赖库
采用cmake方式编译第三方依赖源码，类似于下面的方式
```
add_subdirectory(lua)
...
target_link_libraries(<YOURTARGET> lua_static)
```

但是第三方依赖库不一定提供相应的CMakeLists文件，若真没有提供的话，还得自己编写，所以比较麻烦。
不过一般比较常用的库的话通过"库名 + cmake"进行搜索可以搜索到相应的CMakeLists文件以进行cmake的方式编译。

以下为本程序中用到的一些库的cmake的编译方式

- [CMake based build of Lua 5.4.3](https://github.com/walterschell/Lua)
- [CMake build for libuv](https://github.com/jen20/libuv-cmake)


