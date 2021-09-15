#ifndef REQUEST_HPP
#define REQUEST_HPP


#include "server.hpp"

class Request
{
private:
    Server* server;
    std::string port;
    std::string host;
    std::string path;
    std::string method;
    std::string version;
    std::string body;
    int client_sock;
    int content_lenght;
public:
    //getters and setters
    std::string get_port();
    std::string get_host();
    std::string get_path();
    std::string get_method();
    std::string get_version();
    std::string get_body();
    int get_client_sock();
    int get_content_lenght();
    void set_port(std::string port);
    void set_host(std::string host);
    void set_path(std::string path);
    void set_method(std::string method);
    void set_version(std::string version);
    void set_body(std::string body);
    void set_client_sock(int client_sock);
    void set_content_lenght(int content_lenght);
    //constructors
    ~Request();
    Request(Server* server, int client_sock);
    //methods
    void parse_request();
    void send_response();
    void send_error(int code);
};

#endif