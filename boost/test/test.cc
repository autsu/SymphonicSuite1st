//
// Created by zz on 2022/4/17.
//

#include "gtest/gtest.h"
#include <pthread.h>
#include <iostream>

TEST(test1, c1) {
    EXPECT_EQ(1, 1);
    EXPECT_EQ(1, 2);
}

void *threadFunc(void *) {
    std::cout << "hello" << std::endl;
}

TEST(test1, c2) {
    pthread_t t1, t2;
    pthread_create(&t1, nullptr, threadFunc, nullptr);
    printf("%lx\n", t1);    // 16b293000
    pthread_join(t1, nullptr);

    pthread_create(&t2, nullptr, threadFunc, nullptr);
    printf("%lx\n", t2); // 16b293000
    pthread_join(t2, nullptr);

    // 先后两个线程的标识符是一样的，因此 pthread_t 不适合用做程序中对线程的标识符
}

