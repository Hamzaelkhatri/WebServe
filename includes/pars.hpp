/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zainabdnayagmail.com <zainabdnayagmail.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/21 19:31:17 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/23 09:57:54 by zainabdnaya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARS_HPP
#define PARS_HPP
#include <iostream>
#include <map>
#include "ErrorHandling.hpp"
#include <dirent.h>

class LocationManage
{
private:
public:
};

class Server_element
{
private:
    int nb_port;
    int *port;

public:
    Server_element();
    void setPorts(int *ports);
    void GetPorts();
    void add_ports(int *port, int nb_port);
};

class Parsing
{
private:
    Server_element *server;
    char **env;
    std::map<int, Server_element> parsing;
    std::multimap<std::string, std::string> server_map;
    std::multimap<std::string, std::string> loc_map;
    std::map< int , std::multimap<std::string, std::string> > _server_map;
    std::multimap <int, std::multimap<std::string, std::string> > _loc_map;

public:
    Parsing( char *av);
    void set_env(char **env); 
    char **get_env();
    std::map< int , std::multimap<std::string, std::string> > GetServerMap();
    std::multimap <int, std::multimap<std::string, std::string> > Getloc_map();
    void set_serverMap(std::map< int , std::multimap<std::string, std::string> >  srv ,std::multimap <int, std::multimap<std::string, std::string> > loc);
    ~Parsing();
};
#endif