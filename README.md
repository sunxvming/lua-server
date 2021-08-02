# lua-server

本工程fork自[jwEngine](https://github.com/jwcpp/jwEngine/tree/master),是一个用C++实现的服务器框架。功能包括tcp、udp、kcp、web socket等基本的网络操作模块，mysql、redis等数据库操作模块，以及lua脚本运行模块。可以作为一个轻便、高效的游戏服务器，web接口服务器，其他微服务器等。


## 目录结构
```
├── 3rd            // 第三方依赖源码包
├── cmake          // cmake的include文件
├── doc            // 文档
├── example        // 服务器个模块的使用例子
│   ├── cpp
│   ├── lua
│   └── python
├── libs      
├── src            // 源码文件
│   ├── common     // 通用模块
│   ├── db         // 数据库模块，包括mysql、redis
│   ├── network    // 网络模块
│   ├── protolua   // protobuf对lua语言的支持模块
│   ├── script     // c++对象导入lua环境和lua脚本运行模块
│   └── utils      // 工具类库
└── tests          // 测试代码，测试框架用的是gtest
```


## 第三方依赖
* libuv
跨平台的异步I/O网络库
* sol2
C++绑定Lua脚本的库，仅由头文件组成，方便集成
* lua
* mysql
* hiredis
Redis的C语言客户端库函数，基本实现了Redis的协议的最小集
* kcp
KCP是一个快速可靠协议，能以比 TCP 浪费 10%-20% 的带宽的代价，换取平均延迟降低 30%-40%
* protobuf
免费的、开源的跨平台库，用于序列化结构化数据。


## 编译安装

**步骤一:编译第三方依赖**
1. 进入`3rd`目录
2. 执行`make_depend.sh`脚本文件
此脚本的作用1.把第三方依赖编译成静态库，并复制到libs目录下 2.复制第三方依赖的include头文件到dependencies目录下

**步骤二:编译工程源码**
此工程的编译工具为cmake，执行以下命令进行编译，编译后的二进制程序为`build/bin/lua-server`
```
mkdir build
cd build
cmake && make
```

## lua-server使用例子

### tcp服务器端
运行`lua-server tcp_server.lua`

tcp_server.lua源码
```
-- init
event_init()

-- create server
server = NetServer:new()
server.on_accept = function(conn)
	local pack = NetPacket:new()
	pack:pushString("msg ...")
	conn:sendPacket(1, pack)
end

server.on_close = function(conn)
	print("close")
end

server.on_msg = function(conn, msgtype, pack)
	local str = pack:getString()
	print(str)
	conn:sendPacket(1, pack)
end

server:listen("127.0.0.1", 3001, false)

event_run()
```

### tcp客户端
运行`lua-server tcp_client.lua`

tcp_client.lua源码
```
event_init()

-- create server
client = NetClient:new()
client.on_connect = function(conn, argv)
	print("on onnect:".. argv)
end

client.on_close = function(conn)
	print("close")
end

client.on_msg = function(conn, msgtype, pack)
	local str = pack:getString()
	print(str)
	conn:sendPacket(1, pack)
end

client:connect("127.0.0.1", 3001, false, 1);

event_run()
```


## 其他使用例子说明
参见`example`目录下的各个模块使用的使用例子



## 源码文件说明
参见: [doc/src-note.md](doc/src-note.md)






