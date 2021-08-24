//
// Created by root on 7/17/21.
//

#include <sys/epoll.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 2

int setnonblock(int fd) {
    int old_opt = fcntl(fd, F_GETFL);
    int new_opt = old_opt | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_opt);
    return old_opt;
}

int main() {
    int epfd, nfds;
    char buf[BUF_SIZE];
    struct epoll_event event, events[5];
    epfd = epoll_create(1);
    event.data.fd = STDIN_FILENO;
    event.events = EPOLLIN | EPOLLET;   // set et
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);

    setnonblock(STDIN_FILENO);

    while (1) {
        nfds = epoll_wait(epfd, events, 5, -1);
        //printf("%d events already\n", nfds);
        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == STDIN_FILENO) {
                printf("trigger once!\n");
                memset(buf, '\0', BUF_SIZE);
                while (1) {
                    int n = read(events[i].data.fd, buf, BUF_SIZE);
                    if (n <= 0) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            printf("read EOF\n");
                            break;
                        }
                    }
                    printf("read content: %s\n", buf);
                }

            }
        }
    }
}

