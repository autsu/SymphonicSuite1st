//
// Created by zz on 2021/6/11.
//

#include "../pkg/net/net.h"

#define MAXLINE 1024

int main(int argc, char *argv[]) {
    // maxi  client[] 的长度
    // maxfd 最大的 fd，作为 select 的第一个参数
    int maxi, maxfd;
    // nready 准备就绪的 socket 数量
    // client 保存连接客户端的 socket fd
    int nready, client[FD_SETSIZE];
    ssize_t n;
    fd_set rset, allset;
    char buf[MAXLINE];

    Server s("tcp", "0.0.0.0", "8080");
    s.Listen(1024);

    maxfd = s.Sockfd();
    maxi = -1;
    // FD_SETSIZE 1024
    for (int i = 0; i < FD_SETSIZE; ++i) {
        client[i] = -1;
    }

    FD_ZERO(&allset);
    // 将 listen fd 添加到 select
    FD_SET(s.Sockfd(), &allset);

    for ( ; ; ) {
        rset = allset;  //
        nready = select(maxfd+1, &rset, nullptr, nullptr, nullptr);

        // 当第一个客户与服务器建立连接时，listen fd 变为可读
        if (FD_ISSET(s.Sockfd(), &rset)) {
            // 可读后就可以调用 accept() 了
            auto conn = s.Accept();
            int i;
            // 将当前的 conn fd 保存到数组中
            for (i = 0; i < FD_SETSIZE; ++i) {
                if (client[i] < 0) {
                    client[i] = conn->Connfd();
                    break;  // 保存到第一个 -1 的位置
                }
            }

            if (i == FD_SETSIZE) {
                cout << "too many clients" << endl;
                exit(0);
            }

            // 将 conn fd 添加到 select
            FD_SET(conn->Connfd(), &allset);
            if (conn->Connfd() > maxfd) {
                maxfd = conn->Connfd();
            }
            if (i > maxi) {
                maxi = i;
            }
            if (--nready <= 0) {
                continue;
            }
        }

        for (int i = 0; i < maxi; ++i) {
            if ( client[i] < 0) {
                continue;
            }
            if (FD_ISSET(client[i], &rset)) {
                if ( (n = read(client[i], buf, MAXLINE)) == 0) {
                    FD_CLR(client[i], &allset);
                    client[i] = -1;
                } else {
                    write(client[i], buf, n);
                }

                if (--nready <= 0) {
                    break;
                }
            }
        }
    }
}