/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 14:27:10 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/14 14:42:51 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Webserv.hpp"

int  Server::_Accept_client(int sock)
{
    socklen_t client_len = sizeof(client);
    int addrlen = sizeof(client);
    csock = accept(sock, (struct sockaddr *)&client, (socklen_t *)&addrlen);
    // if(csock)
    if(csock !=  -1)
    {
    fcntl(csock, F_SETFL, O_NONBLOCK);
        FD_SET(csock, &masterfds);
        FD_SET(csock, &writefds);
        if (csock > maxfd)
            maxfd = csock;
        std::cout <<  csock <<  "\t  =  New connection" << std::endl;
        // break;
    }
    else
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    _clients.insert(std::pair<int, std::string>(csock, ""));
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

bool checkRequest(std::string &req)
{
    std::string data;
    size_t i;

    i = req.find("\r\n\r\n");
    if (i == std::string::npos)
    {
        return false;
    }
    if (req.find("Content-Length") != std::string::npos) {

        data = req.substr(i + 4);
        if (data.find("\r\n\r\n") == std::string::npos) {
            return false;
        }
    }
    return true;
}

Server::Server(Parsing *p,char *envp[])
{
    std::string version;
    this->sock = new Socket(p);
    maxfd = 0;
    int j = 0;
    int rc= 0;
    char   buffer[1024];
    FD_ZERO(&masterfds);
    MasterSockets = this->sock->_Get_server_fds();
    int i = 0;
    while(i < this->sock->_Get_h() )
    {
        FD_SET(MasterSockets[i], &masterfds);
        if (MasterSockets[i] > maxfd)
            maxfd = MasterSockets[i];
        i++;
    }
    FD_ZERO(&writefds);
    memcpy(&writefds, &masterfds, sizeof(masterfds));
    // writefds = masterfds;
    while (1)
    {
        FD_ZERO(&readfds);
        memcpy(&readfds, &masterfds, sizeof(masterfds));
        // readfds = masterfds;
        struct timeval _tv = {30, 0};
        selected = select(maxfd + 1, &readfds, &writefds, NULL, &_tv);
        if (selected < 0)
        {
            perror("select");
            exit(1);
        }
        else
            for (int sock_fd = 0; sock_fd <= maxfd ; sock_fd++)
            {
                if(FD_ISSET(sock_fd, &readfds))
                {
                    int newCnx = 0;
                    for(int j = 0; j < MasterSockets.size() ; j++)
                    {
                        if(sock_fd == MasterSockets[j])
                        {
                            newCnx = 1;
                            break ;
                        }
                    }
                    if (newCnx)
                    {
                            // fcntl(i, F_SETFL, O_NONBLOCK);
                        _Accept_client(sock_fd);
                    }
                    else
                    {
                        bzero(buffer, 1024);
                        while ((rc = recv(sock_fd, buffer, 1024, 0) > 0))
                        {
                            if(rc > 0)
                            {
                            std::map<int, std::string>::iterator it = _clients.find(sock_fd);
                            buffer[rc] = '\0';
                            if (it != _clients.end())
                            {
                                it->second += buffer;
                            }
                            if (checkRequest(it->second) == true)
                            {
                                std::cout << "Received: \n" << buffer << std::endl;
                                if(FD_ISSET(sock_fd, &writefds))
                                {
                                    status = "200 OK";
                                    version = "HTTP/1.1 ";
                                    std::string header = version + status + "\nContent-type: text/html; charset=UTF-8\nContent-Length:20\r\n\r\n hello World!";
                                    write(sock_fd, header.c_str(), strlen(header.c_str()));
                                }
                            }
                        }
                       else
                        {
                            std::cout << sock_fd <<  "\t  =   Disconnected" << std::endl;
                            close(sock_fd);
                            FD_CLR(sock_fd, &masterfds);
                            FD_CLR(sock_fd, &writefds);    
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