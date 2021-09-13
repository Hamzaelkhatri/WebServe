/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zainabdnayagmail.com <zainabdnayagmail.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 14:23:25 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/13 22:17:31 by zainabdnaya      ###   ########.fr       */
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

#define BUFFER_SIZE   1024

class Socket;
class Server  : public Socket 
{
    private:
        std::string status;
        std::string tmp2;
        std::string tmp1;
        std::string body;
        std::string line1;
        std::string path;
        int sd;
        int len;
        std::string someString;
        std::map<std::string, std::string> stor;
        std::vector<std::string> Content;
        std::multimap<int, std::multimap<std::string, std::string> > loc;
        int lenght;
        
        //
        int selected;
        Socket *sock;
        fd_set masterfds;
        //select attrubutes
        char * request;
        fd_set readfds;
        fd_set writefds;
        fd_set exceptfds;
        int maxfd;
        int nfds;
        int cnx;
        int csock;
        std::vector<int> clients;
        struct sockaddr_in client;
        std::vector<int> MasterSockets;
    
    public:
        Server( Parsing *pars,char *envp[]);
        int _Accept_client(int sock);
        int _Get_request(int csock);
        ~Server();
        void Get_methode(cgi *c,char *envp[]);
        void Post_methode();
       
        std::string getBody(std::string path);
        int check_dir(std::string dir, std::string str);

int check_index(std::string str);
};

#endif