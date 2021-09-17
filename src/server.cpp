/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 14:27:10 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/17 17:43:22 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Webserv.hpp"
#include "../includes/request.hpp"

std::string _GetFirstLocation(std::multimap<int, std::multimap<std::string, std::string> >::iterator locations)
{
    std::multimap<int, std::multimap<std::string, std::string> >::iterator it;
    std::multimap<std::string, std::string>::iterator it2;

    for (it2 = locations->second.begin(); it2 != locations->second.end(); ++it2)
    {
        if (it2->first.find("location") != std::string::npos)
        {
            return (it2->second);
        }
    }
    return (std::string(""));
}

bool CheckRightServer(std::multimap<std::string, std::string>::iterator it3, Request *request, std::map<int, std::multimap<std::string, std::string> >::iterator it)
{
    int TargetServer = 0;
    if (it3->first.find("listen") != std::string::npos && request->get_port().find(it3->second) == std::string::npos) // 8011 != 8060
        return (false);
    else
    {
        if (it3->first.find("server_addr") != std::string::npos && request->get_host().find(it3->second) != std::string::npos)
            TargetServer += 1;
        else if (it3->first.find("server_name") != std::string::npos && request->get_host().find(it3->second) != std::string::npos)
            TargetServer += 1;
    }
    return (TargetServer == 1 ? true : false);
}

void Server::_GetDataServers(Parsing *parsing)
{
    std::map<int, std::multimap<std::string, std::string> > servers = parsing->GetServerMap();
    std::multimap<int, std::multimap<std::string, std::string> > locations = parsing->Getloc_map();
    std::map<int, std::multimap<std::string, std::string> >::iterator it;
    std::multimap<int, std::multimap<std::string, std::string> >::iterator it2;
    //show data servers
    std::multimap<std::string, std::string>::iterator it3;
    std::multimap<std::string, std::string>::iterator it4;
    std::multimap<std::string, std::string> LocationContent;
    int indexOfServer = 1;
    int indexOfLocation = 1;
    std::string location_tmp = _GetFirstLocation(locations.begin());
    /*
        simulation Of data from Server Request
    */
    std::string Host = stor["Host"].substr(0, stor["Host"].find(":"));
    std::string Port = stor["Host"].substr(stor["Host"].find(":") + 1, stor["Host"].size());
    std::string Methode = (stor.find("GET") != stor.end() ? "GET" : (stor.find("POST") != stor.end()) ? "POST"
                                                                : (stor.find("DELETE") != stor.end()) ? "DELETE"
                                                                                                      : "UNKNOWN");
    std::string Path = stor[Methode];

    Request *request = new Request();
    request->set_host(Host);
    request->set_port(Port);
    request->set_method(Methode);
    request->set_path(Path);

    Response *response = new Response();
    int TargetServer = 0;
    for (it = servers.begin(); it != servers.end(); it++)
    {
        indexOfLocation = 0;
        TargetServer = 0;
        for (it3 = it->second.begin(); it3 != it->second.end(); ++it3)
        {
            if (it3->first.find("listen") != std::string::npos && request->get_port().find(it3->second) != std::string::npos)
                TargetServer -= 1;
            if (it3->first.find("server_addr") != std::string::npos && request->get_host().find(it3->second) != std::string::npos)
                TargetServer -= 1;
            else if (it3->first.find("server_name") != std::string::npos && request->get_host().find(it3->second) != std::string::npos)
                TargetServer -= 1;
            if (TargetServer <= -2)
            {
                TargetServer = indexOfServer;
                break;
            }
        }

        for (it2 = locations.begin(); it2 != locations.end(); it2++)
        {
            if (it2->first == TargetServer)
            {
                location_tmp = _GetFirstLocation(it2);
                for (it4 = it2->second.begin(); it4 != it2->second.end(); it4++)
                {
                    std::cout << it2->first << " "
                              << " " << it4->first << it4->second << std::endl;
                }
            }
        }
        indexOfServer++;
        std::cout << YEL << "***********************************" << reset << std::endl;
    }
    exit(0);
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

    /*
        Print Config Data 
    */
    /* 
        Warning Of Exit
    */

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
            throw(SelectFailed());
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
                            std::string str1(inet_ntoa(this->sock->_Get_addr()[j].sin_addr));
                            std::string str0 = ":" + std::to_string(ntohs(this->sock->_Get_addr()[j].sin_port));
                            std::string str = str1 + str0;
                            ips.insert(std::pair<int, std::string>(j, str));
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
                            someString = "";
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
                                            stor[tmp1] = tmp2.substr(0, tmp2.find_first_of(" "));
                                            t++;
                                        }
                                        else
                                        {
                                            t++;
                                            Content.push_back(line1);
                                        }
                                        i++;
                                    }

                                    //Show Stor
                            std::cout  << someString << std::endl;
                                    // _GetDataServers(p);

                                    // std::map<int, std::string>::iterator op = ips.begin();
                                    // for(op = ips.begin(); op != ips.end(); op++)
                                    // // {std::cout << op->first << " ==> " << op->second << std::endl;}
                                    // witch_server(ips, p);
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
                                    // loc = p->Getloc_map();
                                    // if (stor.find("GET") != stor.end())
                                    //     Get_methode(c, envp);
                                    // else if (stor.find("POST") != stor.end())
                                    //     Post_methode();
                                    // else if (stor.find("DELETE") != stor.end())
                                    //     Delete_methode();
                                    status = "200 OK";
                                    version = "HTTP/1.1 ";
                                    body = "<html><head><title>Web Server</title></head><body><h1>Web Server</h1><p>Hello World</p></body></html>";
                                    std::string header = version + status + "\nContent-type: text/html; charset=UTF-8\nContent-Length: " + std::to_string(body.size()) + "\n\n" + body;
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