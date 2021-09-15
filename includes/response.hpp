#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>

class Response
{
private:
    std::string status;
    std::string version;
    std::string body;
    std::string redirection;
    std::string content_type;
    std::string content_length;
    std::string charset;
    std::string set_cookie;
    std::string cookie;

public:
    Response();
    ~Response();

    //geters
    std::string getStatus();
    std::string getVersion();
    std::string getBody();
    std::string getRedirection();
    std::string getContentType();
    std::string getContentLength();
    std::string getCharset();
    std::string getSetCookie();
    std::string getCookie();
    //setters
    void setStatus(std::string status);
    void setVersion(std::string version);
    void setBody(std::string body);
    void setRedirection(std::string redirection);
    void setContentType(std::string content_type);
    void setContentLength(std::string content_length);
    void setCharset(std::string charset);
    void setSetCookie(std::string set_cookie);
    void setCookie(std::string cookie);
};

#endif