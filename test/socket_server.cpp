//
// Created by zz on 2021/5/29.
//

#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include "../pkg/net/net.h"

using std::vector;

int main() {
    Server s("tcp", "0.0.0.0", "8080");
    s.Listen(1024);

    for (; ;) {
        Conn *conn = s.Accept();

        vector<char> buf;
        buf.emplace_back('1');
        buf.emplace_back('2');
        buf.emplace_back('3');

        //conn->Write(buf, 0);    // test ok

        vector<char> readbuf;
        int n = conn->Read(readbuf, 0);
        cout << "recv bytes: " << n << endl;
        for (auto v : buf) {
            cout << "recv data: " << v << " ";
        }
    }
}