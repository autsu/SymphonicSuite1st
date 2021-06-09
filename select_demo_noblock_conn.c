//
// Created by zz on 2021/6/3.
//

#include <fcntl.h>
#include <strings.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <errno.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdlib.h>

int set_no_blocking(int fd) {
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

// 超时连接函数
int unblock_connect(char *ip, int port, int time) {
    int ret = 0;
    struct sockaddr_in address;
    memset( &address, '\0', sizeof(address) );

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, ip, &address.sin_addr);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // 将 dial 的 sockfd 设置为非阻塞
    int fdopt = set_no_blocking(sockfd);
    ret = connect(sockfd, (struct sockaddr*) &address, sizeof(address));
    if (ret == 0) {
        // 如果连接成功，则恢复 sockfd 的属性，并立即返回
        printf("connect with server immediately\n");
        fcntl(sockfd, F_SETFL, fdopt);
        return sockfd;
    } else if (errno != EINPROGRESS) {
        // 如果连接没有立即建立，那么只有当 errno 为 EINPROGRESS 时才表示连接还在
        // 进行，否则出错返回
        printf("unblock connect not support\n");
        return -1;
    }

    fd_set readfds;
    fd_set writefds;
    struct timeval timeout;

    FD_ZERO(&readfds);
    FD_SET(sockfd, &writefds);

    timeout.tv_sec = time;
    timeout.tv_usec = 0;

    ret = select(sockfd+1, NULL, &writefds, NULL, &timeout);
    if (ret <= 0) {
        // select 超时或出错，立即返回
        printf("connect timeout\n");
        close(sockfd);
        return -1;
    }

    if (!FD_ISSET(sockfd, &writefds)) {
        printf("no events on sockfd found\n");
        close(sockfd);
        return -1;
    }

    int error = 0;
    socklen_t len = sizeof(error);

    if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) {
        printf("get socket option failed\n");
        close(sockfd);
        return -1;
    }

    if (error != 0) {
        printf("connection failed after select with the error: %d\n", error);
        close(sockfd);
        return -1;
    }

    printf("connection ready after select with the socket: %d\n", sockfd);
    fcntl(sockfd, F_SETFL, fdopt);
    return sockfd;
}

int main(int argc, char *argv[]) {
    if (argc <= 2) {
        printf("usage: ip_address port_number \n");
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    int socket = unblock_connect(ip, port, 10);
    if (socket < 0) {
        return 1;
    }
    close(socket);
    return 0;
}