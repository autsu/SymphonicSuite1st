//
// Created by zz on 2022/3/21.
//

#include <sys/socket.h>
#include <csignal>
#include <cstdlib>
#include <cstdio>
#include "../pkg/net/net.h"

#define BUF_SIZE 4096

void signChildHandler(int sig) {
    // -1 代表等待当前进程的所有子进程，第二个参数用于记录导致返回的子进程的信息，如果指定为 null
    // 则代表忽略该信息，第三个参数 option 可以配置行为，
    while (waitpid(-1, nullptr, WNOHANG) > 0) {
        cout << "wait child exit..." << endl;
    }
}

void echo(int fd, char* buf) {
    bzero(buf, BUF_SIZE);
    int n = read(fd, buf, BUF_SIZE);
    //cout << "read " << n << " bytes" << endl;
    write(fd, buf, n);
}

int main(int argc, char **argv) {
    char buf[BUF_SIZE];

    if (argc != 3) {
        fprintf(stderr, "usage: %s <ip> <port>\n", argv[0]);
        exit(0);
    }

    // 子进程停止时触发 SIGCHLD 信号
    signal(SIGCHLD, signChildHandler);
    Server s("tcp", argv[1], argv[2]);
    s.Listen(1024);

    while (true) {
        auto conn = s.Accept();
        int ret = fork();
        if (ret == 0) { // child
            // 关闭父进程的监听 fd，因为子进程用不到该 fd，但是 fork 会复制 fd 表，
            // 所以需要关闭使得监听 fd 的引用计数 -1，来防止内存泄漏
            close(s.Sockfd());
            echo(conn->Connfd(), buf);
            close(conn->Connfd());
            exit(0);
        }
        close(conn->Connfd());
    }
}
