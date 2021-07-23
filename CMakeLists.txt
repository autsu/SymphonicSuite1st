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
        select/select_demo_noblock_conn.c
        test/test_dial.cpp
        select/select_demo_noblock_stdin.c
        nocopy/splice.cpp
        nocopy/tee.cpp
        epoll/epoll_demo.cpp
        epoll/stdin_to_stdout_lt.c
        epoll/stdin_to_stdout_et.c
        epoll/echo.cc
        select/echo_server.cc
        nocopy/tee.cpp
        select/select_single_thread_server.cpp
        select/echo_server_wrong_example.cc
        epoll/echo_lt.cc
        epoll/echo_et.cc thunderingherd/does_accept_has_thundering_herd_problem.cc epoll/baddemo/epoll_et_lt.cc epoll/echo_et_wrong.cc)

