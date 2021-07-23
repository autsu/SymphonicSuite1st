//
// Created by root on 7/22/21.
//

#include "../../pkg/net/net.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 10

// g++ -o lt_test epoll_et_lt.cc ../pkg/net/net.cpp

void addfd(int epollfd, int fd, bool enable_et) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    if (enable_et) {
        event.events |= EPOLLET;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    util::SetNonBlock(fd);
}

void lt(epoll_event *events, int number, int epollfd, int listenfd) {
    char buf[BUFFER_SIZE];
    for (int i = 0; i < number; ++i) {
        int sockfd = events[i].data.fd;
        if (sockfd == listenfd) {
            struct sockaddr_in client_address;
            socklen_t client_addrlength = sizeof(client_address);
            int connfd = accept(listenfd, (struct sockaddr*) &client_address,
                    &client_addrlength);
            addfd(epollfd, connfd, false);
        } else if (events[i].events & EPOLLIN) {
            printf("event trigger once\n");
            memset(buf, '\0', BUFFER_SIZE);
            int ret = recv(sockfd, buf, BUFFER_SIZE-1, 0);
            if (ret <= 0) {
                close(sockfd);
                continue;
            }
            printf("get %d bytes of content: %s\n", ret, buf);
        } else {
            printf("something else happened\n");
        }
    }
}

void et(epoll_event *events, int number, int epollfd, int listenfd) {
    char buf[BUFFER_SIZE];
    for (int i = 0; i < number; i++) {
        int sockfd = events[i].data.fd;
        if (sockfd == listenfd) {
            struct sockaddr_in client_address;
            socklen_t client_addrlength = sizeof(client_address);
            int connfd = accept(listenfd, (struct sockaddr*) &client_address,
                                &client_addrlength);
            addfd(epollfd, connfd, true);
        } else if (events[i].events & EPOLLIN) {
            printf("event trigger once\n");
            while (true) {
                memset(buf, '\0', BUFFER_SIZE);
                int ret = recv(sockfd, buf, BUFFER_SIZE-1, 0);
                if (ret < 0) {
                    if ( (errno == EAGAIN) || (errno == EWOULDBLOCK) ) {
                        printf("read later\n");
                        break;
                    }
                    close(sockfd);
                    break;
                } else if (ret == 0) {
                    close(sockfd);
                } else {
                    printf("get %d bytes of content: %s\n", ret, buf);
                }
            }
        } else {
            printf("something else happened\n");
        }
    }
}

int main(int argc, char *argv[]) {
    int ret = 0;

    Server server("tcp", "0.0.0.0", "8080");

    int listenfd = server.Sockfd();

    int epollfd = epoll_create(5);
    epoll_event events[MAX_EVENT_NUMBER];

    addfd(epollfd, listenfd, true);

    while (true) {
        int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if (ret < 0) {
            printf("epoll failure\n");
            break;
        }
        lt(events, ret, epollfd, listenfd);

    }

    close(listenfd);
    return 0;
}

