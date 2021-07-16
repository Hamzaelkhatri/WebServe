#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <iomanip>
#include <iostream>
class config
{
private:
    std::string _filename;
    config();

public:
    config(std::string _filename);
    config(config const &src);
    class FileNotFoundException : std::exception
    {
        const char *what() const throw()
        {
            return ("config file not exists");
        }
    };
    config &operator=(config const &src);
    virtual ~config();
};

config::config(std::string filename) : _filename(filename)
{
}

//Constrator of copy
config::config(config const &src)
{
    *this = src;
}

//operator
config &config::operator=(config const &src)
{

    return (*this);
}
config::~config(){};
#endif