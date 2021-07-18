#ifndef CONFIG_HPP
#define CONFIG_HPP
#include <iomanip>
#include <iostream>
#include <iterator>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <map>

// Colors Desfines
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define RESET "\033[0m"

class config
{
private:
    std::string _filename;
    std::map<int, std::string> servers;
    std::vector<std::string> _FileVector;
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
    void getServers();
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
    std::string str;
    int num_bracket = 0;
    int index = 0;
    int num_line = 0;
    int num_column = 0;
    std::ifstream myfile;

    myfile.open(filename.c_str());
    //check if file exists
    if (!myfile)
        std::cout << "config file not exists \n";
    //cout number of brackets

    while (std::getline(myfile, str))
    {
        if (str.find("{") != std::string::npos || str.find("}") != std::string::npos)
        {
            num_line = index;
            num_column = str.find("}") != std::string::npos ? str.find("}") : str.find("{");
            num_bracket = str.find("{") != std::string::npos ? num_bracket + 1 : num_bracket - 1;
        }
        index++;
        _FileVector.push_back(str);
    }
    //check if bracket is odd or even
    if (num_bracket)
        std::cerr << "block not close line " << filename << ":" << index << ":" << num_column << "\n";
    else
        getServers();
    myfile.close();
}

int check_words(std::string word, std::string to_find)
{
    int i = 0;
    std::string tmp = "";
    while (i < word.size())
    {
        if (!std::iswspace(word[i]) && word[i] != '{' && word[i] != '\n')
            tmp += word[i];
        else if (tmp == to_find)
            return (1);
        i++;
    }
    return (0);
}

void config::getServers()
{
    std::string res = "";
    int IndexServer = 0;
    std::vector<std::string>::iterator it = _FileVector.begin();
    int d = 0;
    while (it != _FileVector.end())
    {
        std::string tmp = *it;
        if (!check_words(tmp, "server"))
        {
            res += tmp;
            res += "\n";
            // add to vector of string next part;
        }
        else
        {
            if (IndexServer >= 1)
            {
                this->servers.insert(std::pair<int, std::string>(IndexServer, res));
                res = "";
            }
            IndexServer++;
        }
        d++;
        it++;
    }
    this->servers.insert(std::pair<int, std::string>(IndexServer, res));
    std::map<int, std::string>::iterator mp = this->servers.begin();
    while (mp != servers.end())
    {
        std::cout << "[" << mp->second << "]"
                  << "\n";
        mp++;
    }
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