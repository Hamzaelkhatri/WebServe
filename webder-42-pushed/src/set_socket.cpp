/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_socket.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/12 14:17:15 by zdnaya            #+#    #+#             */
/*   Updated: 2021/09/16 12:16:45 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/socket.hpp"

void Socket::set_socket()
{
    int i ;
    for( i  = 0 ; i < _Get_h() ; i++)
    {
        server_fds.push_back(socket(AF_INET,SOCK_STREAM, 0));
        if (server_fds[i] == -1)
            throw(CreatSocketFailed());
        fcntl(server_fds[i], F_SETFL, O_NONBLOCK);
        int buffsize = 1;
        if (setsockopt(server_fds[i], SOL_SOCKET, SO_REUSEADDR, &buffsize, sizeof(buffsize)) == -1 )
            throw(SetSocketFailed());
    }
}


void Socket::_set_addr(std::map<int, std::string> mini,std::map<int, std::multimap<std::string, std::string> > tmp)
{
        struct sockaddr_in tmp_add;

        std::map<int, std::multimap<std::string, std::string> >::iterator it0;
        std::multimap<std::string, std::string>::iterator it;
        it0 = tmp.begin();
        std::map<int, std::string>::iterator o = mini.begin();
        while (it0 != tmp.end() && o != mini.end())
        {
            for (it = it0->second.begin(); it != it0->second.end(); it++)
            {
                if (it->first.find("listen") != std::string::npos)
                {
                    std::string str = it->second;
                    while (str != "NULL")
                    {
                        tmp_add.sin_port = htons(std::stoi(str));
                        tmp_add.sin_family = AF_INET;
                        if (o->second == "INADDR_ANY")
                            tmp_add.sin_addr.s_addr = INADDR_ANY;
                        else
                            tmp_add.sin_addr.s_addr = inet_addr(o->second.c_str());
                        memset(tmp_add.sin_zero, '\0', sizeof tmp_add.sin_zero); // why help to pad from sockaddr_in to sockaddr
    
                        str = "NULL";
                    }
                    address.push_back(tmp_add);
                }
            }
            it0++;
            o++;
        }
}

void Socket::_Set_Binding()
{
    for (int i = 0; i < _Get_h(); i++)
    {
        if (bind(server_fds[i], (struct sockaddr *)&address[i], sizeof(address[i])) == -1)
        {
            close(server_fds[i]);
            throw(BindFailed());
        }
        fcntl(server_fds[i], F_SETFL, O_NONBLOCK);
        if(listen(server_fds[i], 2048) == -1)
        {
            close(server_fds[i]);
            throw(ListenFailed());
        }
        fcntl(server_fds[i], F_SETFL, O_NONBLOCK);
        // std::cout << "listening on port " << ntohs(address[i].sin_port) << "" std::endl;
    }
}




void Socket::_set_tmp(Parsing *p)
{
    tmp = p->GetServerMap();
}

void    Socket::_set_h(Parsing *p)
{
    this->h = nbr_sockets(p);
}


