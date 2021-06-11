//
// Created by zz on 2021/6/6.
//

#ifdef linux

#include <string.h>

#endif

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/select.h>
#include <strings.h>

int set_stdin_noblock() {
    int oldop = fcntl(STDIN_FILENO, F_GETFL);
    int newop = oldop | O_NONBLOCK;
    fcntl(STDIN_FILENO, F_SETFL, newop);
    return oldop;
}

int main() {
    set_stdin_noblock();
    char buf[50];
    memset(&buf, '\0', sizeof(buf));

    fd_set readfd;
    struct timeval timeout;

    FD_ZERO(&readfd);
    FD_SET(STDIN_FILENO, &readfd);

    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    int ret = select(STDIN_FILENO + 1, &readfd, NULL, NULL, &timeout);
    if (ret <= 0) {
        printf("select timeout or error\n");
        exit(0);
    }

    if (!FD_ISSET(STDIN_FILENO, &readfd)) {
        printf("no events on sockfd found\n");
        return -1;
    }

    int n = read(STDIN_FILENO, buf, sizeof(buf));
    printf("read bytes: %d\n", n);
    printf("content: %s\n", buf);
}