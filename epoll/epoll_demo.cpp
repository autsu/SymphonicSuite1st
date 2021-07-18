//
// Created by parallels on 6/14/21.
//

#include <sys/epoll.h>

// epoll api
int main() {
    struct epoll_event event;
    struct epoll_data;

    //
    int epfd = epoll_create(100);
    epoll_create1(1);

    // epfd: 
    epoll_ctl(epfd, EPOLL_CTL_ADD, )
}