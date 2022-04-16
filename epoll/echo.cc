//
// Created by root on 7/17/21.
//

#include <iostream>
#include <sys/epoll.h>
#include "../pkg/net/net.h"

#define BUF_SIZE 100
#define EPOLL_SIZE 50

using namespace std;

// g++ -o echo echo.cc ../pkg/net/net.cpp
int main() {
    Server s("tcp", "0.0.0.0", "8080");
    s.Listen(100);

    char buf[BUF_SIZE];

    int epfd = epoll_create(EPOLL_SIZE);
    struct epoll_event *events;
    struct epoll_event event;

    events = (epoll_event*) malloc(sizeof(struct epoll_event) * EPOLL_SIZE);
    // 疑问：为什么这里还需传递 fd，epoll_ctl 的第三个参数不是已经传递了吗？
    // 答：event.data 可以记录一些事件的信息，当发生事件返回时，可以从 event.data
    // 中拿到这些信息，这里再次设置 event.data.fd，当发生事件时就可以知道是哪个 fd 了
    // 如果不设置，那么只能知道事件发生了，但是却不知道该事件的任何信息
    event.data.fd = s.Sockfd();
    event.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, s.Sockfd(), &event);

    while (true) {
        int okcnt = epoll_wait(epfd, events, EPOLL_SIZE, -1);
        if (okcnt == -1) {
            cout << "epoll error" << endl;
            break;
        }

        for (int i = 0; i < okcnt; i++) {
            if (events[i].data.fd == s.Sockfd()) {
                auto conn = s.Accept();
                event.events = EPOLLIN;
                event.data.fd = conn->Connfd();
                epoll_ctl(epfd, EPOLL_CTL_ADD, conn->Connfd(), &event);
                cout << "connected client: " << conn->Connfd() << endl;
            } else {
                int n = read(events[i].data.fd, buf, BUF_SIZE);
                if (n == 0) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
                    close(events[i].data.fd);
                    printf("closed client: %d", events[i].data.fd);
                } else {
                    write(events[i].data.fd, buf, n);
                }
            }
        }
    }

    close(s.Sockfd());
    close(epfd);
}