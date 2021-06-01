cmake_minimum_required(VERSION 3.19)
project(io_multiplexing)

set(CMAKE_CXX_STANDARD 14)

add_executable(io_multiplexing main.cpp pkg/net/net.cpp pkg/net/net.h test/socket_server.cpp sock_listen_backlog_test_server.cpp sock_listen_backlog_test_client.cpp)