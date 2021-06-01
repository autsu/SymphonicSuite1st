//
// Created by zz on 2021/5/29.
//

#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include "../pkg/net/net.h"

int main() {
    Socket s("tcp", "8080");
    s.Listen(1024);


    for (; ;) {
        int connfd = s.Accept();
        if (connfd < 0) {
            cout << "accept conn error" << endl;
            exit(0);
        }

        char buf[1024];
        //memset(&buf, '\0', sizeof(buf));
        snprintf(buf, sizeof(buf), "%s", "test");
        write(connfd, buf, sizeof(buf));
    }
}