/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/21 19:31:17 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/30 17:24:52 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARS_HPP
#define PARS_HPP
#include <iostream>
#include <map>
#include "ErrorHandling.hpp"
#include <dirent.h>
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"
#define reeset "\e[0m"

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
    char **env;
    std::map<int, Server_element> parsing;
    std::multimap<std::string, std::string> server_map;
    std::multimap<std::string, std::string> loc_map;
    std::map<int, std::multimap<std::string, std::string> > _server_map;
    std::multimap<int, std::multimap<std::string, std::string> > _loc_map;

public:
    Parsing(char *av);
    void set_env(char **env);
    char **get_env();
    std::map<int, std::multimap<std::string, std::string> > GetServerMap();
    std::multimap<int, std::multimap<std::string, std::string> > Getloc_map();
    void set_serverMap(std::map<int, std::multimap<std::string, std::string> > srv, std::multimap<int, std::multimap<std::string, std::string> > loc);
    ~Parsing();
};
#endif