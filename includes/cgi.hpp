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

#define die(e)                      \
    {                               \
        fprintf(stderr, "%s\n", e); \
        exit(EXIT_FAILURE);         \
    }

class cgi
{
private:
public:
    std::string CGI(char *argv[], char *envp[]);
        //  class  cgiFailed : public std::exception
        // {
        //     virtual const char *what() const throw();
        // };
};
#endif