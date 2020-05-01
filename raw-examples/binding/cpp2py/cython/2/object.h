#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <string>

namespace mypy
{

inline int _add(int x, int y)
{
    return x + y;
}

class CObject
{
public:
    CObject() { std::cout << "CObject::CObject" << std::endl; }
    ~CObject() { std::cout << "CObject::~CObject" << std::endl; }
    std::string getName() const { return name; }
    int getSum() const { return sum; }
    void setName(const std::string& _name) { name = _name; }
    void setSum(int _sum) { sum = _sum; }
private:
    std::string name;
    int sum;
};

} // namespace mypy

#endif // OBJECT_H
