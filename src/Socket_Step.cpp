/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket_Step.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 16:56:46 by zdnaya            #+#    #+#             */
/*   Updated: 2021/09/10 17:38:03 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

void Server::creatSocket_fd()
{
  int i = 0;
    server_fds = new int[h + 1];
    for (i = 0; i < h; i++)
    {
        server_fds[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fds[i] == 0)
        {
            std::cerr << "socket failed" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    for (i = 0; i < h; i++)
    {
        int buffsize = 1;
        if (setsockopt(server_fds[i], SOL_SOCKET, SO_REUSEADDR, &buffsize, sizeof(buffsize)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        std::memset((char *)&this->address[i], 0, sizeof(&this->address));
    }
}

void Server::bind_listen()
{
     std::map<int, std::multimap<std::string, std::string> >::iterator it0 = tmp.begin();
    std::multimap<std::string, std::string>::iterator it;
    std::map<int, std::string> mini;
    int j = 0;
    int k = 0;
    int  i = 0;
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
    i = 0;
    std::map<int, std::string>::iterator o = mini.begin();
    it0 = tmp.begin();
    address = new struct sockaddr_in[h + 1];
    while (it0 != tmp.end() && o != mini.end())
    {

        for (it = it0->second.begin(); it != it0->second.end(); it++)
        {
            if (it->first.find("listen") != std::string::npos)
            {
                std::string str = it->second;
                while (i < h && str != "NULL")
                {
                    address[i].sin_port = htons(std::stoi(str));
                    address[i].sin_family = AF_INET;
                    if (o->second == "INADDR_ANY")
                        address[i].sin_addr.s_addr = INADDR_ANY;
                    else
                        address[i].sin_addr.s_addr = inet_addr(o->second.c_str());
                    memset(address[i].sin_zero, '\0', sizeof address[i].sin_zero); // why help to pad from sockaddr_in to sockaddr

                    str = "NULL";
                    i++;
                }
            }
        }
        it0++;
        o++;
    }
    i = 0;
    /**********************Bind**************************************/
    while (i < h)
    {
        fcntl(server_fds[i], F_SETFL, O_NONBLOCK);
        // char *s = inet_ntoa(address[i].sin_addr);
        if (bind(server_fds[i], (struct sockaddr *)(&address[i]), sizeof(address[i])) < 0)
            perror("Bind");
        if (listen(server_fds[i],1000) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        i++;
    }
        for (i = 0; i < 1000; i++)
    {
        client_fds[i] = 0;
    }
}
void Server::accept_socket()
{
    // size_client = sizeof(client); 
    // if ((new_socket = accept(server_fd, (struct sockaddr *)&client,
    //                          &size_client)) < 0)
    // {
    //     std::cerr << "acceptance failed" << std::endl;
    //     exit(EXIT_FAILURE);
    // }

}
