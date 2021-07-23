//
// Created by zz on 2021/5/29.
//

#ifndef IO_MULTIPLEXING_NET_H
#define IO_MULTIPLEXING_NET_H
#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <string.h>
#include <strings.h>
#include <errno.h>

using std::cout;
using std::string;
using std::endl;
using std::vector;

class Conn;

class Server {
public:
    Server(const string& network, const string& ip, const string& port);
    int Listen(int backlog);
    Conn *Accept();
    int Close();
    int Sockfd();
private:
    string network;
    string port;
    string ip;
    int sockfd;
    void create_socket();
};

class ServerUDP {
public:
    ServerUDP(const string& ip, const string& port);
    long RecvFrom(vector<char> &buf, int flag, sockaddr *from, socklen_t from_len);
    long SendTo(vector<char> &buf, int flag, sockaddr *from, socklen_t from_len);
private:
    int sockfd;
    string network;
    string port;
    string ip;
};

class Conn {
public:
    Conn(int fd, struct sockaddr *remote_addr);
    int Close();
    int Close_read();
    int Close_write();
    int Read(vector<char>& buf, int flag);
    int Write(vector<char>& buf, int flag);
    int Connfd();
private:
    int fd;
    struct sockaddr *remote_addr;
};

class Dial {
public:
    Dial(const string &network);
    Conn Connect(const string &ip, const string &port);
    int Sockfd();
private:
    int sockfd;
};

class util {
public:
    static int SetNonBlock(int fd);
};


#endif //IO_MULTIPLEXING_NET_H
