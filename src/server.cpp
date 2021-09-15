/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 14:27:10 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/15 17:41:46 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Webserv.hpp"

int Server::_Accept_client(int sock)
{
    socklen_t client_len = sizeof(client);
    int addrlen = sizeof(client);
    csock = accept(sock, (struct sockaddr *)&client, (socklen_t *)&addrlen);
    if (csock != -1)
    {
        fcntl(csock, F_SETFL, O_NONBLOCK);
        FD_SET(csock, &masterfds);
        FD_SET(csock, &writefds);
        if (csock > maxfd)
            maxfd = csock;

        std::cout << csock << "\t  =  New connection" << std::endl;
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
    if ((n = recv(sock, buf, BUFFER_SIZE, 0)) > 0)
    {
        if (i == 0)
        {
            std::strcpy(this->request, buf);
            i++;
        }
        else
            std::strcat(this->request, buf);
    }
    return (n);
}

bool checkRequest(std::string &req)
{
    std::string data;
    size_t i;

    i = req.find("\r\n\r\n");
    if (i == std::string::npos)
        return false;
    if (req.find("Content-Length") != std::string::npos)
    {

        data = req.substr(i + 4);
        if (data.find("\r\n\r\n") == std::string::npos)
        {
            return false;
        }
    }
    // std::cout << req;
    return true;
}

void Server::_GetDataServers(Parsing *parsing)
{
    std::map<int, std::multimap<std::string, std::string>> servers = parsing->GetServerMap();
    std::multimap<int, std::multimap<std::string, std::string>> locations = parsing->Getloc_map();

    std::map<int, std::multimap<std::string, std::string>>::iterator it;
    std::multimap<int, std::multimap<std::string, std::string>>::iterator it2;

    //show data servers
    std::multimap<std::string, std::string>::iterator it3;
    std::multimap<std::string, std::string>::iterator it4;
    std::multimap<std::string, std::string> LocationContent;

    int indexOfServer = 1;
    int indexOfLocation = 1;
    for (it = servers.begin(); it != servers.end(); it++)
    {
        std::cout << YEL << "------------------------------------------------------"  << reset << std::endl;
        std::cout << BLU << "\t\t\tServer " << indexOfServer << reset << std::endl;
        std::cout << YEL << "------------------------------------------------------"  << reset << std::endl;
            indexOfLocation = 1;
        for (it2 = locations.begin(); it2 != locations.end(); it2++)
        {
            if (indexOfServer == it2->first)
            {
                for (it4 = it2->second.begin(); it4 != it2->second.end(); it4++)
                {
                    if (it4->first == "location") // We can change Result to what we need
                    {
                        std::cout << RED << "Location " << it4->second << " Number " << indexOfLocation << GRN << " : Ended" << reset << std::endl;
                        indexOfLocation++;
                    }
                    else
                    {
                        LocationContent.insert(std::pair<std::string, std::string>(it4->first, it4->second));
                        std::cout << it4->first << GRN << " ===> "<< reset << it4->second << std::endl;
                    }
                }
            }
        }
        indexOfServer++;
    }
}

Server::Server(Parsing *p, char *envp[])
{
    std::string version;
    this->sock = new Socket(p);
    maxfd = 0;
    int j = 0;
    int rc = 0;
    char buffer[BUFFER_SIZE + 1];
    cgi *c;
    loc = p->Getloc_map();
    _GetDataServers(p);
    exit(EXIT_SUCCESS);
    FD_ZERO(&masterfds);
    MasterSockets = this->sock->_Get_server_fds();
    int i = 0;
    while (i < this->sock->_Get_h())
    {
        FD_SET(MasterSockets[i], &masterfds);
        if (MasterSockets[i] > maxfd)
            maxfd = MasterSockets[i];
        i++;
    }
    FD_ZERO(&writefds);
    memcpy(&writefds, &masterfds, sizeof(masterfds));
    std::map<int, std::string> ips;
    int o = 0;
    while (1)
    {
        FD_ZERO(&readfds);
        memcpy(&readfds, &masterfds, sizeof(masterfds));
        struct timeval _tv = {30, 0};
        selected = select(maxfd + 1, &readfds, &writefds, NULL, &_tv);
        if (selected < 0)
        {
            perror("select");
            exit(1);
        }
        else
            for (int sock_fd = 0; sock_fd <= maxfd; sock_fd++)
            {
                if (FD_ISSET(sock_fd, &readfds))
                {
                    int newCnx = 0;
                    for (int j = 0; j < MasterSockets.size(); j++)
                    {
                        if (sock_fd == MasterSockets[j])
                        {
                            newCnx = 1;
                            break;
                        }
                    }
                    if (newCnx)
                        _Accept_client(sock_fd);
                    else
                    {
                        bzero(buffer, BUFFER_SIZE);
                        rc = recv(sock_fd, buffer, BUFFER_SIZE, 0);
                        if (rc > 0)
                        {
                            std::map<int, std::string>::iterator it = _clients.find(sock_fd);
                            buffer[rc] = '\0';
                            if (it != _clients.end())
                                it->second += buffer;
                            if (checkRequest(it->second) == true)
                            {
                                someString = it->second;
                                if (FD_ISSET(sock_fd, &writefds))
                                {
                                    std::stringstream ss(someString);
                                    int t = 0;
                                    while (std::getline(ss, line1, '\n'))
                                    {
                                        if (line1.find_first_of(":") != std::string::npos && t == 1)
                                        {
                                            tmp1 = line1.substr(0, line1.find_first_of(":"));
                                            tmp2 = line1.substr(line1.find_first_of(":") + 1);
                                            stor[tmp1] = tmp2;
                                        }
                                        else if (t == 0)
                                        {
                                            tmp1 = line1.substr(0, line1.find_first_of(" "));
                                            tmp2 = line1.substr(line1.find_first_of(" ") + 1);
                                            stor[tmp1] = "webpage" + tmp2.substr(0, tmp2.find_first_of(" "));
                                            t++;
                                        }
                                        else
                                        {
                                            t++;
                                            Content.push_back(line1);
                                        }
                                        i++;
                                    }
                                    std::map<int, std::multimap<std::string, std::string>> tmp = p->GetServerMap();
                                    std::map<int, std::multimap<std::string, std::string>>::iterator sp;
                                    std::multimap<std::string, std::string>::iterator sp2;
                                    if (stor.find("GET") != stor.end())
                                        Get_methode(c, envp);
                                    else if (stor.find("POST") != stor.end())
                                        Post_methode();
                                    else if (stor.find("DELETE") != stor.end())
                                        Delete_methode();
                                    status = "200 OK";
                                    version = "HTTP/1.1 ";
                                    std::string header = version + status + "\nContent-type: text/html; charset=UTF-8\nContent-Length: " + std::to_string(body.length()) + "\n\n" + body;
                                    write(sock_fd, header.c_str(), strlen(header.c_str()));
                                }
                            }
                        }
                        else
                        {
                            if (rc == 0)
                            {
                                std::cout << sock_fd << "\t  =   Diconnected" << std::endl;
                                close(sock_fd);
                                FD_CLR(sock_fd, &masterfds);
                                FD_CLR(sock_fd, &writefds);
                            }
                            else
                                continue;
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