/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zainabdnayagmail.com <zainabdnayagmail.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/28 16:45:47 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/28 19:50:09 by zainabdnaya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/response.hpp"

//read file
#include <fstream>
#include <iostream>
#include <string>

Response::Response()
{
    status = "200 OK";
    version = " HTTP/1.1";
    redirection = "";
    charset = "";
    content_type = "text/html";
    content_length = "";
    set_cookie = "";
    cookie = "";
    path = "/";
    host = "localhost";
    port = "5000";
    method = "GET";
    ssid = "";
    cgipath = "";
    params = "";
}

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

std::string Response::getPath()
{
    return (path);
}

std::string Response::getHost()
{
    return (host);
}

std::string Response::getPort()
{
    return (port);
}
std::string Response::getMethod()
{
    return (method);
}

std::string Response::getSSID()
{
    return (ssid);
}
//seters

void Response::setPath(std::string path)
{
    this->path = path;
}

void Response::setHost(std::string host)
{
    this->host = host;
}

void Response::setServerName(std::string servername)
{
    this->servername = servername;
}

void Response::setCGIPath(std::string cgi_path)
{
    this->cgipath = cgi_path;
}

std::string Response::getCGIPath()
{
    return (cgipath);
}

void Response::set_params(std::string param)
{
    this->params = param;
}

std::string Response::get_params()
{
    return params;
}

std::string Response::getServerName()
{
    return (servername);
}

void Response::setPort(std::string port)
{
    this->port = port;
}

void Response::setMethod(std::string method)
{
    this->method = method;
}

void Response::setSSID(std::string ssid)
{
    this->ssid = ssid;
}
void Response::setStatus(std::string status)
{
    if (status == "")
        this->status = "200 OK";
    else
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
    this->content_length = std::to_string(body.size());
    (void)content_length;
}

void Response::setContentType(std::string content_type)
{
    this->content_type = content_type;
}

void Response::setVersion(std::string version)
{
    this->version = version;
}

void Response::setSetCookie(std::string set_cookie)
{
    this->set_cookie = set_cookie;
}

Response::~Response()
{
}