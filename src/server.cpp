/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 14:27:10 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/10 19:22:45 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Webserv.hpp"

void Server::SetAll_FD()
{
        int i = 0;
    int count = 0;

    while (i < h)
    {
        nfds = server_fds[i];
        FD_SET(server_fds[i], &readfds);
        if(FD_ISSET(server_fds[i], &readfds))
            count++;
        if(nfds > server_fds[i])
            nfds = server_fds[i];
        i++;
    }
    for (i = 0; i < 1000; i++)
    {
        sd = client_fds[i];
        if (sd > 0)
            FD_SET(sd, &readfds);
        if(FD_ISSET( client_fds[i], &readfds))
            count++;
        if (sd > nfds)
            nfds = sd;
    }
    std::cout << "count "  << count << std::endl;
}

Server::Server(Parsing *p,char *envp[])
{
    int i ;
    h = calcul_liten(p->GetServerMap());
    cgi *c;
    loc = p->Getloc_map();
    tmp = p->GetServerMap();
    std::map<int, std::multimap<std::string, std::string> > tmp = p->GetServerMap();
    mtmp = tmp[1];
    creatSocket_fd();
    error = 0;
    bind_listen();

    while (1)
    {
        i = 0;
        FD_ZERO(&readfds);
        std::cout << "\t\t\t ---waiting for connection--- " << std::endl;
        SetAll_FD();
        timeval tv;
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        if (select(nfds + 1, &readfds, NULL, NULL,&tv) < 0)
        {
            perror("select");
            exit(EXIT_FAILURE);
        }      
        for (i = 0; i < h; i++)
        {
            if (FD_ISSET(server_fds[i], &readfds))
            {
                int addrlen = sizeof(client_add);
                std::cout << "i m waiting here2\n";
                int new_sd = accept(server_fds[i], (struct sockaddr *)&client_add, (socklen_t *)&addrlen);
                if (new_sd < 0)
                {
                    perror("accept");
                    continue;
                }
                for (int j = 0; j < 1000; j++)
                {
                    if (client_fds[j] == 0)
                    {
                        client_fds[j] = new_sd;
                        FD_SET(client_fds[j], &readfds);
                        break;
                    }
                }
                std::cout << "New connection accepted " << new_sd << std::endl;
            }
        }
        int n ;
        for (i = 0; i < 1000; i++)
        {
            int t = 0;
            sd = client_fds[i];
                fcntl(sd, F_SETFL, O_NONBLOCK);
            if (FD_ISSET(sd, &readfds))
            {
                std::cout << "The client " << sd << " connected"  << std::endl;
                char buffer[1000];
                bzero(buffer, 1000);
                n = recv(sd, (void *)&buffer, sizeof(buffer), 0);
                if (n < 0)
                    continue;
                if (n == 0)
                {
                    std::cout << "Client " << sd << " disconnected" << std::endl;
                    continue;
                }
                std::string str = buffer;
                someString = buffer;
                std::stringstream ss(someString);
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
                i = 0;
                status = "200 OK";
                version = "HTTP/1.1 ";
                error = 0;
                if (stor.find("GET") != stor.end())
                    Get_methode(c, envp);
                else if (stor.find("POST") != stor.end())
                    Post_methode();
                std::string header = version + status + "\nContent-type: text/html; charset=UTF-8\nContent-Length: " + std::to_string(len) + "\n\n" + body;
                write(sd, header.c_str(), strlen(header.c_str()));
            }
        }
    }
    i = 0;
}

Server::~Server()
{
}
