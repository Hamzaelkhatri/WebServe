/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 14:27:10 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/15 19:34:17 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Webserv.hpp"

int  Server::_Accept_client(int sock)
{
    socklen_t client_len = sizeof(client);
    int addrlen = sizeof(client);
    csock = accept(sock, (struct sockaddr *)&client, (socklen_t *)&addrlen);
    if(csock !=  -1)
    {
        fcntl(csock, F_SETFL, O_NONBLOCK);
        FD_SET(csock, &masterfds);
        FD_SET(csock, &writefds);
        if (csock > maxfd)
            maxfd = csock;
            
        std::cout <<  csock <<  "\t  =  New connection" << std::endl;
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
    if((n = recv(sock, buf, BUFFER_SIZE, 0)) > 0)
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
    if (req.find("Content-Length") != std::string::npos) {

        data = req.substr(i + 4);
        if (data.find("\r\n\r\n") == std::string::npos) {
            return false;
        }
    }
    // std::cout << req;
    return true;
}


Server::Server(Parsing *p,char *envp[])
{
    std::string version;
    this->sock = new Socket(p);
    maxfd = 0;
    int j = 0;
    int rc= 0;
    char   buffer[BUFFER_SIZE+1];
    cgi *c;
    loc = p->Getloc_map();

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
            for (int sock_fd = 0; sock_fd <= maxfd ; sock_fd++)
            {
                if(FD_ISSET(sock_fd, &readfds))
                {
                    int newCnx = 0;
                    for(int j = 0; j < MasterSockets.size() ; j++)
                    {
                        if(sock_fd == MasterSockets[j])
                        {
                            std::string str1(inet_ntoa(this->sock->_Get_addr()[j].sin_addr));
                            std::string str0 = ":" + std::to_string(ntohs(this->sock->_Get_addr()[j].sin_port));
                            std::string str = str1 +   str0  ;
                            ips.insert(std::pair<int, std::string>(j, str));
                            newCnx = 1;
                            break ;
                        }
                    }
                    if (newCnx)
                        _Accept_client(sock_fd);
                    else
                    {
                        bzero(buffer, BUFFER_SIZE);
                        rc = recv(sock_fd, buffer, BUFFER_SIZE, 0);
                        if(rc > 0)
                        {
                            std::map<int, std::string>::iterator it = _clients.find(sock_fd);
                            buffer[rc] = '\0';
                            if (it != _clients.end())
                                it->second += buffer;
                            if (checkRequest(it->second) == true)
                            {
                                someString = it->second;
                                if(FD_ISSET(sock_fd, &writefds))
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
                                    // std::map<int, std::string>::iterator op = ips.begin();
                                    // for(op = ips.begin(); op != ips.end(); op++)
                                    // {std::cout << op->first << " ==> " << op->second << std::endl;}
                                    witch_server(ips,p);
                                    // std::map< int , std::multimap<std::string, std::string> > tmp = p->GetServerMap();
                                    // std::map< int , std::multimap<std::string, std::string> >::iterator  sp;
                                    // std::multimap<std::string, std::string>::iterator sp2;
                                    // for(sp = tmp.begin(); sp!= tmp.end(); sp++)
                                    // {
                                    //     std::cout << sp->first << std::endl;
                                    //     for(sp2 = sp->second.begin(); sp2 != sp->second.end(); sp2++)
                                    //     {
                                    //         if(sp2->first == "listen")
                                    //         {
                                    //         std::cout <<   sp2->first <<  "==>"<< sp2->second << std::endl;
                                    //         std::cout << " Host :" << stor["Host"] << std::endl;
                                    //             // if(sp2->second.find(stor["Host"]) != std::string::npos)
                                    //             //     {
                                    //             //         std::cout <<  sp->first << "==>" << sp2->second << std::endl;
                                    //             //         break;
                                    //             //     }
                                                
                                    //         }
                                    //         if(sp2->first == "server_addr")
                                    //         {
                                    //             std::cout <<   sp2->first <<  "==>"<< sp2->second << std::endl;
                                    //             std::cout << " Host :" << stor["Host"] << std::endl;
                                    //         }
                                    //     }
                                    //     std::cout << "" << std::endl;
                                    // }
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
                            if(rc == 0)
                            {
                                std::cout << sock_fd <<  "\t  =   Diconnected" << std::endl;
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