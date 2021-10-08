/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_sockets.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 14:16:50 by zdnaya            #+#    #+#             */
/*   Updated: 2021/09/15 10:44:00 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/socket.hpp"

std::vector<int> Socket::_Get_server_fds()
{
    return(this->server_fds);
}

std::vector<struct sockaddr_in> Socket::_Get_addr()
{
    return(this->address);
}

std::map<int, std::multimap<std::string, std::string> >  Socket::_get_tmp()
{
    return(this->tmp);
}

int     Socket::_Get_h()
{
    return(h);
}