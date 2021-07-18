#ifndef SHEMA_HPP
#define SHEMA_HPP
#include <iomanip>
#include <iostream>
class shema
{
private:
    std::string block_name;
    std::string block_description;
    shema();

public:
    virtual ~shema();
    shema(shema const &src);
    shema &operator=(shema const &src);
};
//Constrator of copy
shema::shema(shema const &src)
{
    *this = src;
}

//operator
shema &shema::operator=(shema const &src)
{
}
shema::~shema(){};
#endif