/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorHandling.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/16 11:58:37 by zdnaya            #+#    #+#             */
/*   Updated: 2021/09/18 16:27:47 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef ERRORHANDLING_HPP
#define ERRORHANDLING_HPP

#include "tools.hpp"

#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <utility>
#include <iomanip>
#include <unistd.h>
#include <fcntl.h>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <iterator>
#include <unordered_map>
#include <ctype.h>
#include <stdexcept>
#define CYN "\e[0;36m"
#define RESET "\e[0m"

class ErrorHandling
{
private:
    unsigned int nbr_server;
    unsigned int nbr_location;
    unsigned int server;
    unsigned int location;

    std::map<int, std::string> map_s;
    char *file;
    std::unordered_multimap<std::string, std::string> tst;

public:
    ErrorHandling(char *file);
    void set_map(std::map<int, std::string> map_s);
    std::map<int, std::string> get_map_s();
    int get_nbr_server();
    int get_nbr_location();
    std::unordered_multimap<std::string, std::string> get_cmap(void);
    
    class FileNotFound : public std::exception
    {
        virtual const char *what() const throw();
    };
    class NoComment : public std::exception
    {
        virtual const char *what() const throw();
    };
    class StartWithServer : public std::exception
    {
        virtual const char *what() const throw();
    };
    class OpenBracket : public std::exception
    {
        virtual const char *what() const throw();
    };
    class CloseBracket : public std::exception
    {
        virtual const char *what() const throw();
    };
    class NewlineBracket: public std::exception
    {
        virtual const char *what() const throw();
    };
    class Oneserver : public std::exception
    {
        virtual const char *what() const throw();
    };
    class NumberBrackets : public std::exception
    {
        virtual const char *what() const throw();
    };
    ~ErrorHandling();
};

std::map<int, std::string> get_map(char *av);
std::map<int, std::string> clean_map(std::map<int, std::string> error_mp);

#endif