/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zainabdnayagmail.com <zainabdnayagmail.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 14:27:10 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/08/19 10:51:28 by zainabdnaya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

Server::Server(std::unordered_multimap<std::string, std::string> mp)
{
    std::memset((char *)&this->add, 0, sizeof(this->add)); 
//         this->socket_fd = socket(AF_INET, SOCK_STREAM,0);
//     if(this->socket_fd < 0)
//         error_msg("Error: you can't create socket");
//     add.sin_port = htons(5003);
//     add.sin_family = AF_INET;
//     add.sin_addr.s_addr = htonl(INADDR_ANY);
//     if(bind(this->socket_fd, (struct sockaddr *)&add, sizeof_add) < 0)
//         error_msg("Error: you can't bind socket");
    
//      if (listen(socket_fd, SOMAXCONN)< 0) // SOMAXCONN is the maximum number of pending connections that can be queued up before connections are refused.
//       {
//         std::cerr << "Listening failed" << std::endl;
//         exit(EXIT_FAILURE);
//       }
//        if ((new_socket = accept(socket_fd, (struct sockaddr *)&add, &sizeof(add)) < 0)
//     {
//           std::cerr << "acceptance failed" << std::endl;
//           exit(EXIT_FAILURE);
//     }
}

int Server::creatSocket_fd()
{
    this->socket_fd = socket(AF_INET, SOCK_STREAM,0);
    if(this->socket_fd < 0)
        error_msg("Error: you can't create socket");
    return(this->socket_fd);
}

void Server::set_strructAddr(struct sockaddr_in add)
{
    add.sin_port = htons(5003);
    add.sin_family = AF_INET;
    add.sin_addr.s_addr = htonl(INADDR_ANY);
}

int Server::get_sizeofAdd()
{
    return(sizeof(this->add));  // socklen_t size of adress
}


void Server::bind_socket(struct sockaddr_in add)
{
    if(bind(this->socket_fd, (struct sockaddr *)&add, get_sizeofAdd()) < 0)
            error_msg("Error: Binding socket failed");
}

void Server::listen_socket(int sockket_fd)
{
     if (listen(socket_fd, SOMAXCONN)< 0) // SOMAXCONN is the maximum number of pending connections that can be queued up before connections are refused.
      {
        std::cerr << "Listening failed" << std::endl;
        exit(EXIT_FAILURE);
      }
}

void Server::accept_socket(int socket_fd,struct sockaddr_in add)
{
    socklen_t size_add = sizeof(add);

    if ((new_socket = accept(socket_fd, (struct sockaddr *)&add, &size_add)) < 0)
    {
          std::cerr << "acceptance failed" << std::endl;
          exit(EXIT_FAILURE);
    }
}


Server::~Server()
{
}