/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 14:27:10 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/08/29 17:58:02 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"
#include <string>
Server::Server(Parsing *p)
{
    std::map<std::string, std::string> stor;
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
    std::map<int, std::multimap<std::string, std::string> > tmp = p->GetServerMap();
    std::multimap<std::string, std::string> mtmp = tmp[1];

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
        std::string body = "";

        std::ifstream MyReadFile("webpage/index.html");
        while (std::getline(MyReadFile, tmp))
        {
            body += tmp;
            body += "\n";
        }
        MyReadFile.close();
        int lenght = body.size();
        std::cout << buffer << std::endl;
        std::cout << "/*******************************Store respond**********************/\n";
        std::string someString(buffer);
        std::stringstream out;

        out << someString;
        std::string line1;
        std::string first;
        char *str = ":";
        int i = 0;
        while (std::getline(out, line1))
        {
            // std::cout <<"["<< line1 <<"]"<< "\n";
            // printf("[%i,%s]\n",i,line1.c_str());
            if (i == 0)
                first = line1;
            else
            {
                if (line1 != std::string(""))
                {
                    char **res = ft_charSplit(line1.c_str(), str);
                    std::string key = res[0];
                    if (res[1])
                    {
                        std::string value = res[1];
                        std::cout << value << std::endl;
                    }
                    // stor[key] = value;
                    // for(int j = 0 ; j < 3; j++)
                    //         std::cout <<  " ==> " << res[j]  << std::endl;
                }
            }
            i++;
        }

        //\nSet-Cookie email=hamzaelkhatri1@gmail.com
        std::string header = "HTTP/1.1 301 OK\nContent-Type: text/html\nContent-Length: " + std::to_string(lenght) + "\n\n" + body;
        write(new_socket, header.c_str(), strlen(header.c_str()));
        // std::cout << "------------------------------------------------------" << std::endl;
        close(new_socket);
    }
    // std::map<std::string, std::string>::iterator it2;
    // for (it2 = stor.begin(); it2 != stor.end(); ++it2)
    // {
    //         std::cout << it2->first << " ==> " << it2->second << std::endl;
    // }
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