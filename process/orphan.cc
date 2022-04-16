//
// Created by zz on 2022/3/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main() {
    pid_t pid;
    // 创建一个进程
    pid = fork();
    // 创建失败
    if (pid < 0) {
        perror("fork error:");
        exit(1);
    }
    // 子进程
    if (pid == 0) {
        printf("[child] I am the child process.\n");
        // 输出进程 ID 和父进程 ID
        printf("[child] pid: %d\tppid:%d\n", getpid(), getppid());
        printf("[child] I will sleep five seconds.\n");
        // 睡眠 5s，保证父进程先退出
        sleep(5);
        // 此时父进程已经结束，但子进程还没结束，所以成为孤儿进程
        // 再次查看子进程此时的父进程
        printf("[child] pid: %d\tppid:%d\n", getpid(), getppid());
        printf("[child] child process is exited.\n");
        exit(0);
    } else { // 父进程
        printf("[father] I am father process.\n");
        // 父进程睡眠 1s，保证子进程输出进程 id
        // 父进程先于子进程结束，使得子进程成为孤儿进程
        sleep(1);
        printf("[father] father process is exited.\n");
        exit(0);
    }
    // Output:
    // [father] I am father process.
    // [child] I am the child process.
    // [child] pid: 19422	ppid:19421
    // [child] I will sleep five seconds.
    // [father] father process is exited.
    // [child] pid: 19422	ppid:1   此时 child 的 ppid 为 1，说明已经被 init 进程接管
    // [child] child process is exited.
}
