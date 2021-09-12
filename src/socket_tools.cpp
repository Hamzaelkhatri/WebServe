/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_tools.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 12:23:28 by zdnaya            #+#    #+#             */
/*   Updated: 2021/09/12 14:14:32 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/socket.hpp"

int  nbr_sockets(Parsing *p)
{
    int h = 0;
    std::map<int, std::multimap<std::string, std::string> > all_servers = p->GetServerMap();
    std::map<int, std::multimap<std::string, std::string> >::iterator it0;
    std::multimap<std::string, std::string>::iterator it;
    for (it0 = all_servers.begin(); it0 != all_servers.end(); ++it0)
    {
        for (it = it0->second.begin(); it != it0->second.end(); ++it)
        {
            if (it->first == "listen")
                h++;
        }
    }
    return(h);
}

std::map<int, std::string> addr_map(std::map<int, std::multimap<std::string, std::string> > tmp)
{
    int j = 0;
    int k = 0;
    std::map<int, std::string> mini;
    std::map<int, std::multimap<std::string, std::string> >::iterator it0;
    std::multimap<std::string, std::string>::iterator it;
    for (it0 = tmp.begin(); it0 != tmp.end(); ++it0)
    {
        j++;
        k = 0;
        for (it = it0->second.begin(); it != it0->second.end(); it++)
        {
            if (it->first == "server_addr")
            {
                k++;
                mini[j] = it->second;
            }
        }
        if (k == 0)
        {
            mini[j] = "INADDR_ANY";
        }
    }
    return(mini);
}




