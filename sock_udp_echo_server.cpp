//
// Created by zz on 2021/6/2.
//

#include <sys/socket.h>
#include <iostream>
#include <vector>
#include "./pkg/net/net.h"

using std::vector;

void dg_echo(int sockfd, struct sockaddr *sa, socklen_t salen) {

}

int main(int argc, char *argv[]) {
    ServerUDP s("127.0.0.1", "8080");

    vector<char> buf;
    buf.emplace_back('1');
    buf.emplace_back('2');
    buf.emplace_back('3');



    //s.SendTo(&buf, 0, );
}