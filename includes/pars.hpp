/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/21 19:31:17 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/17 19:56:54 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARS_HPP
#define PARS_HPP
#include <iostream>
#include <map>
#include "ErrorHandling.hpp"

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
    void set_env(char **env); 
    char **get_env();
    Parsing( char *av);
    std::map< int , std::multimap<std::string, std::string> > GetServerMap();
    std::multimap <int, std::multimap<std::string, std::string> > Getloc_map();
    ~Parsing();
};
#endif