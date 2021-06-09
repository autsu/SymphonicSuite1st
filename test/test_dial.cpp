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

//    vector<char> write_buf;
//    write_buf.emplace_back('1');
//    write_buf.emplace_back('2');
//    write_buf.emplace_back('3');
//
//    int n = c.Write(write_buf, 0);
//    cout << "write bytes: " << n << endl;

    vector<char> read_buf(1024, '\0');
    int n;
    n = c.Read(read_buf, 0);
    cout << "recv bytes: " << n << endl;
    string content;
    content.insert(content.begin(), read_buf.begin(), read_buf.end());
    cout << content << endl;

}

