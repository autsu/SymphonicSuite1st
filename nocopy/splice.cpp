//
// Created by parallels on 6/9/21.
//

#include <fcntl.h>
#include <cassert>
#include "../pkg/net/net.h"

// g++ -o splice splice.cpp ../pkg/net/net.cpp
int main(int argc, char *argv[]) {
    if (argc <= 2) {
        cout << "usage: ./splice ip port" << endl;
        exit(0);
    }
    int ret;

    string ip = argv[1];
    string port = argv[2];

    Server server("tcp", ip, port);
    ret = server.Listen(5);
    if (ret < 0) {
        cout << "socket listen error" << endl;
    }

    Conn *conn = server.Accept();
    int pipefd[2];
    ret = pipe(pipefd);
    assert(ret != -1);

    //
    ret = splice(conn->get_fd(), nullptr, pipefd[1], nullptr, 32768,
                 SPLICE_F_MORE | SPLICE_F_MOVE);
    assert(ret != -1);

    ret = splice(pipefd[0], nullptr, conn->get_fd(), nullptr, 32768,
                 SPLICE_F_MORE | SPLICE_F_MOVE);
    assert(ret != -1);

    conn->Close();

    server.Close();
    return 0;
}
