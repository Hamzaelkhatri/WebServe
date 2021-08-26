/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zainabdnayagmail.com <zainabdnayagmail.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/21 19:31:17 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/08/24 12:42:22 by zainabdnaya      ###   ########.fr       */
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
    std::map<int, Server_element> parsing;
    std::multimap<std::string, std::string> server_map;
    std::multimap<std::string, std::string> loc_map;
    std::map< int , std::multimap<std::string, std::string> > _server_map;
    std::map <int, std::multimap<std::string, std::string> > _loc_map;

public:
    Parsing( char *, int nbr_server, int nbr_location);
    ~Parsing();
};
#endif