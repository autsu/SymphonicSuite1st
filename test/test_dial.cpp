//
// Created by zz on 2021/6/3.
//

#include "../pkg/net/net.h"
#include <vector>

using std::vector;

// g++ -std=c++11 -o test_dial test_dial.cpp ../pkg/net/net.cpp
int main() {
    Dial d("tcp");
    Conn c = d.Connect("0.0.0.0", "8080");

    vector<char> buf;
    int n = c.Read(buf, 0);
    cout << "recv bytes: " << n << endl;
    for (auto v : buf) {
        cout << "recv data: " << v << " ";
    }
    cout << endl;
}

