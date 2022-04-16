//
// Created by zz on 2022/3/22.
//

#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <iostream>

using std::cout;
using std::endl;

int main() {
    int ret = fork();
    if (ret == 0) { // child
        cout << "child process start, pid is: " << getpid() << endl;
        sleep(5);
        cout << "child process is done" << endl;
        exit(0);
    } else { // parent
        cout << "wait child" << endl;
        // 0 代表不设置任何 option，此时会阻塞等待子进程结束
        // waitpid 返回等待的子进程 pid，如果当前进程没有该指定的子进程，或者函数执行错误返回 -1
        int pid = waitpid(-1, nullptr, 0);
        cout << "waitpid return: " << pid << endl;

        // WNOHANG 非阻塞
        // 如果等待的子进程未结束返回 0
        // int ret_ = waitpid(-1, nullptr, WNOHANG);
        // cout << "waitpid WNOHANG return: " << ret_ << endl;

        // 等同于 waitpid(-1, nullptr, 0)
        //wait(nullptr);
        cout << "wait done" << endl;
    }
}
