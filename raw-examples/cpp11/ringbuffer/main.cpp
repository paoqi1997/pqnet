#include <iostream>
#include <string>

#include "ringbuffer.h"

using std::cout;
using std::endl;

int main()
{
    RingBuffer rb;

    rb.appendInt32(16);
    cout << rb.getInt32() << endl;

    cout << rb.writableBytes() << endl;

    std::string s(512, 'a');

    rb.append(s.c_str(), s.length());
    cout << rb.get(s.length()) << endl;

    s.assign(768, 'c');

    rb.append(s.c_str(), s.length());
    cout << rb.get(s.length()).length() << endl;

    return 0;
}
