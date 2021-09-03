/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 14:23:25 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/03 12:07:15 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>     //it have all data sockaddr ....
#include <netdb.h> // definitions for network database operations
#include <sys/socket.h> // have data of socket ... lenght 32bits
#include <sys/types.h>
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <sstream>
#include <netinet/in.h>
#include "pars.hpp"
#include "ErrorHandling.hpp"
#include "cgi.hpp"


class Server 
{
    private:
        int server_fd; // socket descriptor, an integer!
        int new_socket;// conection establish btw client & server
        struct sockaddr_in add;
        int sizeof_add;
        int valrecv;  // communication part
        struct sockaddr_in client; // socklen_t size of adress
        socklen_t size_client;
        std::map<std::string, std::string> stor;
        std::map<int, std::multimap<std::string, std::string> > tmp;
        std::multimap<int, std::multimap<std::string, std::string> > loc;
        std::multimap<std::string, std::string> mtmp;
        
    public:
        Server(Parsing *p,char *envp[]);
        ~Server();
        void creatSocket_fd(void);
        void bind_listen(void);
        void accept_socket();
};

#endif