//
// Created by root on 7/18/21.
//

#include "../pkg/net/net.h"

// > lsof -i tcp:8080
// COMMAND   PID USER   FD   TYPE DEVICE SIZE/OFF NODE NAME
// accept  12710 root    3u  IPv4 248860      0t0  TCP *:http-alt (LISTEN)
// accept  12711 root    3u  IPv4 248860      0t0  TCP *:http-alt (LISTEN)
// accept  12712 root    3u  IPv4 248860      0t0  TCP *:http-alt (LISTEN)
// accept  12713 root    3u  IPv4 248860      0t0  TCP *:http-alt (LISTEN)
// accept  12714 root    3u  IPv4 248860      0t0  TCP *:http-alt (LISTEN)
//
//
int main() {
    Server s("tcp", "0.0.0.0", "8080");
    s.Listen(100);

    for (int i = 0; i < 5; i++) {
        int pid = fork();
        if (pid == 0) {  // father
            while (true) {
                auto conn = s.Accept();
                printf("accept PID is %d\n", getpid());
                vector<char> buf;
                buf.emplace_back('1');
                conn->Write(buf, 0);
                printf("process %d accept success!\n", getpid());
            }
        }
    }
}