#include <sys/epoll.h>

#include "../pkg/net/net.h"

#define DEFAULT_BUF_SIZE 4096

struct myData {
    int fd;
    string msg;
    void print() {
        cout << "fd: " << fd << ", msg: " << msg << endl;
    };
};

int main() {
    Server s("tcp", "0.0.0.0", "6666");
    int err = s.Listen(1024);
    if (err == -1) {
        cout << "listen error: " << strerror(errno) << endl;
        return 0;
    }
    cout << "listener socket fd: " << s.Sockfd() << endl;

    int epfd = epoll_create(10);
    epoll_event event;
    event.events = EPOLLIN;
    struct myData data = myData{
        fd : s.Sockfd(),
        msg : "accept event",
    };
    // 携带自己的数据
    event.data.ptr = &data;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, s.Sockfd(), &event);
    if (ret == -1) {
        cout << "epoll_ctl error: " << strerror(errno) << endl;
        return 0;
    }

    vector<epoll_event> epollEvents(128);
    char buf[DEFAULT_BUF_SIZE];
    memset(buf, '0', DEFAULT_BUF_SIZE);

    for (;;) {
        int n = epoll_wait(epfd, epollEvents.data(), epollEvents.size(), -1);
        if (n == -1) {
            cout << "epoll wait error: " << strerror(errno) << endl;
            break;
        }

        cout << n << "events" << endl;
        for (int i = 0; i < n; i++) {
            epoll_event event = epollEvents[i];
            auto data_ = (myData*)event.data.ptr;
            data_->print();
            if (data_->fd == s.Sockfd()) {
                Conn* conn = s.Accept();
                event.events = EPOLLIN;
                struct myData data = myData{
                    fd : conn->Connfd(),
                    msg : "conn read event",
                };
                event.data.ptr = &data;
                epoll_ctl(epfd, EPOLL_CTL_ADD, conn->Connfd(), &event);
            } else {
                auto fd = data_->fd;
                int n = read(fd, buf, DEFAULT_BUF_SIZE);
                cout << "read " << n << "bytes" << endl;
                if (n == 0) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, data_->fd, nullptr);
                    close(data_->fd);
                    printf("closed client: %d", data_->fd);
                } else {
                    printf("%s\n", buf);
                }
            }
        }
    }
}