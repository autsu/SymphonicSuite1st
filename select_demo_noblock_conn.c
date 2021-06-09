//
// Created by zz on 2021/6/3.
//

#include <fcntl.h>
#include <iostream>
#include "./pkg/net/net.h"

using std::string;

int set_no_blocking(int fd) {
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

// 超时连接函数
int unblock_connect(string &ip, string &port, int time) {
    int ret = 0;
    struct sockaddr_in
    Dial dial("tcp");
    // 将 dial 的 sockfd 设置为非阻塞
    int fd = set_no_blocking(dial.get_sockfd());

    Conn conn = dial.Connect(ip, port);

}