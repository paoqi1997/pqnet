#ifndef MYPY_H
#define MYPY_H

#include <iostream>
#include <string>

class Object
{
public:
    Object() : flag(true) { std::cout << "Object::Object" << std::endl; }
    ~Object() { std::cout << "Object::~Object" << std::endl; }
    std::string getName() const { return name; }
    int getSum() const { return sum; }
    void setName(const std::string& _name) { name = _name; }
    void setSum(int _sum) { sum = _sum; }
public:
    bool getFlag() const { return flag; }
    void setFlag(bool _flag) { flag = _flag; }
private:
    std::string name;
    int sum;
    bool flag;
};

#endif // MYPY_H
