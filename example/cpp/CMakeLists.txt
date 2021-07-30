# tcp
add_executable(tcp_client
    tcp_client.cpp)

target_link_libraries(tcp_client
    ${STATIC_LIB_NAME}
    Threads::Threads)

add_executable(tcp_server
    tcp_server.cpp)

target_link_libraries(tcp_server
    ${STATIC_LIB_NAME}
    Threads::Threads)
    

# kcp    
add_executable(kcp_client
    kcp_client.cpp)

target_link_libraries(kcp_client
    ${STATIC_LIB_NAME}
    Threads::Threads)

add_executable(kcp_server
    kcp_server.cpp)

target_link_libraries(kcp_server
    ${STATIC_LIB_NAME}
    Threads::Threads)    
    
    
    
# http
add_executable(http_server
    http_server.cpp)

target_link_libraries(http_server
    ${STATIC_LIB_NAME}
    Threads::Threads)

add_executable(http_server_ext
    http_server_ext.cpp)

target_link_libraries(http_server_ext
    ${STATIC_LIB_NAME}
    Threads::Threads)    


# mysql
add_executable(mysql_test
mysql_test.cpp)

target_link_libraries(mysql_test
    ${STATIC_LIB_NAME}
    Threads::Threads)

# mysql
add_executable(redis_test
redis_test.cpp)

target_link_libraries(redis_test
    ${STATIC_LIB_NAME}
    Threads::Threads)
    


# json_test
add_executable(json_test
json_test.cpp)

target_link_libraries(json_test
    ${STATIC_LIB_NAME}
    Threads::Threads)


# timer_test
add_executable(timer_test
timer_test.cpp)

target_link_libraries(timer_test
    ${STATIC_LIB_NAME}
    Threads::Threads)



# timer_test
add_executable(protobuf_client_test
    protobuf_test/client/protobuf_client_test.cpp
    protobuf_test/client/msgtest.pb.cc
    )

target_link_libraries(protobuf_client_test
    ${STATIC_LIB_NAME}
    Threads::Threads)
