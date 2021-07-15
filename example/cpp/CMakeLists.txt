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


    
    