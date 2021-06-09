//
// Created by zz on 2021/6/6.
//

#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int set_stdin_noblock() {
    int oldop = fcntl(STDIN_FILENO, F_GETFL);
    int newop = oldop | O_NONBLOCK;
    fcntl(STDIN_FILENO, F_SETFL, newop);
    return oldop;
}

int main() {
    //set_stdin_noblock();
    char buf[50];
    memset(&buf, '\0', sizeof(buf));

    int n = read(STDIN_FILENO, buf, sizeof(buf));
    printf("read bytes: %d\n", n);
    printf("content: %s\n", buf);
}