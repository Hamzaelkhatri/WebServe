/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket_steps.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 16:56:46 by zdnaya            #+#    #+#             */
/*   Updated: 2021/09/07 19:16:14 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

void Server::creatSocket_fd()
{
        // ** CREATE SOCKET**/
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
    {
        std::cerr << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    ///Set Socket
    //int setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_length);
    //it helps in reuse of address and port
    int buffsize = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &buffsize, sizeof(buffsize)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    // bind an IP add and a port to a  socket
    //   p->GetServerMap().find()
}

void Server::bind_listen()
{
    add.sin_port = htons(std::stoi(mtmp.find("listen")->second));
    add.sin_family = AF_INET;
    if (mtmp.find("server_addr")->second.c_str() != NULL)
        add.sin_addr.s_addr = inet_addr(mtmp.find("server_addr")->second.c_str());
    else
        add.sin_addr.s_addr = INADDR_ANY;
    memset(add.sin_zero, '\0', sizeof add.sin_zero); // why help to pad from sockaddr_in to sockaddr
    // Forcefully attaching socket to the PORT
    if (bind(server_fd, (struct sockaddr *)&add, sizeof(add)) < 0)
    {
        perror("Bind");
        std::cerr << "Bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    // wait for an incoming connection
    if (listen(server_fd, SOMAXCONN)) // SOMAXCONN is the maximum number of pending connections that can be queued up before connections are refused.
    {
        std::cerr << "Listening failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    sizeof_add = sizeof(add);
}
//  for one server
void Server::accept_socket()
{
    size_client = sizeof(client); 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&client,
                             &size_client)) < 0)
    {
        std::cerr << "acceptance failed" << std::endl;
        exit(EXIT_FAILURE);
    }

}

// void Server::accept_socket()
// {
//     // initialize the bit set before calling FD_SET() to set a socket as a member. ==>clear set
//     FD_ZERO(&readfds);
//     // add socket_fd to the set 
//     FD_SET(server_fd, &readfds);
//     nfds = server_fd;

    

// }