cmake_minimum_required(VERSION 3.19)
project(net)

set(CMAKE_CXX_STANDARD 14)

# 子项目，要求子目录里必须有一个子CMakeLists.txt，包含了子目录的编译信息
add_subdirectory(boost)
add_subdirectory(boost/test)
add_subdirectory(pkg/googletest)

# 单元测试
enable_testing()
add_executable(
        net
        main.cpp
        # boost/test_boost_include.cc
        # boost/counter_thread_safe.cc
        # main.cpp
        # pkg/net/net.cpp
        # pkg/net/net.h
        # test/socket_server.cpp
        # sock_listen_backlog_test_server.cpp
        # sock_listen_backlog_test_client.cpp
        # sock_udp_echo_server.cpp
        # sock_udp_echo_client.cpp
        # select/select_demo_noblock_conn.c
        # test/test_dial.cpp
        # select/select_demo_noblock_stdin.c
        # nocopy/splice.cpp
        # nocopy/tee.cpp
        # epoll/epoll_demo.cpp
        # epoll/stdin_to_stdout_lt.c
        # epoll/stdin_to_stdout_et.c
        # epoll/echo.cc
        # select/echo_server.cc
        # nocopy/tee.cpp
        # select/select_single_thread_server.cpp
        # select/echo_server_wrong_example.cc
        # epoll/echo_lt.cc
        # epoll/echo_et.cc
        # thunderingherd/does_accept_has_thundering_herd_problem.cc
        # epoll/baddemo/epoll_et_lt.cc
        # epoll/echo_et_wrong.cc
        # demo/chat_shm.cc
        # process/echo_waitpid.cc
        # process/waitpid.cc
        # process/orphan.cc
        # process/zombie.cc
        # pkg/sync/Mutex.h
)

