/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zainabdnayagmail.com <zainabdnayagmail.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 14:27:10 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/12 23:14:46 by zainabdnaya      ###   ########.fr       */
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
    char buf[BUFFER_SIZE + 1];
    int n;
    int i = 0;
    while ((n = recv(csock, buf, BUFFER_SIZE, 0)) > 0)
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
        j = 0;
        for( i = 0; i < this->sock->_Get_h();i++)
        {
            if(FD_ISSET(this->sock->_Get_server_fds()[i], &readfds))
            {
                if(_Accept_client(this->sock->_Get_server_fds()[i]) == -1)
                {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                char *ip = inet_ntoa(client.sin_addr);
                std::cout << "New connection from\t" << ip << ":" << ntohs(client.sin_port) << std::endl;
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
                char buffer[BUFFER_SIZE + 1];
                int n;
                bzero(buffer, BUFFER_SIZE + 1);
                n = recv(sd, buffer, BUFFER_SIZE, 0);
                if (n == 0)
                {
                    i++;
                    close(sd);
                    continue;
                }
                if( n < 0)
                {
                    close (sd);
                    FD_CLR (sd, &readfds);
                    std::cout << "Client Desconnected " <<  inet_ntoa(client.sin_addr)<<  ":" << ntohs(client.sin_port)  << std::endl;
                }
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