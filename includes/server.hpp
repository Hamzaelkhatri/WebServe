/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zainabdnayagmail.com <zainabdnayagmail.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 14:23:25 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/23 10:48:33 by zainabdnaya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>  //it have all data sockaddr ....
#include <netdb.h>      // definitions for network database operations
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
#include "request.hpp"

#define BUFFER_SIZE 65507
//Regular text
#define BLK "\e[0;30m"
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define WHT "\e[0;37m"
#define reset "\e[0m"

class Socket;
class Server : public Socket
{
private:
    int sd;
    int len;
    int cnx;
    int nfds;
    int maxfd;
    int csock;
    int lenght;
    int selected;
    char *request;
    Socket *sock;
    fd_set readfds;
    fd_set writefds;
    fd_set masterfds;
    fd_set exceptfds;
    std::string path;
    std::string tmp2;
    std::string tmp1;
    std::string body;
    std::string line1;
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
    Server(Parsing *pars, char *envp[]);
    int _Accept_client(int sock);
    int _Get_request(int csock);
    int check_index(std::string str);
    int check_dir(std::string dir, std::string str);
    bool checkRequest(std::string &req);
    void _GetDataServers(Parsing *parsing, Response *response,Request *request);
    void witch_server(std::map<int, std::string> str, Parsing *pars);
    void Post_Method(Request *request,Parsing *parsing,int indexOfServer,int indexOflocation,Response *response);
    void Delete_methode(Request *request,Parsing *parsing,int indexOfServer,int indexOflocation,Response *response);
    void Get_methode(cgi *c, char *envp[]);
    std::string str5;

    std::string getBody(std::string path);
    std::map<int, std::string>::iterator its;

    class AcceptFailed : public std::exception
    {
        virtual const char *what() const throw();
    };
    class SelectFailed : public std::exception
    {
        virtual const char *what() const throw();
    };
    std::string GetValueBykeyLocation(std::multimap<int, std::multimap<std::string, std::string> > locations, int indexOfServer, int indexOfLocation, std::string key);
    std::string _GetFirstLocation(std::multimap<int, std::multimap<std::string, std::string> >::iterator locations);
    std::string GetValueBykeyServer(std::map<int, std::multimap<std::string, std::string> > servers, int indexOfserver, std::string key);
    bool is_location(std::multimap<int, std::multimap<std::string, std::string> >::iterator locations, std::string location);
    int GetTargetServer(Request *request,Parsing *parsing,std::string &root,std::multimap<std::string, std::string>::iterator it3,std::map<int, std::multimap<std::string, std::string> >::iterator it,int &check_server,int indexOfServer);
    int check_if_file_or_dir(std::string path);
    std::string CreateAutoIndexHtmlFile(std::string path, std::string locatioName);
    std::string getBodyFromFile(std::string path);
void execute_cgi(Response *response, int TargetServer, int TargetLocation, std::string root, Parsing *parsing, cgi *c, Request *request);
void SaveAsFile(std::string path, std::string body, int b);
std::map<std::string, std::string> pars_request();
    ~Server();
};

#endif