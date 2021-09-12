/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 14:27:10 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/12 18:43:10 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Webserv.hpp"

int  Server::_Accept_client(int sock)
{
    socklen_t client_len = sizeof(client);
    csock = accept(sock, (struct sockaddr *)&client, &client_len);
    return (csock);
}

int Server::_Get_request(int csock)
{
    char buf[BUFSIZ + 1];
    int n;
    int i = 0;
    while ((n = recv(csock, buf, BUFSIZ, 0)) > 0)
    {
        buf[n] = '\0';
        if (i == 0)
        {
            std::strcpy(this->request, buf);
            i++;
        }
        else
        {
            std::strcat(this->request, buf);
        }
    }
    std::cout << buf << std::endl;
    return (n);
}


Server::Server(Parsing *p,char *envp[])
{
    std::string version;
    this->sock = new Socket(p);
    maxfd = -1;
    csock = -1;
    int j = 0;
    while (1)
    {
        FD_ZERO(&readfds);
        int i = 0;
        while( i < this->sock->_Get_h() )
        {
            maxfd = this->sock->_Get_server_fds()[i];
            FD_SET(this->sock->_Get_server_fds()[i], &readfds);
            if(this->sock->_Get_server_fds()[i] > maxfd) 
                maxfd = this->sock->_Get_server_fds()[i];
            i++;
        }   
        if(select(maxfd + 1, &readfds, NULL, NULL, NULL) == -1)
        {
            perror("select");
            exit(EXIT_FAILURE);            
        }
        i = 0;
        j = 0;
        while( i < this->sock->_Get_h())
        {
            if(FD_ISSET(this->sock->_Get_server_fds()[i], &readfds))
            {
                if(_Accept_client(this->sock->_Get_server_fds()[i]) == -1)
                {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                char *ip = inet_ntoa(client.sin_addr);
                printf("New connection from %s\n", ip);
                clients.push_back(csock);
                break;
            }
            i++;
        }
        for(i = 0; i < clients.size(); i++)
            {
                maxfd = clients[i];
                FD_SET(clients[i], &readfds);
                if(clients[i] > maxfd)
                    maxfd = clients[i];
            }
        i = 0;
        while( i < clients.size())
        {
            sd = clients[i];
            if(FD_ISSET(sd, &readfds))
            {
                char buffer[BUFSIZ + 1];
                int n;
                bzero(buffer, BUFSIZ + 1);
                n = recv(sd, buffer, BUFSIZ, 0);
                if (n == 0)
                {
                    i++;
                    std::cout << "Client Desconnected" <<  inet_ntoa(client.sin_addr) << std::endl;
                    close(sd);
                    continue;
                }
                if( n == -1)
                {
                    i++;
                    continue;
                }
                std::cout << "sd = " << sd << std::endl;

                status = "200 OK";
                version = "HTTP/1.1 ";
                std::string header = version + status + "\nContent-type: text/html; charset=UTF-8\nContent-Length: 12\n\n" + "HELO WORLD";
                write(sd, header.c_str(), strlen(header.c_str()));
                close(sd);
            }
            fcntl(sd, F_SETFL, O_NONBLOCK);
            i++;
        }
    }
}
Server::~Server()
{
    delete this->sock;
}