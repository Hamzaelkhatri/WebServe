/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 14:27:10 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/09 15:50:08 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

Server::Server(Parsing *p,char *envp[])
{
    cgi *c;
    loc = p->Getloc_map();
    tmp = p->GetServerMap();
    multi_server(p,envp);
    // mtmp = tmp[1];
    // creatSocket_fd();
    // error = 0;
    // bind_listen();
    // while (1)
    // {
    //     std::cout << "\t\t\t Listening " << mtmp.find("listen")->second << std::endl;
    //     accept_socket();
    //     int t = 0;
    //     int i = 0;
    //     lenght = 0;
    //     someString = bufferStor();
    //     std::stringstream ss(someString);
    //     while (std::getline(ss, line1, '\n'))
    //     {
    //         if (line1.find_first_of(":") != std::string::npos && t == 1)
    //         {
    //             tmp1 = line1.substr(0, line1.find_first_of(":"));
    //             tmp2 = line1.substr(line1.find_first_of(":") + 1);
    //             stor[tmp1] = tmp2;
    //         }
    //         else if (t == 0)
    //         {
    //             tmp1 = line1.substr(0, line1.find_first_of(" "));
    //             tmp2 = line1.substr(line1.find_first_of(" ") + 1);
    //             stor[tmp1] = "webpage" + tmp2.substr(0, tmp2.find_first_of(" "));
    //             t++;
    //         }
    //         else
    //         {
    //             t++;
    //             Content.push_back(line1);
    //         }
    //         i++;
    //     }
    //     int l = 0;
    //     i = 0;
    //     status = "200 OK";
    //     version = "HTTP/1.1 ";
    //     error = 0;
    //     std::cout << someString << std::endl;
    //         if (stor.find("GET") != stor.end())
    //             Get_methode(c,envp);
    //         else if (stor.find("POST") != stor.end())
    //             Post_methode();
    //     std::string header = version + status + "\nContent-type: text/html; charset=UTF-8\nContent-Length: " + std::to_string(len) + "\n\n" + body;
    //     write(new_socket, header.c_str(), strlen(header.c_str()));
    //     close(new_socket);
    //     stor.clear();
    // }
    // close(server_fd);
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

int getNumberOfline(std::string str)
{
    int i = 0;
    int count = 0;
    while (str[i])
    {
        if(str[i]=='\n')
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
    while ((nDataLength = recv(sd, (void *)&buffer, 999, 0)) > 0)
    {
        if(someString.find("\r\n\r\n") != std::string::npos && sizeOfFile < loop)
            break;
        bzero(buffer, sizeof(buffer));
        nDataLength = recv(sd, (void *)&buffer, 999, 0);
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
        std::cout << buffer << std::endl;
        // std::cout << buffer << std::endl << nDataLength << "  "<< sizeOfFile << std::endl;
    }
    len = someString.size();
    if(sizeOfFile != -1)
    {
        body = someString.substr(someString.find("\r\n\r\n")+4,someString.size());
        // if(body.size() != sizeOfFile)
        {
            body = body.substr(body.find("\r\n\r\n")+4,body.size());
            body = body.substr(0,body.find("\r\n"));
            SaveFile("/home/hamza/Desktop/WebServe/webpage/upload/output.txt", body,body.size());
        }
        // someString = someString.substr(0,someString.find("\r\n\r\n")+4);
    }
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
      body = body.substr(body.find("\r\n\r\n")+4,body.size());
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