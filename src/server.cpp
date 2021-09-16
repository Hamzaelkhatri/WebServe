/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 14:27:10 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/16 12:17:31 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Webserv.hpp"

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
    std::string Methode = (stor.find("GET") != stor.end() ? "GET" : (stor.find("POST") != stor.end())? "POST" : (stor.find("DELETE") != stor.end())? "DELETE" : "UNKNOWN");
    std::string Path = stor[Methode];
    std::cout << "Host: " << Host << std::endl;
    std::cout << "Port: " << Port << std::endl;
    std::cout << "Methode: " << Methode << std::endl;
    std::cout << "Path: " << Path << std::endl;
    for (std::map<std::string, std::string>::iterator it = stor.begin(); it != stor.end(); ++it)
            std::cout << it->first << " --> " << it->second << std::endl;
    Response *response = new Response();
    for (it = servers.begin(); it != servers.end(); it++)
    {
        std::cout << YEL << "------------------------------------------------------" << reset << std::endl;
        std::cout << BLU << "\t\tServer INFO " << RED << indexOfServer << reset << std::endl;
        indexOfLocation = 0;

        /* Print Server DATA*/

        for (it3 = it->second.begin(); it3 != it->second.end(); ++it3)
            std::cout << GRN << "\t"<< it3->first << RED << " --> " << it3->second << reset << std::endl;

        std::cout << YEL << "------------------------------------------------------" << reset << std::endl;

        for (it2 = locations.begin(); it2 != locations.end(); it2++)
        {
            if (indexOfServer == it2->first)
            {
                location_tmp = _GetFirstLocation(it2);
                for (it4 = it2->second.begin(); it4 != it2->second.end(); it4++)
                {
                    if (indexOfLocation != std::stoi(it4->first.substr(0, it4->first.find(" "))))
                        std::cout << MAG << "\t\tlocation ---> " << it4->first.substr(0, it4->first.find(" ")) << RED << " " << location_tmp << reset << std::endl;
                    indexOfLocation = std::stoi(it4->first.substr(0, it4->first.find(" ")));
                    LocationContent.insert(std::pair<std::string, std::string>(it4->first, it4->second));
                    std::cout << it4->first.substr(it4->first.find(" ") + 1) << GRN << " ===> " << reset << it4->second << std::endl;
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
                            someString ="";
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

                                    _GetDataServers(p); 

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