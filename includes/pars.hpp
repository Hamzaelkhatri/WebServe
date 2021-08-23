/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zainabdnayagmail.com <zainabdnayagmail.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/21 19:31:17 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/08/22 19:06:37 by zainabdnaya      ###   ########.fr       */
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
    std::multimap<std::string, std::string> pars_map;

public:
    Parsing(std::map<int, std::string>, int nbr_server, int nbr_location);
    ~Parsing();
};
#endif