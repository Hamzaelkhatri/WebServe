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

std::string Request::get_filename()
{
    if(filename.size()<2)
        return filename;
    return filename.substr(1, filename.size() - 3);
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

void Request::set_filename(std::string filename)
{
    this->filename = filename;
}



void Request::set_host(std::string host)
{
    this->host =host;
}

void Request::set_method(std::string method)
{
    this->method = method;
}

void Request::set_params(std::string param)
{
    this->params = param;
}

std::string Request::get_params()
{
    return params;
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

//initialaze
Request::Request(): client_sock(-1), content_lenght(0), filename(""), host(""), method(""), params(""), path(""), port(""), version("")
{

}

int Request::get_content_lenght()
{
    return content_lenght;
}



Request::Request(int client_sock)
{
    this->client_sock = client_sock;
}
