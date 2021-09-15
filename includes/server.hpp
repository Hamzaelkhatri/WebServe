/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 14:23:25 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/15 10:52:22 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>     //it have all data sockaddr ....
#include <netdb.h> // definitions for network database operations
#include <sys/socket.h> // have data of socket ... lenght 32bits
#include <sys/types.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <sstream>
#include "pars.hpp"
#include "ErrorHandling.hpp"
#include "cgi.hpp"
#include <string>
#include <dirent.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string>
#include <regex>
#include <arpa/inet.h> 
#include "socket.hpp"
#include "pars.hpp"
#include <string.h>

#define BUFFER_SIZE   65507

class Socket;
class Server  : public Socket 
{
    private:
        int sd;
        int len;
        int cnx;
        int nfds;
        int maxfd;
        int csock;
        int lenght;
        Socket *sock;
        int selected;
        char * request;
        fd_set readfds;
        fd_set writefds;
        fd_set exceptfds;
        std::string tmp2;
        std::string tmp1;
        std::string body;
        std::string line1;
        std::string path;
        fd_set masterfds;
        std::string status;
        std::string someString;
        std::vector<int> clients;
        struct sockaddr_in client;
        std::vector<int> MasterSockets;
        std::vector<std::string> Content;
        std::map<int, std::string> _clients;
        std::map<std::string, std::string> stor;
        std::multimap<int, std::multimap<std::string, std::string> > loc;
    public:
        Server(Parsing *pars,char *envp[]);
        int             _Accept_client(int sock);
        int             _Get_request(int csock);
        void            Get_methode(cgi *c,char *envp[]);
        void            Post_methode();
        void            Delete_methode();
        std::string     getBody(std::string path);
        int             check_dir(std::string dir, std::string str);
        ~Server();

int check_index(std::string str);
};

#endif