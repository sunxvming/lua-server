
require "strings"

proto.parse("msgtest.proto")


-- init
event_init()

-- create server
server = NetServer:new()
server.on_accept = function(conn)
	
end

server.on_close = function(conn)

end

server.on_msg = function(conn, msgtype, pack)
	-- 从二进制数据解析出实际消息
    
    local family = proto.decode("Family", pack:readData())
    
    print("family = "..t2string(family))

    local data = proto.encode("Family", family)
	conn:sendMsg(111, data);
end

server:listen("127.0.0.1", 3001, false)

event_run()