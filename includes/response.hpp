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
    std::string path;
    std::string host;
    std::string port;
    std::string method;
    std::string servername;
    std::string query;
    std::string cgipath;
    std::string params;

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
    std::string getPath();
    std::string getHost();
    std::string getPort();
    std::string getMethod();
    std::string getServerName();
    std::string getCGIPath();
    //setters
    void setStatus(std::string status);
    void setVersion(std::string version);
    void setBody(std::string body);
    void setRedirection(std::string redirection);
    void setContentType(std::string content_type);
    void set_params(std::string param);
    std::string get_params();
    void setContentLength(std::string content_length);
    void setCharset(std::string charset);
    void setSetCookie(std::string set_cookie);
    void setCookie(std::string cookie);
    void setPath(std::string path);
    void setHost(std::string host);
    void setPort(std::string port);
    void setMethod(std::string method);
    void setServerName(std::string servername);
    void setCGIPath(std::string cgipath);
};

#endif