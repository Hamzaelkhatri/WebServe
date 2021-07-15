#ifndef SERVER_HPP
#define SERVER_HPP
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

class server
{
private:
    int opt;
    int server_fd;
    int new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen;
    char *message;
    char buffer[1024];
    int Port;
    server();

public:
    server(int Port, char *message);

    //getters and setters
    int getServer_fd();
    void setServer_fd(int server_fd);

    int getNew_socket();
    void setNew_socket(int new_socket);

    long getValread();
    void setValread(long valread);

    sockaddr_in getAddress();
    int getAddrlen();

    void setAddress(sockaddr_in address);
    void setAddrlen(int addrlen);

    //destractor
    void _listen();
    virtual ~server();
};
#endif