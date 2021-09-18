#ifndef CGI_HPP
#define CGI_HPP
#include <iomanip>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdexcept>
#include "response.hpp"

class cgi
{
private:
public:
    std::string CGI(Response *r, char *envp[]);
    //  class  cgiFailed : public std::exception
    // {
    //     virtual const char *what() const throw();
    // };
};
#endif