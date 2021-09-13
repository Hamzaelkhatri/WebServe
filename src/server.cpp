/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zainabdnayagmail.com <zainabdnayagmail.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 14:27:10 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/13 22:59:19 by zainabdnaya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Webserv.hpp"

int  Server::_Accept_client(int sock)
{
    // socklen_t client_len = sizeof(client);
    csock = accept(sock, NULL, NULL);
    return (csock);
}

int Server::_Get_request(int sock)
{
    char buf[BUFFER_SIZE + 1];
    int n;
    int i = 0;
    bzero(buf, BUFFER_SIZE + 1);
    std::cout << "here\n";
    if((n = recv(sock, buf, BUFFER_SIZE, 0)) > 0)
    {
        // buf[n] = '\0';
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
    std::cout << this->request << std::endl;
    return (n);
}


Server::Server(Parsing *p,char *envp[])
{
    std::string version;
    this->sock = new Socket(p);
    maxfd = -1;
    csock = -1;
    int j = 0;
    int i = 0;
    int rc= 0;
    char   buffer[2048];
    MasterSockets = this->sock->_Get_server_fds();
    FD_ZERO(&masterfds);
    while( i < this->sock->_Get_h() )
    {
        maxfd = MasterSockets[i];
        FD_SET(MasterSockets[i], &masterfds);
        if(MasterSockets[i] > maxfd) 
            maxfd = MasterSockets[i];
        i++;
    }
    while (1)
    {
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        memcpy(&readfds, &masterfds, sizeof(masterfds));
        selected = select(maxfd + 1, &readfds, &writefds, NULL, NULL);
        for (i = 0; i <= maxfd && selected > 0 ; i++)
        {
            if(FD_ISSET(i, &readfds))
            {
                cnx = -1;
                for(int j = 0; j < MasterSockets.size() ; j++)
                {
                    if(i == MasterSockets[j])
                    {
                        cnx = _Accept_client(i);
                        if(cnx !=  -1)
                        {
                             FD_SET(cnx, &masterfds);
                             FD_SET(cnx, &writefds);
                            if (cnx > maxfd)
                                maxfd = cnx;
                            std::cout <<  cnx <<  "\t  =  New connection" << std::endl;
                        
                        }
                        else
                        {
                            perror("accept");
                            exit(EXIT_FAILURE);
                        }
                    }
                    else
                    { 
                        bzero(buffer, 2048);
                        rc = recv(i, buffer, sizeof(buffer), 0);
                        if (rc < 0)
                        {
                            fcntl(i, F_SETFL, O_NONBLOCK);
                            continue;
                        }
                        else if (rc == 0)
                        {
                            close(i);
                            FD_CLR(i, &readfds);
                            FD_CLR(i, &writefds);
                            std::cout <<  i <<  "\t  =   Diconnected" << std::endl;
                            break;
                        }
                        else
                        {
                            std::cout << "Received: " << buffer << std::endl;
                            if(FD_ISSET(i, &writefds))
                            {
                                status = "200 OK";
                                version = "HTTP/1.1 ";
                                std::string header = version + status + "\nContent-type: text/html; charset=UTF-8\nContent-Length:20\n\n hello World!";
                                write(i, header.c_str(), strlen(header.c_str()));
                            }
                        }
                      
                    }
                }
                
            }
        }
    }
}
Server::~Server()
{
    delete this->sock;
}