//
// Created by zz on 2021/5/29.
//

#include "pkg/net/net.h"
#include <unistd.h>
#include <libgen.h>
#include <iostream>
#include <vector>

using std::vector;

// +build:
// g++ -std=c++11 -o socket_listen_backlog_server  sock_listen_backlog_test_server.cpp  pkg/net/net.cpp

// socket listen() 函数中 backlog 参数的作用


static bool stop = false;

static void handle_term(int sig) {
    stop = true;
}

int main(int argc, char *argv[]) {
    signal(SIGTERM, handle_term);

    if (argc <= 3) {
        printf("usage: %s ip_address port_number backlog\n", basename(argv[0]));
        return 1;
    }

    string ip = argv[1];
    string port = argv[2];
    int backlog = atoi(argv[3]);

    Server socket("tcp", ip, port);
    socket.Listen(backlog);


    for (;;) {
        Conn *c = socket.Accept();
        if (c == nullptr) {
            continue;
        }
        vector<char> buf;
//        int i = c->Read(buf);
//        if (i < 0) {
//            cout << "read error" << endl;
//            break;
//        }
//        cout << "read data bytes: " << i << endl;
//        string res;
//        for (auto v : buf) {
//            res += v;
//        }
//        cout << res << endl;
        buf.emplace_back(48);
        buf.emplace_back(49);
        buf.emplace_back(50);
        buf.emplace_back(51);
        buf.emplace_back(52);
        buf.emplace_back('\0');

        int n = c->Write(buf, 0);
        cout << "send bytes: " << n << endl;
        //c->Close();
        //delete c;
    }

    socket.Close();
    return 0;
}