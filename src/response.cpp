#include "response.hpp"

std::string Response::getStatus()
{
    return (status);
}

std::string Response::getBody()
{
    return (body);
}

std::string Response::getVersion()
{
    return (version);
}

std::string Response::getRedirection()
{
    return (redirection);
}

std::string Response::getCharset()
{
    return (charset);
}

std::string Response::getContentType()
{
    return (content_type);
}

std::string Response::getContentLength()
{
    return (content_length);
}

std::string Response::getSetCookie()
{
    return (set_cookie);
}

std::string Response::getCookie()
{
    return (cookie);
}


//seters

Response::Response()
{

}

Response::~Response()
{
    
}

void Response::setStatus(std::string status)
{
    this->status = status;
}


void Response::setRedirection(std::string redirection)
{
    this->redirection = redirection;
}

void Response::setBody(std::string body)
{
    this->body = body;
}

void Response::setCharset(std::string charset)
{
    this->charset = charset;
}

void Response::setCookie(std::string cookie)
{
    this->cookie = cookie;
}

void Response::setContentLength(std::string content_length)
{
    this->content_length = content_length;
}

void Response::setContentType(std::string content_type)
{
    this->content_type = content_type;
}

