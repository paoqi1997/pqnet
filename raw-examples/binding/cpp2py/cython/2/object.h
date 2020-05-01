#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <string>

namespace mypy
{

inline int add(int x, int y)
{
    return x + y;
}

class CppObject
{
public:
    CppObject() : money(0) { std::cout << "CppObject::CppObject" << std::endl; }
    ~CppObject() { std::cout << "CppObject::~CppObject" << std::endl; }
    std::string getName() const { return name; }
    int getMoney() const { return money; }
    void setName(std::string& _name) { name = _name; }
    void setMoney(int _money) { money = _money; }
private:
    std::string name;
    int money;
};

} // namespace mypy

#endif // OBJECT_H
