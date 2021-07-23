//
// Created by root on 7/17/21.
//

#include <sys/epoll.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 2

int main() {
    int epfd, nfds;
    char buf[BUF_SIZE];
    struct epoll_event event, events[5];
    epfd = epoll_create(1);
    event.data.fd = STDIN_FILENO;
    event.events = EPOLLIN | EPOLLET;   // set et
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);

    while (1) {
        nfds = epoll_wait(epfd, events, 5, -1);
        //printf("%d events already\n", nfds);
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == STDIN_FILENO) {
                printf("trigger once!\n");
                memset(buf, '\0', BUF_SIZE);
                read(events[i].data.fd, buf, BUF_SIZE);
                printf("read content: %s\n", buf);
            }
        }
    }
}

