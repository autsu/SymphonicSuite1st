//
// Created by parallels on 6/14/21.
//

#include <sys/epoll.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdio.h>

// epoll api
int main() {
    // epoll_create 创建一个 epoll 实例，函数会返回一个指向 epoll 实例的描述符，在使用完
    // 毕后应该调用 close 关闭 epoll 实例。size 参数类似 map 的 capacity，标识 epoll 实
    // 例维护的描述符的数量。
    int epfd = epoll_create(100);

    // epoll_create1 与 epoll_create 相相似，但参数变成了 flags，size 则被忽略。
    // 这里的 flags 有一个可选项：EPOLL_CLOEXEC，EPOLL_CLOEXEC 表示在创建的描述
    // 符上设置 FD_CLOEXEC 标志, 即进程被替换时会关闭文件描述符
    epfd = epoll_create1(EPOLL_CLOEXEC);

    // 用于保存一个事件信息，之后会将信息添加到 epoll 中，让 epoll 对该事件进行监听
    struct epoll_event event{};
    event.events = EPOLLIN; // 监听的事件类型
    event.data.fd = STDIN_FILENO;   // 监听的文件描述符

    // 这里将长度设置为 5，代表每次最多可以接收 5 个就绪的 fd
    struct epoll_event events[5];
    // 添加事件到 epoll 中
    // 除了 EPOLL_CTL_ADD，还有 DEL (代表删除) MOD（代表修改）
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event);

    printf("wait event... \n");
    // 阻塞等待，直到有事件就绪，返回就绪的事件数量，同时就绪的事件都保存在 events 中
    int rd = epoll_wait(epfd, events, 5, -1);
    printf("%d events is already! \n", rd);

    // 遍历所有的就绪事件
    for (int i = 0; i < rd; ++i) {
        int fd = events[i].data.fd;  // 从 events 中取出事件信息
        char buf[1024];
        memset(buf, '\0', 1024);

        read(fd, buf, 1024);
        printf("%s \n", buf);
    }

    // Output:
    // wait event...
    // 123
    // 1 events is already!
    // 123
}