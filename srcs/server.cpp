#include "server.hpp"

int server::getServer_fd()
{
    return this->server_fd;
}

void server::setServer_fd(int server_fd)
{
    this->server_fd = server_fd;
}

int server::getNew_socket()
{
    return this->new_socket;
}

void server::setNew_socket(int new_socket)
{
    this->new_socket = new_socket;
}

long server::getValread()
{
    return this->valread;
}

void server::setValread(long valread)
{
    this->valread = valread;
}

sockaddr_in server::getAddress()
{
    return this->address;
}

void server::setAddress(sockaddr_in address)
{
    this->address = address;
}

int server::getAddrlen()
{
    return this->addrlen;
}

void server::setAddrlen(int addrlen)
{
    this->addrlen = addrlen;
}

server::server(/* args */)
{
}

server::~server()
{
}

server::server(int Port, char *message) : message(message)
{
    opt = 1;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    memset((char *)&address, '\0', sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(Port);
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
}

void server::_listen()
{
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read(new_socket, buffer, 1024);
    std::cout << "BUFFER " << buffer << std::endl;
    send(new_socket, message, strlen(message), 0);
    std::cout << "Message Sent " << std::endl;
}
