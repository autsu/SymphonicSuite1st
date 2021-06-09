cmake_minimum_required(VERSION 3.19)
project(io_multiplexing)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        io_multiplexing
        main.cpp
        pkg/net/net.cpp
        pkg/net/net.h
        test/socket_server.cpp
        sock_listen_backlog_test_server.cpp
        sock_listen_backlog_test_client.cpp
        sock_udp_echo_server.cpp
        sock_udp_echo_client.cpp
        select_demo_noblock_conn.c
        test/test_dial.cpp
        select_demo_noblock_stdin.c
)