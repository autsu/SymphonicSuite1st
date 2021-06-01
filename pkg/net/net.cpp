//
// Created by zz on 2021/5/29.
//

#include "net.h"
#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <vector>

using std::vector;

void Server::create_socket() {
    int domain = AF_INET;
    int type = -1;
    int ret, sockfd;
    struct sockaddr_in saddr;

    if (this->networt == "tcp") {
        type = SOCK_STREAM;
    } else if (this->networt == "udp") {
        type = SOCK_DGRAM;
    } else {
        cout << "not support type of this network, support: [tcp, udp]" << endl;
        exit(0);
    }
    sockfd = socket(domain, type, 0);
    if (sockfd < 0) {
        cout << "create socket error" << endl;
        exit(0);
    }

    memset(&saddr, '\0', sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(stoul(this->port));

    ret = bind(sockfd, (struct sockaddr *) &saddr, sizeof(saddr));
    if (ret < 0) {
        cout << "bind addr to socket error" << endl;
        exit(0);
    }

    this->sockfd = sockfd;
}

int Server::Listen(int backlog) {
    int ret = listen(this->sockfd, backlog);
    return ret;
}

Server::Server(const string &network, const string &port) {
    this->port = port;
    this->networt = network;
    create_socket();
}

Conn *Server::Accept() {
    int ret = accept(this->sockfd, (struct sockaddr *) nullptr, nullptr);
    if (ret < 0) {
        cout << "accept connect error" << endl;
        return nullptr;
    }
    Conn *c = new Conn(ret);
    return c;
}

int Server::Close() {
    return close(this->sockfd);
}

int Conn::Close() {
    return close(this->fd);
}

int Conn::Read(vector<char>& buf, int flag) {
    return recv(this->fd, &buf[0], buf.size(), flag);
}

int Conn::Write(vector<char>& buf, int flag) {
    return send(this->fd, &buf[0], buf.size(), flag);
}

Conn::Conn(int fd) {
    this->fd = fd;
}


