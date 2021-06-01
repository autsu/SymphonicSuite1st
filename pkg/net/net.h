//
// Created by zz on 2021/5/29.
//

#ifndef IO_MULTIPLEXING_NET_H
#define IO_MULTIPLEXING_NET_H
#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

using std::cout;
using std::string;
using std::endl;
using std::vector;

class Conn;

class Server {
public:
    Server(const string& network, const string& port);
    int Listen(int backlog);
    Conn *Accept();
    int Close();
private:
    string networt;
    string port;
    int sockfd;
    void create_socket();
};

class Dial {
public:
    Dial(const string& network, const string& port);
    int Connect();

};

class Conn {
public:
    Conn() = default;
    Conn(int fd);
    int Close();
    int Read(vector<char>& buf, int flag);
    int Write(vector<char>& buf, int flag);
private:
    int fd;
};


#endif //IO_MULTIPLEXING_NET_H
