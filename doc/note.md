

## 脚本模块


## 网络模块

### tcp相关
```
EventLoop
    功能就是对libuv做了一下简单的封装
    init()
    run()
    exit()
    getLoop()
        返回的是uv_loop_t



NetEvent
    NetConnect * createConnect
    destroyConnect(NetConnect * conn)
    
    onAccept(NetConnect * conn)
    onConnect(NetConnect * conn)
    onClose(NetConnect * conn)
    onMsg(NetConnect * conn)
    
    
socket
-------------
MessageBuffer
    结构为rpos,wpos,storage = std::vector<uint8> 

    每个socket都有一个自己提供的buffer供libuv用，这个buffer可以复用，他的生命周期和TcpSocketBase一样长
    当有读事件的时候，libuv会自动把数据填入的这个buffer中

TcpSocketBase
    成员
        uv_tcp_t m_uv_tcp;       //在connect的时候通过uv_tcp_init()初始化
        uv_write_t m_write_t;
        MessageBuffer mBuffer;   
        
    localIP()
    localPort()
    remoteIP()
    remotePort()
    getUvTcp()
        返回的是uv_tcp_t，网络操作还多都得靠他
    
    write(const uv_buf_t* buf, uint32 size)
        uv_write(x,x,x,x,echo_write)
    echo_write() 写事件完了之后的回调  
        self->on_writecomplete()
        
    on_read_start()    // 开启监听读事件
        uv_read_start((uv_stream_t *)getUvTcp(), alloc_buffer, echo_read)
    echo_read() 读事件完了之后的回调
        出错
            uv_close((uv_handle_t*)tcp, on_uv_close);
                TcpSocketBase::on_uv_close()
                    回调user的self->on_clsesocket();
        buffer有数据时，通知上层的connect对象进行处理
            self->on_msgbuffer(&self->mBuffer);

TcpSocket:TcpSocketBase
    setUserdata()
    getUserdata()
    setUsernum()
    getUsernum()
    
NetConnect:TcpSocket
    发送相关函数
        sendMsg()
        sendPacket()
        sendDate()
            构建NetPacket包 header + body
            把构建的包push到mSendPackets中
            TcpSocket::write(pack)
            send_top_msg()
                从mSendPackets中取出包进行发送
                TcpSocketBase::write(&mSending[0], mSending.size());
                    uv_write(x,x,x,x,echo_write)
                    echo_write()
                        self->on_writecomplete()
                            包得再发送成功后才能删除
                            清理mSendPackets
                            清理mSending
    接收相关函数
        on_msgbuffer()
            从MessageBuffer中读数据，先读header再读body,最终创建一个NetPacket，并将NetPacket传入到user的回调函数
            _netevent->onMsg(this, mReadPacket->getMsgType(), mReadPacket);
    
    其他
        on_clsesocket()

server
--------------    
TcpServer
    基类，提供tcp服务，继承着只需现实相应的虚方法即可
    成员
        uv_tcp_t m_uv_tcp;  //服务器的socket

    listen()
        uv_listen((uv_stream_t*)&m_uv_tcp, DEFAULT_BACKLOG, on_new_connection);
            有新连接的时候回调 on_new_connection(uv_stream_t *server, int status)
            
    shutdown()
    
    on_new_connection()
        此处有两个回调
            TcpSocketBase::on_read_start(), 此时的处理逻辑转移到了socket对象中
            onSocket()    
    virtual createSocket() = 0
    virtual onSocket() = 0


NetServer:TcpServer
    createSocket()
        创建TcpSocket对象
        _netevent->createConnect()
    onSocket()
        _netevent->onAccept((NetConnect *)connect)


clinet
--------------
TcpClient
    connect()
        核心对外函数，连接指定服务器
        此处有两个回调
            TcpSocketBase::on_read_start()
            onSocket()
    virtual createSocket() = 0
    virtual onSocket() = 0

NetClient:TcpClient
    createSocket()
        创建TcpSocket对象
    onSocket()
        _netevent->onConnect((NetConnect *)conn, argv) 
        调用user传入的onConnect回调函数,主要是判断连接成没成功，没成功第一个参数为null


packet
-------------
ByteConverter
    字节序的转换
ByteBuffer
    rpos,wpos,storage

BasePacket:ByteBuffer
    一系列操作buffer的方法
    
    
NetPacket:BasePacket
    head = msglen:uint32 + msgtype:uint32 
    
    getMarkLen()
    getMsgType()
    isHeadFull()
    
    sendSize()
    sendStream()
    
    writeHead()
    readHead()

ProtoPacket
    readProto()
        从NetPacket包中读出protobuf对象
    writeProto()
        将protobuf对象写入到NetPacket包中


```


### udp相关
```
UdpClient
    connect()
        uv_udp_recv_start(&_udp, UdpClient::alloc_buffer, UdpClient::on_read);
        
    writeToServer()
        uv_udp_try_send()
        
    on_read()
        self->onMessage(addr, rcvbuf->base, nread);
        
        
    virtual onMessage() = 0  user继承后，由user具体实现其逻辑


UdpServer
    start()
        uv_udp_bind()
        uv_udp_recv_start(x,x,on_read)
    on_read()
        再调用onMessage()
    virtual onMessage() = 0  user继承后，由user具体实现其逻辑


UdpPacket
    packet = head(4byte,length) + body

```


### http相关
```
HttpEvent
    virtual onClose() = 0
    virtual onGet()   = 0
    virtual onPost()  = 0
    virtual onOther() = 0


HttpServer : TcpServer,HttpEvent
    其中继承了TcpServer的所有功能，其中最主要的就是开启tcp的监听
   
    TcpServer::on_new_connection()
        此处有两个回调
            TcpSocketBase::on_read_start(), 此时的处理逻辑转移到了socket对象中
            onSocket() 
            当buffer中有数据的话，最终会调用Connect对象的 on_msgbuffer
    
    onSocket()
        方法体为空 
    createSocket()
        和tpc不同的地方为创建不同的socket对象，以处理不同的逻辑

    onGet()
        根据url查找注册的处理方法并回调
    onPost()
        根据url查找注册的处理方法并回调
    onOther()
        调用m_other function
    onClose()
        CommPool::reclaim(conn)  复用connect连接对象

HttpConnect : TcpSocket
    接收
        on_msgbuffer()
            主要逻辑就是从buffer中读数据，并进行http包的处理
            解析完整的http包后调用complete()
        complete()
            根据类型进行以下回调
            HttpServer::onPost()
            HttpServer::onGet()
            HttpServer::onOther()
        
    发送    
        sendMsg(const char* msg, int32 len)
        sendData(std::string_view sv)
        autoMsg(std::string_view sv, enum http_content_type type = hct_text_html);
    
    
    on_writecomplete()
        keep_alive = 0时在写入完成后则关闭这条连接
    
    成员
        HttpParser m_parser;
        
        
HttpParser : llhttp_t
    主要是解析http header中的内容
    parser()
    contentLen()
    method()
    isClose()
    getUrl()
    getParser()
    成员：
        ParseUrl m_url;

ParseUrl
    解析url的工具类，底层使用http_parser.h中提供的方法
    parse()
    haveParam()
    getPath()
    getParam()
    
    成员
        http_parser_url

http_parser.h
    http_parser_parse_url()  主要使用此方法解析url
```

### kcp相关


### websocket相关
```
WebSocketEvent
    createConnect()
    destroyConnect()
    onHandshake()
    onAccept()
    onClose()
    onMsg()


WebSocketConnect : TcpSocket
    parseHandshake
        解析握手
    respondHandshake
        响应握手

    接收
        on_msgbuffer    
            respondHandshake()
            回调WebSocketEvent::onHandshake()
            回调WebSocketEvent::onMsg()
    发送
        sendMsg()
        sendPacket()
        sendData()
        
        
        
WebSocketServer : TcpServer
    createSocket()
        调用WebSocketEvent::createConnect()
    onSocket()
        调用WebSocketEvent::onAccept()
    
    
```





## 通用模块

```
ObjectPool
    对象池，目的是做到对象的重复利用，以减少内存重复的分配和回收

CommonPool


线程池：Task、CThread、ThreadPool
---------------------------------------
Task
    virtual void process() = 0;   // 子类需要实现的接口，作为主要处理的逻辑
    virtual void complete() = 0;  // 

CThread
    构造时创建并开启线程，线程运行的函数为CThread::backfunc
    
    backfunc()
        t->onStart()     // t = CThread
        if t->_isrun
            从_waitTasks队列中取task，然后进行处理
            t->run(task);
                task->process(); 调用task的process()接口
            处理完了之后将任务放到_completeTasks中
            t->_pool->_completeTasks.push(task)
        t->onEnd()
    run()
        task->process();


    
ThreadPool
    成员变量
		std::vector<CThread *> _threads;
		TQueue<TaskPtr> _waitTasks;        // 类型为ConcurrentQueue
		TQueue<TaskPtr> _completeTasks;    // 类型为ConcurrentQueue

    virtual CThread* createThread() = 0;

    create()
        创建n个线程并push到_threads中
        其中调用createThread去真正创建线程，不用的继承类可以有不同的实现
    addTask()
        将task push到_waitTasks中
    update()
        处理已经完成的任务，主要是调用完成后的回调函数
        从_completeTasks中取task
        task->complete()   // 逻辑由Task的继承类来实现
        DBThreadPool::completeTask(task)
    popWaitTask()
        从_waitTasks中pop出task，以交给thread处理
        


csv文件
---------------
CsvRow
    成员变量
        std::vector<std::string> _values;
    
CsvParser
    成员变量
        std::vector<std::string> _header;
        std::vector<CsvRow> _content;


json
-----------------
JBaseObj
    type
    write()
        将json对象转换成json字符串

JValue:JBaseObj
    用一个枚举和string来表示一个json的值
JArray:JBaseObj
    成员变量：
        std::vector<JBaseObj *> items;   //按index进行索引

JSonObj:JBaseObj
    成员变量：
        std::vector<JBaseObj *> items;    //按key进行索引

JsonBuff
    

JsonReader
    parser()
        解析json串为json对象
    write()
        将json对象转换成json字符串
```



## 数据库模块
```
DB_Interface
    m_ip,m_port
    connect()
    detach()
    execute()
    getError()
    getErrno()

mysql
-----------------------
DBInterfaceMysql:DB_Interface
    connect()
    detach()
        mysql_close()
    execute()
        mysql_real_query()
        mysql_store_result()
        static_cast<MysqlResult *>(result)->setResult(pResult); // 将结果设置到MysqlResult的pResult中

    MySQL::libraryInit()
        mysql_library_init()
    MySQL::libraryEnd()
        mysql_library_end()
    MySQL::threadSafe()
        mysql_thread_safe()


DBResult
    isEmpty()
    fetch()
    getRowCount()
    getFieldsCount()
    getDate()


MysqlResult:DBResult
    fetch()
        mysql_fetch_row()  //从pResult中mysql_fetch_row到aRow中
    getRowCount()
        mysql_num_rows()
    getFieldsCount()
        mysql_num_fields()
    setResult()
        执行DBInterfaceMysql::execute()的时候会将结果复制给pResult
    
    成员变量：
        MYSQL_RES * pResult;  // 代表所有结果，每次fetch()时会从pResult中fetch一条
        MYSQL_ROW aRow;       // 代表一行结果，每次fetch()的时候将结果赋给aRow
        uint32 pos;           // 代表一行结果中字段的位置
    
SqlResultSet:DBResult
    fetch()
    setResult()
        主要是往m_fields设置结果集
    getXXX()
        getField()

    成员变量
        std::vector<SqlField> m_fields;   // 结果集在m_fields中
        int64 m_currRowIdx;   //考row和col来从结果集中取数据
        int32 m_currColIdx;
        int64 m_rowCount;
        int32 m_fieldCount

SqlPrepare
    SqlPrepare()
        构造的时候初始化MYSQL_BIND m_paramBind
    prepare()
    execute()
        _query()
            mysql_stmt_bind_param()
            mysql_stmt_execute()
        mysql_stmt_store_result()    
        static_cast<SqlResultSet*>(resultSet)->setResult()
    pushxxx()
        最终会调用SetParameterValue()
    SetParameterValue()
        设置MYSQL_BIND的值
    

redis
----------------------------
DBInterfaceRedis::DB_Interface
    成员变量
        redisContext * m_context;
    connect()
        hiredis-->redisConnect()
    detach()
        hiredis-->redisFree()
    execute(DBResult * result, const char * cmd, int len)
        redisCommand()
        static_cast<RedisResult *>(result)->setResult(pRedisReply);
        
    execute(RedisCommand * command, DBResult * result)
        redisCommandArgv()  //这个可以传递多个参数，并且可以支持二进制的数据
        static_cast<RedisResult *>(result)->setResult(pRedisReply);
    ping()
        发送ping Command，看是否恢复PONG，用来测试redis是否连通

RedisCommand
    用来构造redis的命令
    最终会将构造的命令传递给redisCommandArgv()函数进行命令的执行

RedisResult
    fetch()
        根据getRowCount()判断是否还有数据
    getRowCount()
        
    getFieldsCount()

    setResult()
        执行DBInterfaceRedis::execute()的时候会将结果复制给m_reply
    
    成员变量：
        redisReply * m_reply;   // redis返回的结果集
        uint32 pos              // 在结果集的element中所处的位置


数据库线程池
------------------------
DBTask:Task
    dbi()
        设置DB_Interface，是mysql的还是redis的


DBSqlTask:DBTask
    process()
        SqlPrepare::prepare()
        SqlPrepare::execute()
    complete()
        若有backfunc回调函数，则调用backfunc回调函数
        函调函数的逻辑为：从SqlResultSet中取数据


DBRedisTask:DBTask
    process()
        调用DBInterfaceRedis::execute()
    complete()
        若有backfunc回调函数，则调用backfunc回调函数
        函调函数的逻辑为：从SqlResultSet中取数据


DBThread:CThread
    onStart()
        1.根据DBThreadPool的config判断是生成DBInterfaceMysql还是DBInterfaceRedis
        2.DB_Interface::connect()
    run()
        设置task的DB_Interface
        CThread::run(task);
    onEnd()
        delete DB_Interface

DBThreadPool:ThreadPool
    createThread()
        new DBThread(this);
    deleteThread()
        delete CThread
    completeTask()
        方法体为空

```

### mysql接口
```
连接数据库、执行语句
mysql_init()
mysql_options()
mysql_real_connect()
mysql_real_query()
mysql_store_result()
mysql_close()

取数据
mysql_fetch_row()
mysql_num_rows()
mysql_num_fields()

sql语句绑定参数
mysql_stmt_init()
mysql_stmt_prepare()
mysql_stmt_param_count()
mysql_stmt_attr_set()

mysql_stmt_bind_param()
mysql_stmt_execute()
mysql_stmt_result_metadata()
mysql_stmt_store_result()
mysql_stmt_affected_rows()
mysql_free_result()


mysql_error()
mysql_errno()
```


## protobuf模块
此模块的源码来自于github上的[protolua](https://github.com/jinjiazhang/protolua)工程，实现了在lua中对protobuf message对象的序列化和反序列化。
原始工程中是将protobuf message对象的序列化和反序列化的方法等通过c lua api导入到lua环境中，然后将c++源代码编译成名字为protolua的动态库以供lua中使用。
在lua的代码中使用时只需加入`require "protolua"`，就可以使用protobuf相关的方法了。


## 第三方插件


### http相关
http_parser

llhttp




## todo
c++中默认参数到lua中会不生效
HttpConnect.h中的http_content_type枚举没有导入到lua中