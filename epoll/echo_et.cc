//
// Created by root on 7/18/21.
//


#include "../pkg/net/net.h"
#include <iostream>
#include <sys/epoll.h>

#define BUF_SIZE 4  // 将调用 read 函数时使用的缓冲大小缩减为 4 个字节
#define EPOLL_SIZE 50

int main() {
    Server s("tcp", "0.0.0.0", "8080");
    s.Listen(100);

    char buf[BUF_SIZE];

    int epfd = epoll_create(EPOLL_SIZE);
    struct epoll_event *events;
    struct epoll_event event;

    events = (epoll_event*) malloc(sizeof(struct epoll_event) * EPOLL_SIZE);
    event.data.fd = s.Sockfd();
    event.events = EPOLLIN | EPOLLET;   // ET
    epoll_ctl(epfd, EPOLL_CTL_ADD, s.Sockfd(), &event);

    for (; ;) {
        int okcnt = epoll_wait(epfd, events, EPOLL_SIZE, -1);
        if (okcnt == -1) {
            cout << "epoll error" << endl;
            break;
        }

        // 插入验证 epoll_wait 函数调用次数的语句
        cout << "return epoll_wait" << endl;

        for (int i = 0; i < okcnt; i++) {
            if (events[i].data.fd == s.Sockfd()) {
                auto conn = s.Accept();
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = conn->Connfd();
                epoll_ctl(epfd, EPOLL_CTL_ADD, conn->Connfd(), &event);
                cout << "connected client: " << conn->Connfd() << endl;
            } else {
                int n = read(events[i].data.fd, buf, BUF_SIZE);
                if (n == 0) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
                    close(events[i].data.fd);
                    printf("closed client: %d\n", events[i].data.fd);
                } else {
                    write(events[i].data.fd, buf, n);
                }
            }
        }
    }
}
