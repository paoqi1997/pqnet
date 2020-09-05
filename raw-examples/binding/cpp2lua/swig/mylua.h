#ifndef MYLUA_H
#define MYLUA_H

#include <iostream>
#include <string>

inline int add(int x, int y)
{
    return x + y;
}

class Object
{
public:
    Object() { std::cout << "Object::Object" << std::endl; }
    ~Object() { std::cout << "Object::~Object" << std::endl; }
    std::string getName() const { return name; }
    int getSum() const { return sum; }
    void setName(const std::string& _name) { name = _name; }
    void setSum(int _sum) { sum = _sum; }
private:
    std::string name;
    int sum;
};

#endif // MYLUA_H
