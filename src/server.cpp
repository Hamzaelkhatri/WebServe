/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 14:27:10 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/08/28 17:56:27 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"
#include <string>
Server::Server(Parsing *p)
{
    std::memset((char *)&this->add, 0, sizeof(this->add));
    int server_fd;  // socket descriptor, an integer!
    int new_socket; // conection establish btw client & server
    int valrecv;    // communication part
    struct sockaddr_in add;
    struct sockaddr_in client;
    socklen_t size_client = sizeof(client); // socklen_t size of adress

    // ** CREATE SOCKET**/
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
    {
        std::cerr << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    // bind an IP add and a port to a  socket
    //   p->GetServerMap().find()
    std::map<int, std::multimap<std::string, std::string>> tmp = p->GetServerMap();
    std::multimap<std::string, std::string> mtmp = tmp[1];

    add.sin_port = htons(std::stoi(mtmp.find("listen")->second));
    add.sin_family = AF_INET;
    add.sin_addr.s_addr = inet_addr(mtmp.find("server_addr")->second.c_str());
    ;

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
    while (1)
    {
        std::cout << "\t\t\t Listening " << mtmp.find("listen")->second << std::endl;
        if ((new_socket = accept(server_fd, (struct sockaddr *)&client,
                                 &size_client)) < 0)
        {
            std::cerr << "acceptance failed" << std::endl;
            exit(EXIT_FAILURE);
        }
        char buffer[30000] = {0};
        valrecv = recv(new_socket, buffer, 30000, 0);

        std::string tmp;
        std::string index ="";

        std::ifstream MyReadFile("webpage/index.html");
        while (std::getline(MyReadFile, tmp))
        {
            index+= tmp;
            index+= "\n";
        }
        MyReadFile.close();
        int lenght = index.size();
        std::string header = "HTTP/1.1 301 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(lenght) + "\n\n" + index;
        write(new_socket, header.c_str(), strlen(header.c_str()));
        std::cout << "------------------------------------------------------" << std::endl;
    }
    close(new_socket);
    close(server_fd);
}

int Server::creatSocket_fd()
{
    this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket_fd < 0)
        error_msg("Error: you can't create socket");
    return (this->socket_fd);
}

void Server::set_strructAddr(struct sockaddr_in add)
{
    add.sin_port = htons(5003);
    add.sin_family = AF_INET;
    add.sin_addr.s_addr = htonl(INADDR_ANY);
}

int Server::get_sizeofAdd()
{
    return (sizeof(this->add)); // socklen_t size of adress
}

void Server::bind_socket(struct sockaddr_in add)
{
    if (bind(this->socket_fd, (struct sockaddr *)&add, get_sizeofAdd()) < 0)
        error_msg("Error: Binding socket failed");
}

void Server::listen_socket(int sockket_fd)
{
    if (listen(socket_fd, SOMAXCONN) < 0) // SOMAXCONN is the maximum number of pending connections that can be queued up before connections are refused.
    {
        std::cerr << "Listening failed" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Server::accept_socket(int socket_fd, struct sockaddr_in add)
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