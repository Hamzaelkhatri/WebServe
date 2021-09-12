/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 14:27:10 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/12 14:34:51 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Webserv.hpp"

void Server::creatSocket_fd()
{
  int i = 0;
    // server_fds = new int[h + 1];
    for (i = 0; i < h; i++)
    {
        server_fds[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fds[i] == 0)
        {
            std::cerr << "socket failed" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    for (i = 0; i < h; i++)
    {
        int buffsize = 1;
        if (setsockopt(server_fds[i], SOL_SOCKET, SO_REUSEADDR, &buffsize, sizeof(buffsize)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        // std::memset((char *)&this->address[i], 0, sizeof(&this->address));
    }
}

void Server::bind_listen()
{
     std::map<int, std::multimap<std::string, std::string> >::iterator it0 = tmp.begin();
    std::multimap<std::string, std::string>::iterator it;
    std::map<int, std::string> mini;
    int j = 0;
    int k = 0;
    int  i = 0;
    for (it0 = tmp.begin(); it0 != tmp.end(); ++it0)
    {
        j++;
        k = 0;
        for (it = it0->second.begin(); it != it0->second.end(); it++)
        {
            if (it->first == "server_addr")
            {
                k++;
                mini[j] = it->second;
            }
        }
        if (k == 0)
        {
            mini[j] = "INADDR_ANY";
        }
    }
    i = 0;
    std::map<int, std::string>::iterator o = mini.begin();
    it0 = tmp.begin();
    // address = new struct sockaddr_in[h + 1];
    while (it0 != tmp.end() && o != mini.end())
    {

        for (it = it0->second.begin(); it != it0->second.end(); it++)
        {
            if (it->first.find("listen") != std::string::npos)
            {
                std::string str = it->second;
                while (i < h && str != "NULL")
                {
                    address[i].sin_port = htons(std::stoi(str));
                    address[i].sin_family = AF_INET;
                    if (o->second == "INADDR_ANY")
                        address[i].sin_addr.s_addr = INADDR_ANY;
                    else
                        address[i].sin_addr.s_addr = inet_addr(o->second.c_str());
                    memset(address[i].sin_zero, '\0', sizeof address[i].sin_zero); // why help to pad from sockaddr_in to sockaddr

                    str = "NULL";
                    i++;
                }
            }
        }
        it0++;
        o++;
    }
    i = 0;
    /**********************Bind**************************************/
    while (i < h)
    {
        fcntl(server_fds[i], F_SETFL, O_NONBLOCK);
        // char *s = inet_ntoa(address[i].sin_addr);
        if (bind(server_fds[i], (struct sockaddr *)(&address[i]), sizeof(address[i])) < 0)
            perror("Bind");
        if (listen(server_fds[i],1024) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        i++;
    }
        for (i = 0; i < 1024; i++)
    {
        client_fds[i] = 0;
    }
}
void Server::accept_socket()
{
    // size_client = sizeof(client); 
    // if ((new_socket = accept(server_fd, (struct sockaddr *)&client,
    //                          &size_client)) < 0)
    // {
    //     std::cerr << "acceptance failed" << std::endl;
    //     exit(EXIT_FAILURE);
    // }

}


void Server::SetAll_FD()
{
        int i = 0;
    int count = 0;

    while (i < h)
    {
        nfds = server_fds[i];
        FD_SET(server_fds[i], &readfds);
        // if(FD_ISSET(server_fds[i], &readfds))
        //     count++;
        if(nfds > server_fds[i])
            nfds = server_fds[i];
        i++;
    }
    for (i = 0; i < 1024; i++)
    {
        sd = client_fds[i];
        if (sd > 0)
            FD_SET(sd, &readfds);
        // if(FD_ISSET( client_fds[i], &readfds))
        //     count++;
        if (sd > nfds)
            nfds = sd;
    }
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
        std::cout << "\t\t\t ---waiting for connection--- "  << std::endl;
        SetAll_FD();
        timeval tv;
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        if (select(nfds + 1, &readfds, NULL, NULL,NULL) < 0)
        {
            perror("select");
            exit(EXIT_FAILURE);
        }      
        for (i = 0; i < h; i++)
        {
            if (FD_ISSET(server_fds[i], &readfds))
            {
                int addrlen = sizeof(client_add);
                int new_sd = accept(server_fds[i], (struct sockaddr *)&client_add, (socklen_t *)&addrlen);
                if (new_sd < 0)
                {
                    perror("accept");
                    continue;
                }
                for (int j = 0; j < 1024; j++)
                {
                    if (client_fds[j] == 0)
                    {
                        client_fds[j] = new_sd;
                        FD_SET(new_sd, &readfds);
                        break;
                    }
                }
                // std::cout << "New connection accepted " << new_sd << std::endl;
            }
        }
        int n ;
        for (i = 0; i < 1024; i++)
        {
            int t = 0;
            sd = client_fds[i];
            if (FD_ISSET(sd, &readfds))
            {
                fcntl(sd, F_SETFL, O_NONBLOCK);
                // std::cout << "The client " << sd << " connected"  << std::endl;
                char buffer[1024];
                bzero(buffer, 1024);
                n = recv(sd, (void *)&buffer, sizeof(buffer), 0);
                if (n < 0)
                    continue;
                if (n == 0)
                {
                    // FD_CLR(sd, &readfds);
                    // std::cout << "The client " << sd << " disconnected" << std::endl;
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
    close(sd);
    i = 0;
    while(i < h)
        close(server_fds[i++]);
}

Server::~Server()
{
}
