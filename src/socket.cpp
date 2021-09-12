/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 16:56:46 by zdnaya            #+#    #+#             */
/*   Updated: 2021/09/12 15:40:50 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/socket.hpp"

Socket::Socket()
{
    
}

Socket::Socket(Parsing *p)
{
    _set_h(p); //this number of sockets in  total
    set_socket();// creat and set all sockets
    _set_tmp(p); // server map
    _set_addr( addr_map(_get_tmp()),_get_tmp());
    // for(int i = 0; i < _Get_h(); i++)
    // {
    //     char *str = inet_ntoa(_Get_addr()[i].sin_addr);
    //     std::cout << "Socket " << i << ": " << str << ":" << ntohs(_Get_addr()[i].sin_port) << std::endl;
    // }
    _Set_Binding();
}




Socket::~Socket()
{
}