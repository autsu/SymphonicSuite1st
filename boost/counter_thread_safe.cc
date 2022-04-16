#include <boost/noncopyable.hpp>
#include "../pkg/sync/Mutex.h"
#include <iostream>

class Counter : boost::noncopyable {
private:
    int64_t value_;
    // 用 mutable 申明，表示该变量即使在 const 函数中也允许修改
    // ps: const 函数表示不允许对当前对象的任何属性进行修改，但对象
    // 中可能有一些属性又需要更改，mutable 就是用来解决这个问题的
    mutable MutexLock mutex_;

public:
    Counter() : value_(0) {};

    ~Counter();

    int64_t value() const;

    int64_t getAndIncrease();
};

int64_t Counter::value() const {

}

int64_t Counter::getAndIncrease() {

}
