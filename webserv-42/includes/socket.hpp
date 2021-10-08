/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 10:48:26 by zdnaya            #+#    #+#             */
/*   Updated: 2021/09/16 11:41:43 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include <sstream>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>
#include <regex>
#include <arpa/inet.h> 
#include "pars.hpp"
#include "ErrorHandling.hpp"
#include "cgi.hpp"

class Socket 
{
    private:
        int h;
        std::vector<int> server_fds;
        std::vector<struct sockaddr_in>  address;
        std::map<int, std::multimap<std::string, std::string> > tmp;
                  std::multimap<int, std::multimap<std::string, std::string> > loc;

    public:
        Socket();
        Socket(Parsing *p);
        void    set_socket();
        void _set_h(Parsing *p);
        void _set_tmp(Parsing *p);
        void _set_addr(std::map<int, std::string> ,std::map<int, std::multimap<std::string, std::string> >  );
        void  _Set_Binding();
        int     _Get_h();
        std::vector<struct sockaddr_in> _Get_addr();
        std::vector<int> _Get_server_fds();
        std::map<int, std::multimap<std::string, std::string> >  _get_tmp();

        class  CreatSocketFailed : public std::exception
        {
            virtual const char *what() const throw();
        };
        class SetSocketFailed : public std::exception
        {
            virtual const char *what() const throw();
        };
        class BindFailed : public std::exception
        {
            virtual const char *what() const throw();
        };
        class ListenFailed : public std::exception
        {
            virtual const char *what() const throw();
        };

        ~Socket();
};  
int nbr_sockets(Parsing *p);
std::map<int, std::string> addr_map(std::map<int, std::multimap<std::string, std::string> > tmp);
#endif