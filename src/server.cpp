/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 14:27:10 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/06 19:43:17 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"
#include <string>
#include <regex>

char *removeHTTPHeader(char *buffer, int &bodySize) {
    char *t = strstr(buffer, "\r\n\r\n");
    t = t + 4;

    for (char* it = buffer; it != t; ++it) 
        ++bodySize;

    return t;
}


Server::Server(Parsing *p,char *envp[])
{
    cgi *c;
    loc = p->Getloc_map();
    tmp = p->GetServerMap();
    mtmp = tmp[1];
    creatSocket_fd();
    error = 0;
    bind_listen();
    while (1)
    {
        std::cout << "\t\t\t Listening " << mtmp.find("listen")->second << std::endl;
        accept_socket();
        int t = 0;
        int i = 0;
        lenght = 0;
        someString = bufferStor();
        std::stringstream ss(someString);
        while (std::getline(ss, line1, '\n'))
        {
            if (line1.find_first_of(":") != std::string::npos && t == 1)
            {
                tmp1 = line1.substr(0, line1.find_first_of(":"));
                tmp2 = line1.substr(line1.find_first_of(":") + 1);
                stor[tmp1] = tmp2;
            }
            else if (t == 0)
            {
                tmp1 = line1.substr(0, line1.find_first_of(" "));
                tmp2 = line1.substr(line1.find_first_of(" ") + 1);
                stor[tmp1] = "webpage" + tmp2.substr(0, tmp2.find_first_of(" "));
                t++;
            }
            else
            {
                t++;
                Content.push_back(line1);

            }
            i++;
        }
        
        // std::cout << someString << std::endl;
        int l = 0;
        // char *d= removeHTTPHeader((char *)someString.c_str(), l);
        // SaveFile("/home/hamza/Desktop/WebServe/output.png", d);
        // std::map<std::string, std::string>::iterator it0;
        // std::map<std::string, std::string>::iterator it;
        // for(it0 = stor.begin(); it0 != stor.end(); ++it0)
        // {
        //     // std::cout << "-s---------------------------------\n";
        //     std::cout << "[" << it0->first << "]\t\t\t==>\t"  << it0->second << std::endl;
        // }
        // std::string File_Content = "";
        // i = 0; 
        // for(i = 0 ; i < Content.size() ; i++)
        //     std::cout  << Content.at(i) << std::endl;
        i = 0;
        // while (Content.size() > i)
        // {
        //     // if(Content.at(i).find(Content.at(0)) == std::string::npos)
        //     {
        //         std::cout << Content.at(i) << std::endl;
        //         File_Content += Content.at(i) + "\n";
        //     }
        //     i++;
        // }

        status = "200 OK";
        version = "HTTP/1.1 ";
        error = 0;
        std::cout << someString << std::endl;
        // if(!error)
        // {
            if (stor.find("GET") != stor.end())
                Get_methode(c,envp);
            else if (stor.find("POST") != stor.end())
                Post_methode();
        // }
        // else
        // {
        //     status = "400 Bad Request";
        //     version = "HTTP/1.1 ";
        //     body = someString;
        // }
        std::string header = version + status + "\nContent-type: text/html; charset=UTF-8\nContent-Length: " + std::to_string(len) + "\n\n" + body;
        write(new_socket, header.c_str(), strlen(header.c_str()));
        close(new_socket);
        stor.clear();
    }
    close(server_fd);
}

void Server::creatSocket_fd()
{
    std::memset((char *)&this->add, 0, sizeof(this->add));
        // ** CREATE SOCKET**/
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
    {
        std::cerr << "socket failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    ///Set Socket
    //int setsockopt(int socket, int level, int option_name, const void *option_value, socklen_t option_length);
    //it helps in reuse of address and port
    int buffsize = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &buffsize, sizeof(buffsize)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    // bind an IP add and a port to a  socket
    //   p->GetServerMap().find()
}

void Server::bind_listen()
{
    add.sin_port = htons(std::stoi(mtmp.find("listen")->second));
    add.sin_family = AF_INET;
    if (mtmp.find("server_addr")->second.c_str() != NULL)
        add.sin_addr.s_addr = inet_addr(mtmp.find("server_addr")->second.c_str());
    else
        add.sin_addr.s_addr = INADDR_ANY;
    memset(add.sin_zero, '\0', sizeof add.sin_zero); // why help to pad from sockaddr_in to sockaddr
    // Forcefully attaching socket to the PORT
    if (bind(server_fd, (struct sockaddr *)&add, sizeof(add)) < 0)
    {
        perror("Bind");
        std::cerr << "Bind failed" << std::endl;
        exit(EXIT_FAILURE);
    }
    // wait for an incoming connection
    if (listen(server_fd, SOMAXCONN)) // SOMAXCONN is the maximum number of pending connections that can be queued up before connections are refused.
    {
        std::cerr << "Listening failed" << std::endl;
        exit(EXIT_FAILURE);
    }
}


void Server::accept_socket()
{
    size_client = sizeof(client); 
    if ((new_socket = accept(server_fd, (struct sockaddr *)&client,
                             &size_client)) < 0)
    {
        std::cerr << "acceptance failed" << std::endl;
        exit(EXIT_FAILURE);
    }

}

int ContaineOnly(std::string str)
{
    std::regex e ("[\\n\\r\\n\\r]+");
    if (std::regex_match(str, e))
        return 1;
    return 0;
}

int indexOfNewLine(std::string str)
{
    int i = 0;
    while (str[i])
    {
        if(str[i]=='\n')
            return (i);
        i++;
    }
    return (i-1);
}

int isAlpha(std::string str)
{
    std::regex e ("[a-zA-Z]+");//iudeheideigefytuvngjcigaicgwiagaiohdisgsx
    if (std::regex_match(str, e))
        return 1;
    return 0;
}

int isCapital(std::string str)
{
    std::regex e ("[A-Z]+");
    if (std::regex_match(str, e))
        return 1;
    return 0;
}

int numberOfWords(std::string str)
{
    int i = 0;
    int count = 0;
    while (str[i])
    {
        if(str[i]==' ')
            count++;
        i++;
    }
    return (count+1);
}

std::string Server::bufferStor()
{
    char buffer[1000];
    int nDataLength = 2;
    int loop = 0;
    int n = 0;
    int first_line = 0;
    error = 0;
    
    someString = "";
    int sizeOfFile=-1;
    while (1)
    {
        if(someString.find("\r\n\r\n") != std::string::npos && sizeOfFile < loop)
            break;
        bzero(buffer, sizeof(buffer));
        nDataLength = recv(new_socket, (void *)&buffer, 999, 0);
        loop+=nDataLength;
        if (nDataLength < 0 || nDataLength == 0)
            break;
        if(nDataLength > 2 || n != 0)
        {
            someString += buffer;
            if(!first_line)
            {
                // std::cout << someString << std::endl;
                std::string str = someString.substr(0,someString.find_first_of("\r\n"));
                if(!isCapital(str.substr(0,str.find_first_of(" "))) || someString.find("HTTP/1.1") == std::string::npos || numberOfWords(str) != 3)
                {
                    error = 1;
                    someString ="<html><head><title>400 Bad Request</title></head><body><h1>Bad Request</h1><p>Bad Request</p></body></html>";
                    len = someString.size();
                    return(someString);
                }
                first_line = 1;
            }
            if(someString.find("Content-Length:") != std::string::npos)
            {
                int index = someString.find("Content-Length:")+15;
                sizeOfFile =std::stoi(someString.substr(someString.find("Content-Length:")+15,indexOfNewLine(&someString[index]-1)));
            }
            n++;
        }
        // std::cout << buffer << std::endl << nDataLength << "  "<< sizeOfFile << std::endl;
    }
    len = someString.size();
    return(someString);

}


void Server::Get_methode(cgi *c,char *envp[])
{
  int dir = 0;
  std::multimap<std::string, std::string> mtmp = loc.find(1)->second;
  path = "webpage" + mtmp.find("location")->second;
  if (stor.find("GET")->second.find(".php") == std::string::npos)
  {
      if (stor.find("GET")->second == path)
      {
          body = getBody(stor.find("GET")->second + "/index.html");
          len = body.size();
      }
      else if ((dir = check_dir(path, stor.find("GET")->second)))
      {
          if (dir == 1)
          {
              body = getBody(stor.find("GET")->second + "/index.html");
              len = body.size();
          }
          else
          {
              body = getBody(stor.find("GET")->second);
              len = body.size();
          }
      }
      else
      {
          body = getBody("webpage/errors/404.html");
          len = body.size();
          status = "404 Not Found";
      }
  }
  else
  {
      char *argv[3];
      std::string str("/Users/helkhatr/goinfre/.brew/bin/php-cgi");
      argv[0] = (char *)str.c_str();
      argv[1] = (char *)stor.find("GET")->second.c_str();
      argv[2] = NULL;
      body = c->CGI(argv, envp);
      len = body.size();
  }
}

void Server::Post_methode()
{

    std::multimap<std::string, std::string> mtmp = loc.find(1)->second;
    path = "webpage" + mtmp.find("location")->second;
    if (mtmp.find("http_methods")->second.find("POST") != std::string::npos)
    {
        int dir = 0;
        if (stor.find("POST")->second == path)
        {
            body = getBody(stor.find("POST")->second + "/index.html");
            lenght = body.size();
        }
        else if ((dir = check_dir(path, stor.find("POST")->second)))
        {
            if (dir == 1)
            {
                body = getBody(stor.find("POST")->second + "/index.html");
                lenght = body.size();
            }
            else
            {
                body = getBody(stor.find("POST")->second);
                lenght = body.size();
            }
        }
        else
        {
            body = getBody("webpage/errors/404.html");
            lenght = body.size();
            status = "404 Not Found";
        }
    }
    else
    {
        body = getBody("webpage/errors/405.html");
        lenght = body.size();
        status = "405 Not Allowed";
    }
}

Server::~Server()
{
}