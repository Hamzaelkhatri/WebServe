/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/10 14:27:10 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/04 12:20:44 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

Server::Server(Parsing *p,char *envp[])
{
    cgi *c;
    loc = p->Getloc_map();
    tmp = p->GetServerMap();
    mtmp = tmp[1];
    creatSocket_fd();
    bind_listen();
    while (1)
    {
        std::cout << "\t\t\t Listening " << mtmp.find("listen")->second << std::endl;
        accept_socket();
        int t = 0;
        int i = 0;
        lenght = 0;
        someString = bufferStor();
        while (i < len)
        {
            line1 = Those_lines(someString, i, len);
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
        std::map<std::string, std::string>::iterator it0;
        std::map<std::string, std::string>::iterator it;
        for(it0 = stor.begin(); it0 != stor.end(); ++it0)
        {
            // std::cout << "-s---------------------------------\n";
            std::cout << "[" << it0->first << "]\t\t\t==>\t"  << it0->second << std::endl;
        }
        // std::string Fie_Content = "";
        i = 0; 
        for(i = 0 ; i < Content.size() ; i++)
            std::cout  << Content.at(i) << std::endl;
        // while (Content.size() > i)
        // {
        //     // if(Content.at(i).find(Content.at(0)) == std::string::npos)
        //     {
        //         std::cout << Content.at(i) << std::endl;
        //         File_Content += Content.at(i) + "\n";
        //     }
        //     i++;
        // }
        // SaveFile("/home/hamza/Desktop/WebServe/txt.c", someString);
        status = "200 OK";
        if (stor.find("GET") != stor.end())
           Get_methode(c,envp);
        else if (stor.find("POST") != stor.end())
            Post_methode();
        std::string header = "HTTP/1.1 " + status + "\nContent-type: text/html; charset=UTF-8\nContent-Length: " + std::to_string(lenght) + "\n\n" + body;
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
    int option = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEPORT, &option, sizeof(option)))
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

std::string Server::bufferStor()
{
    unsigned char *buffer = new unsigned char[8000000];
    someString = "";
    while ((valrecv = read(new_socket, buffer, 8000000)) > 0)
    {
        someString.append((char *)buffer);
        fcntl(new_socket, F_SETFL, O_NONBLOCK);
    }
    len = nbr_lines(someString);
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
          lenght = body.size();
      }
      else if ((dir = check_dir(path, stor.find("GET")->second)))
      {
          if (dir == 1)
          {
              body = getBody(stor.find("GET")->second + "/index.html");
              lenght = body.size();
          }
          else
          {
              body = getBody(stor.find("GET")->second);
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
      char *argv[3];
      std::string str("/usr/bin/php-cgi");
      argv[0] = (char *)str.c_str();
      argv[1] = (char *)stor.find("GET")->second.c_str();
      argv[2] = NULL;
      body = c->CGI(argv, envp);
      lenght = body.size();
      std::cout << "Body ==> " << body << std::endl;
  // std::cout << dir << std::endl;
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