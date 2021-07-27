local config = DBConfig:new()
config.device = "mysql"
config.ip = "192.168.1.120"
config.dbname = "test_db"
config.user = "root"
config.pswd = "123456"
config.port = 3306

mysql_library_init()
mysql_thread_safe()

pool = DBThreadPool:new(config)
pool:create(1)

func = function(errno, err, result)
	if errno ~= 0 then
		print(err)
		return
	end

	while(result:fetch())
	do
		local id = result:getInt32()
		local num = result:getInt32()
		local name = result:getString()
				
		local str = "id:" .. id .. ", num:" .. num .. ", name:" .. name
		-- errorLog(str)
		print(str)
	end
end

function exec()
	local sql = SqlCommand:new("select * from test where id = ?")
	sql:pushInt32(1)
	sql:addToPool(pool, func)
end

event_init()

exec()

timer = UTimer:new()
timer:start(function ()
	pool:update()
end, 10, 10)

event_run()

mysql_library_end()