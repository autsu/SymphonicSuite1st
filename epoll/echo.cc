//
// Created by root on 7/17/21.
//

#include <iostream>
#include <sys/epoll.h>
#include "../pkg/net/net.h"

#define BUF_SIZE 100
#define EPOLL_SIZE 50


using namespace std;

int main() {
    Server s("tcp", "localhost", "8080");
    s.Listen(100);

    int epfd = epoll_create(EPOLL_SIZE);
    struct epoll_event *events;
    struct epoll_event event;

    events = (epoll_event*) malloc(sizeof(struct epoll_event) * EPOLL_SIZE);
    event.data.fd = s.get_sockfd();
    event.events = EPOLLIN;
    epoll_ctl(epfd, EPOLL_CTL_ADD, s.get_sockfd(), &event);

    while (true) {
        int okcnt = epoll_wait(epfd, events, EPOLL_SIZE, -1);
        if (okcnt == -1) {
            cout << "epoll error" << endl;
            break;
        }

        for (int i = 0; i < okcnt; i++) {
            if (events[i].data.fd == s.get_sockfd()) {
                auto conn = s.Accept();
                event.events = EPOLLIN;
                event.data.fd = conn->get_fd();
                epoll_ctl(epfd, EPOLL_CTL_ADD, conn->get_fd(), &event);
                cout << "connected client: " << conn->get_fd() << endl;
            }
        }
    }
}