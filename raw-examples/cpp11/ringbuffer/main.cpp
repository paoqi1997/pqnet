#include <cstdio>
#include <iostream>
#include <string>

#include "ringbuffer.h"

using std::cout;
using std::endl;

template <typename T>
void print_info(T val) {
    std::printf("obj: %s(%zu)\n", typeid(val).name(), sizeof(val));
}

template <>
void print_info(std::string s) {
    std::printf("obj: std::string(%zu)\n", s.length());
}

template <>
void print_info(std::string& s) {
    std::printf("obj: std::string&(%zu)\n", s.length());
}

template <>
void print_info(const char *s) {
    std::printf("obj: const char*(%zu)\n", std::strlen(s));
}

template <>
void print_info(char *s) {
    std::printf("obj: char*(%zu)\n", std::strlen(s));
}

int main()
{
    RingBuffer rb;

    std::printf("rIdx: %zu, wIdx: %zu\n", rb.rIndex(), rb.wIndex());

    int x = 16;
    print_info(x);

    rb.appendInt32(x);
    cout << rb.getInt32() << endl;

    std::printf("rIdx: %zu, wIdx: %zu\n", rb.rIndex(), rb.wIndex());

    std::string s(512, 'a');
    print_info(s);

    rb.append(s.c_str(), s.length());
    std::printf("rIdx: %zu, wIdx: %zu\n", rb.rIndex(), rb.wIndex());

    cout << rb.get(s.length()) << endl;
    std::printf("rIdx: %zu, wIdx: %zu\n", rb.rIndex(), rb.wIndex());

    s.assign(768, 'c');

    rb.append(s.c_str(), s.length());
    std::printf("rIdx: %zu, wIdx: %zu\n", rb.rIndex(), rb.wIndex());

    cout << rb.get(s.length()) << endl;
    std::printf("rIdx: %zu, wIdx: %zu\n", rb.rIndex(), rb.wIndex());

    return 0;
}
