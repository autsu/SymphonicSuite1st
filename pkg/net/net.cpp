//
// Created by zz on 2021/5/29.
//

#include "net.h"
#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <vector>
#include <sys/select.h>
#include <fcntl.h>

using std::vector;

void Server::create_socket() {
    int domain = AF_INET;
    int type = -1;
    int ret, _sockfd;
    struct sockaddr_in saddr;

    // TODO tcp tcp4 tcp6 unix
    if (this->network == "tcp") {
        type = SOCK_STREAM;
    } else {
        cout << "not support type of this network, support: [tcp]" << endl;
        exit(0);
    }
    _sockfd = socket(domain, type, 0);
    if (_sockfd < 0) {
        cout << "create socket error: " << strerror(errno) << endl;
        exit(0);
    }

    memset(&saddr, '\0', sizeof(saddr));
    saddr.sin_family = AF_INET;
    if (this->ip.empty()) {
        saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        saddr.sin_addr.s_addr = htonl(stoi(this->ip));
    }
    saddr.sin_port = htons(stoul(this->port));

    ret = bind(_sockfd, (struct sockaddr *) &saddr, sizeof(saddr));
    if (ret < 0) {
        cout << "bind addr to socket error:" << strerror(errno) << endl;
        exit(0);
    }

    this->sockfd = _sockfd;
}

int Server::Listen(int backlog) {
    int ret = listen(this->sockfd, backlog);
    return ret;
}

Server::Server(const string &network, const string &ip, const string &port) {
    this->port = port;
    this->network = network;
    this->ip = ip;
    //cout << port << network << ip << endl;
    create_socket();
}

Conn *Server::Accept() {
    struct sockaddr_in client;
    memset(&client, '\0', sizeof(client));
    socklen_t cli_len = sizeof(client);
    int ret = accept(this->sockfd, (struct sockaddr *) &client, &cli_len);
    if (ret < 0) {
        cout << "accept connect error: " <<  strerror(errno) << endl;
        return nullptr;
    }
    Conn *c = new Conn(ret, (struct sockaddr *) &client);

    return c;
}

int Server::Close() {
    return close(this->sockfd);
}

int Server::Sockfd() {
    return this->sockfd;
}

int Conn::Close() {
    return close(this->fd);
}

// buf.size() 不能为 0
int Conn::Read(vector<char> &buf, int flag) {
    fd_set readfd;
    int ret;
    int n = 0;
    FD_ZERO(&readfd);
    FD_SET(this->fd, &readfd);

    ret = select(this->fd+1, &readfd, nullptr, nullptr, nullptr);
    if (ret <= 0) {
        cout << "select error" << endl;
        exit(0);
    }

    if (FD_ISSET(this->fd, &readfd)) {
        n = recv(this->fd, &buf[0], buf.size(), flag);
    }
    return n;
}

int Conn::Write(vector<char> &buf, int flag) {  // test ok
    return send(this->fd, &buf[0], buf.size(), flag);
}

Conn::Conn(int fd, struct sockaddr *remote_addr) {
    this->fd = fd;
    this->remote_addr = remote_addr;
}

int Conn::Connfd() {
    return this->fd;
}

int Conn::Close_read() {
    return shutdown(this->fd, SHUT_RD);
}

int Conn::Close_write() {
    return shutdown(this->fd, SHUT_WR);
}

ServerUDP::ServerUDP(const string &ip, const string &port) {
    int domain = AF_INET;
    int type = SOCK_DGRAM;
    int ret, sockfd;
    struct sockaddr_in saddr;

    sockfd = socket(domain, type, 0);
    if (sockfd < 0) {
        cout << "create socket error" << endl;
        exit(0);
    }

    memset(&saddr, '\0', sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(stoi(this->ip));
    saddr.sin_port = htons(stoul(this->port));

    ret = bind(sockfd, (struct sockaddr *) &saddr, sizeof(saddr));
    if (ret < 0) {
        cout << "bind addr to socket error" << endl;
        exit(0);
    }

    this->sockfd = sockfd;
}

long ServerUDP::RecvFrom(vector<char> &buf, int flag, struct sockaddr *from, socklen_t from_len) {
    auto n = recvfrom(this->sockfd, &buf[0], buf.size(), flag, from, &from_len);
    return n;
}

long ServerUDP::SendTo(vector<char> &buf, int flag, sockaddr *to, socklen_t to_len) {
    auto n = sendto(this->sockfd, &buf[0], buf.size(), flag, to, to_len);
    return n;
}

Dial::Dial(const string &network) {
    int domain = AF_INET;
    int type = -1;
    int ret, _sockfd;

    if (network == "tcp") {
        type = SOCK_STREAM;
    } else {
        cout << "not support type of this network, support: [tcp]" << endl;
        return;
    }

    _sockfd = socket(domain, type, 0);
    if (_sockfd < 0) {
        cout << "create socket error" << endl;
        exit(0);
    }

    this->sockfd = _sockfd;
}

Conn Dial::Connect(const string &ip, const string &port) {
    struct sockaddr_in saddr;
    memset(&saddr, '\0', sizeof(saddr));
    saddr.sin_family = AF_INET;
    //cout << port << "" << stoul(port) << htons(8080) << endl;
    if (ip.empty()) {
        saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        saddr.sin_addr.s_addr = htonl(stoi(ip));
    }
    saddr.sin_port = htons(stoul(port));
//    cout << "sockfd: " << this->sockfd << " "
//         << "ip: " << saddr.sin_addr.s_addr << " "
//         << "port: " << saddr.sin_port << " "
//         << endl;

    int ret = connect(this->sockfd, (struct sockaddr*) &saddr, sizeof(saddr));
    // TODO 错误处理，调用方如何判断连接是否成功
    if (ret == -1) {
        cout << "connect error" << endl;
        exit(0);
    }
    return Conn(this->sockfd, (struct sockaddr*) &saddr);
}

int Dial::Sockfd() {
    return this->sockfd;
}


int util::SetNonBlock(int fd) {
    int old_opt = fcntl(fd, F_GETFL);
    int new_opt = old_opt | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_opt);
    return old_opt;
}
