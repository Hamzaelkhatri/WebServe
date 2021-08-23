/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zainabdnayagmail.com <zainabdnayagmail.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/22 19:03:57 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/08/22 19:07:45 by zainabdnaya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "pars.hpp"

Server_element::Server_element()
{
    this->port = 0;
}

Parsing::Parsing(std::map<int, std::string> conf, int nbr_server, int nbr_location)
{
    int i = 0;
    int nbr = 1;
    std::cout << nbr_location << "|" << nbr_server << std::endl;
    while (i < conf.size())
    {
        if (conf[i] == "server")
        {
            i++;
            std::cout << "server" << nbr << std::endl;
            // while ()
            // {
            // std::cout << conf[i] << std::endl;
            // }
            while (i < conf.size() && conf[i] != "server")
            {
                // std::cout << conf[i] << std::endl;
                if (conf[i].find("listen") != std::string::npos)
                {
                    std::cout << conf[i] << std::endl;
                }
                i++;
            }
            i--;
            nbr++;
        }
        i++;
    }
}

Parsing::~Parsing()
{
}