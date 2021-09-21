#include "request.hpp"

// getters and setters

std::string Request::get_port()
{
    return port;
}

std::string Request::get_host()
{
    return host;
}

std::string Request::get_path()
{
    return path;
}

std::string Request::get_method()
{
    return method;
}

std::string Request::get_version()
{
    return version;
}

std::string Request::get_body()
{
    return body;
}

void Request::set_body(std::string body)
{
    this->body = body;
}

void Request::set_client_sock(int client_sock)
{
    this->client_sock = client_sock;
}

void Request::set_content_lenght(int content_lenght)
{
    this->content_lenght = content_lenght;
}

void Request::set_host(std::string host)
{
    this->host = trim(host);
}

void Request::set_method(std::string method)
{
    this->method = method;
}

void Request::set_path(std::string path)
{
    this->path = path;
}

void Request::set_port(std::string port)
{
    this->port = port;  
}

void Request::set_version(std::string version)
{
    this->version = version;
}

Request::~Request()
{

}

Request::Request()
{

}


Request::Request(Server *server,int client_sock)
{
    this->server = server;
    this->client_sock = client_sock;
}
