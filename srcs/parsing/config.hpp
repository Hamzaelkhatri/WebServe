#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <iomanip>
#include <iostream>
#include <iterator>
#include <fstream>
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
        const char *what() const throw();
    };
    config &operator=(config const &src);
    void check_bracket(std::string filename);
    virtual ~config();
};

const char *config::FileNotFoundException::what() const throw()
{
    return ("config file not exists");
}

// *********** check bracket ***************//
//                                          //
//      check if bracket in config file     //
//                                          //
// *****************************************//

void config::check_bracket(std::string filename)
{
    //assignment
    char c = 0;
    int num_bracket = 0;
    std::ifstream myfile;

    myfile.open(filename);
    //check if file exists
    if (!myfile)
        std::cout << "config file not exists \n";
    //cout number of brackets
    while (myfile.get(c))
    {
        if (c == '}' || c == '{')
            num_bracket++;
    }
    //check if bracket is odd or even
    if (num_bracket % 2 != 0)
        std::cout << "error : bracket not close";
    myfile.close();
}

config::config(std::string filename) : _filename(filename)
{
    check_bracket(filename);
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