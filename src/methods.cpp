/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   methods.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 18:02:30 by zdnaya            #+#    #+#             */
/*   Updated: 2021/09/15 10:49:30 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

std::string Server::getBody(std::string path)
{
    std::string body;
    std::ifstream file(path);
    if (file.is_open())
    {
        std::string line;
        while (getline(file, line))
        {
            body += line;
            body += "\n";
        }
        file.close();
    }
    return body;
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
    //   body = c->CGI(argv, envp);
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


void Server::Delete_methode()
{
    
}