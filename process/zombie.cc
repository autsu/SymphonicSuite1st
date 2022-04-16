//
// Created by zz on 2022/3/22.
//

#include <cstdio>
#include <unistd.h>
#include <errno.h>
#include <cstdlib>

int demo() {
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        perror("fork error:");
        exit(1);
    } else if (pid == 0) {
        printf("I am child process.I am exiting.\n");
        exit(0);
    }
    printf("I am father process.I will sleep two seconds\n");
    // 等待子进程先退出
    sleep(2);
    // 输出进程信息
    system("ps -o pid,ppid,state,tty,command");
    printf("father process is exiting.\n");
    return 0;
}

static void sigChild(int signo) {
    pid_t pid;
    // 处理僵尸进程，这里设置 WNOHANG 来保证不阻塞进程，同时使用 while 来回收尽可能多的
    // 僵尸进程，这涉及到了信号的排队问题，同一类型的信号最多只能有一个待处理信息，比如在瞬间
    // 有 5 个子进程结束，那么就会产生 5 个 SIGCHLD 信号，但是因为是瞬间同时产生的，那么就可能
    // 发生这种情况：第一个信号到来触发回调函数，在回调函数完成前剩下的四个信号也到来了，此时第二个
    // 会变为待处理状态，但是剩下的三个信号全部会被丢弃，如果下面的逻辑中使用 if，那么一次回调只能
    // 回收一个僵尸进程，导致有 3 个僵尸进程无法被回收，但是使用 while 便可在一次回调中处理多个
    // 僵尸进程，这样就可以解决信号丢弃的问题
    while ((pid = waitpid(-1, nullptr, WNOHANG)) > 0) {
        printf("child %d terminated.\n", pid);
    }
}

// 使用信号来解决僵尸进程问题
// 运行有时候会发生 [1]    27133 trace trap  ./zombie 错误，有时候又不会，
// 并且 sleep() 无效，函数会立马结束
int solveWithSignal() {
    pid_t pid;
    // 创建捕捉子进程退出信号
    signal(SIGCHLD, sigChild);
    // 创建 5 个子进程
    for (int i = 0; i < 5; ++i) {
        pid = fork();
        if (pid < 0) {
            perror("fork error");
            exit(1);
        } else if (pid == 0) {  // child
            printf("I am child process,pid id %d.I am exiting.\n", getpid());
            exit(0);
        }
    }
    printf("I am father process.I will sleep two seconds\n");
    // 等待子进程先退出
    sleep(10);
    // 输出进程信息
    //system("ps -o pid,ppid,state,tty,command");
    printf("father process is exiting.\n");
    return 0;
}

int main() {
    solveWithSignal();
}

