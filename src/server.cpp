#include "../includes/Webserv.hpp"
#include "../includes/request.hpp"
#include <dirent.h>

std::map<std::string, std::string> Server::pars_request()
{
    std::stringstream ss(someString);
    std::map<std::string, std::string> stor;
    int t = 0;
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
            std::cout << " tmp1  |" << tmp1 << "|" << std::endl;
            tmp2 = line1.substr(line1.find_first_of(" ") + 1);
            stor[tmp1] = tmp2.substr(0, tmp2.find_first_of(" "));
            t++;
        }
        else
        {
            t++;
            Content.push_back(line1);
        }
    }
    return (stor);
}
void Server::_GetDataServers(Parsing *parsing, Response *response, Request *request)
{
    stor = pars_request();
    std::map<int, std::multimap<std::string, std::string>> servers = parsing->GetServerMap();
    std::multimap<int, std::multimap<std::string, std::string>> locations = parsing->Getloc_map();
    std::map<int, std::multimap<std::string, std::string>>::iterator it;
    std::multimap<int, std::multimap<std::string, std::string>>::iterator it2;
    std::multimap<std::string, std::string>::iterator it3;
    std::multimap<std::string, std::string>::iterator it4;
    std::multimap<std::string, std::string> LocationContent;
    int indexOfServer = 1;
    int indexOfLocation = 1;
    std::string location_tmp = _GetFirstLocation(locations.begin());
    std::string Host = stor["Host"].substr(0, stor["Host"].find(":"));
    std::string Port = stor["Host"].substr(stor["Host"].find(":") + 1, stor["Host"].size());
    std::string Methode = (stor.find("GET") != stor.end() ? "GET" : (stor.find("POST") != stor.end()) ? "POST"
                                                                : (stor.find("DELETE") != stor.end()) ? "DELETE"
                                                                                                      : "UNKNOWN");
    std::string Content_lenght = (stor.find("Content-Length") != stor.end()) ? stor["Content-Length"] : "0";
    std::string Content_Disposition = "";
    for (int j = 0; j < Content.size(); j++)
    {
        if (Content[j].find("Content-Disposition") != std::string::npos)
        {
            // std::string tmp = Content[j].substr(Content[j].find("filename=") + 9, Content[j].size());
            // tmp = tmp.substr(0, tmp.find(";"));
            Content_Disposition = Content[j].substr(Content[j].find("filename=") + 9, Content[j].size());
            break;
        }
    }
    std::cout << Content_Disposition << std::endl;
    std::string Path = stor[Methode];

    request->set_host(Host);
    request->set_port(Port);
    request->set_method(Methode);
    request->set_path(Path);
    request->set_content_lenght(std::stoi(Content_lenght));
    request->set_filename(Content_Disposition);
    cgi *c;

    // std::cout << "Host: " << Host << std::endl;
    // std::cout << "Port: " << Port << std::endl;
    // std::cout << "Methode: " << Methode << std::endl;
    // std::cout << "Path: " << Path << std::endl;

    response->setContentType("text/html");
    response->setVersion("HTTP/1.1");
    response->setCharset("UTF-8");
    std::string root = "";
    int TargetServer = 0;
    int TargetLocation = 0;
    int check_server = 0;
    str5 += its->second;

    for (it = servers.begin(); it != servers.end(); it++)
    {
        indexOfLocation = 0;
        TargetServer = 0;
        for (it3 = it->second.begin(); it3 != it->second.end(); ++it3)
        {
            if (it3->first.find("listen") != std::string::npos && request->get_port().find(it3->second) != std::string::npos)
                TargetServer -= 1;
            if (it3->first.find("server_addr") != std::string::npos && (request->get_host().find(it3->second) != std::string::npos || request->get_host().find("localhost") != std::string::npos))
                TargetServer -= 1;
            else if (it3->first.find("server_name") != std::string::npos && request->get_host().find(it3->second) != std::string::npos)
                TargetServer -= 1;
            if (TargetServer <= -2)
            {
                check_server = 1;
                TargetServer = indexOfServer;
                root = GetValueBykeyServer(servers, indexOfServer, "root");
                break;
            }
        }
        TargetLocation = 1;

        for (it2 = locations.begin(); it2 != locations.end(); it2++)
        {
            std::string pathLocation = request->get_path();
            if (it2->first == TargetServer)
            {
                location_tmp = _GetFirstLocation(it2);
                // std::cout << location_tmp << std::endl;
                for (it4 = it2->second.begin(); it4 != it2->second.end(); it4++)
                {
                    TargetLocation = std::stoi(it4->first.substr(0, it4->first.find(" ")));
                    if (pathLocation.find(".py") != std::string::npos)
                    {
                        if (location_tmp == "*.py")
                            execute_cgi(response, TargetServer, TargetLocation, root, parsing, c, request);
                        break;
                    }
                    else if (pathLocation.find(".php") != std::string::npos)
                    {
                        if (location_tmp == "*.php")
                        {
                            execute_cgi(response, TargetServer, TargetLocation, root, parsing, c, request);
                            if (request->get_method() == "POST")
                            {
                                std::string BodySize = GetValueBykeyServer(servers, indexOfServer, "client_body_size");
                                if (BodySize == "")
                                    BodySize = "10m";
                                std::cout << request->get_content_lenght()<< " " << (std::stol(BodySize)* 1048576) << std::endl;
                                if (request->get_content_lenght() > 0 && std::stoi(BodySize) > 0 && request->get_content_lenght() <= (std::stol(BodySize)* 1048576))
                                {
                                    SaveAsFile("/home/hamza/Desktop/WebServe/output.txt", its->second, 1);
                                }
                                else
                                {
                                    response->setStatus("413");
                                    response->setBody("413 Request Entity Too Large");
                                }
                            }
                        }
                        break;
                    }
                    else
                    {
                        root = GetValueBykeyServer(servers, indexOfServer, "root");
                        if (location_tmp == pathLocation)
                        {
                            if (GetValueBykeyLocation(locations, TargetServer, TargetLocation, "root") == "")
                                root = GetValueBykeyServer(servers, indexOfServer, "root");
                            else
                                root = GetValueBykeyLocation(locations, TargetServer, TargetLocation, "root");
                            // std::cout << GetValueBykeyLocation(locations, TargetServer, TargetLocation, "root") << std::endl;

                            if (check_if_file_or_dir(root + request->get_path()) == 1)
                            {
                                response->setStatus(GetValueBykeyLocation(locations, TargetServer, TargetLocation, "return"));
                                response->setCookie("");
                                response->setSetCookie("");
                                response->setPath(root + request->get_path());
                                response->setHost(GetValueBykeyServer(servers, TargetServer, "server_addr"));
                                if (GetValueBykeyServer(servers, TargetServer, "server_name") != "")
                                    response->setServerName(GetValueBykeyServer(servers, TargetServer, "server_name"));
                                else
                                    response->setServerName("localhost");
                                response->setCGIPath(GetValueBykeyLocation(locations, TargetServer, TargetLocation, "cgi_path"));
                                response->setMethod(request->get_method());
                                response->setRedirection("");
                                std::string Bodytmp = getBodyFromFile(root + request->get_path());
                                response->setBody(Bodytmp);
                                response->setContentLength("");
                            }
                            else if (check_if_file_or_dir(root + request->get_path()) == 2)
                            {
                                if (GetValueBykeyLocation(locations, TargetServer, TargetLocation, "index") != "")
                                {
                                    if (Path[Path.size() - 1] != '/')
                                    {
                                        response->setStatus("301"); //moved permanently
                                        response->setRedirection("\nlocation:" + Path + "/");
                                        response->setPath(root + request->get_path() + "/");
                                    }
                                    std::string BodyTmp = getBodyFromFile(root + request->get_path() + GetValueBykeyLocation(locations, TargetServer, TargetLocation, "index"));
                                    if (BodyTmp.empty())
                                    {
                                        std::cout << "403 forbidden" << std::endl; //403 forbidden
                                        response->setStatus("403");
                                    }
                                    response->setBody(BodyTmp);
                                    if (GetValueBykeyLocation(locations, TargetServer, TargetLocation, "return") != "")
                                        response->setStatus(GetValueBykeyLocation(locations, TargetServer, TargetLocation, "return"));
                                    response->setCookie("");
                                    response->setSetCookie("");
                                    response->setContentLength("");
                                    body = response->getBody();
                                }
                                else if (GetValueBykeyLocation(locations, TargetServer, TargetLocation, "autoindex") != "" && GetValueBykeyLocation(locations, TargetServer, TargetLocation, "") == "off")
                                {
                                    response->setBody(CreateAutoIndexHtmlFile(root + request->get_path(), pathLocation));
                                    response->setStatus("");
                                    response->setContentLength("");
                                    if (Path[Path.size() - 1] != '/')
                                    {
                                        response->setStatus("301"); //moved permanently
                                        response->setRedirection("\nlocation:" + Path + "/");
                                    }
                                }
                            }
                            else
                                std::cout << root + request->get_path() << " 404 not found" << std::endl;
                            break;
                        }
                    }
                    if (pathLocation.find_last_of("/") != std::string::npos)
                        pathLocation = pathLocation.substr(0, pathLocation.find_last_of("/"));
                    else
                        pathLocation = "/";
                }
            }
            TargetLocation++;
        }
        indexOfServer++;
    }
    // std::cout << request->get_method();

    if (check_server == 0)
    {
        std::cout << TargetServer << std::endl;
        response->setStatus("403");
        response->setContentType("text/html");
        response->setVersion("HTTP/1.1");
        response->setCharset("UTF-8");
        response->setBody("<html><head><title>403</title></head><body><h1>403 </h1> </body> </html>");
        response->setContentLength("");
    }
}

Server::Server(Parsing *p, char **envp)
{
    std::string version;
    Response *response = new Response();

    this->sock = new Socket(p);
    maxfd = 0;
    int j = 0;
    int rc = 0;
    char buffer[BUFFER_SIZE + 1];
    // cgi *c;
    p->set_env(envp);
    FD_ZERO(&masterfds);
    MasterSockets = this->sock->_Get_server_fds();
    int i = 0;
    while (i < this->sock->_Get_h())
    {
        FD_SET(MasterSockets[i], &masterfds);
        if (MasterSockets[i] > maxfd)
            maxfd = MasterSockets[i];
        i++;
    }
    FD_ZERO(&writefds);
    memcpy(&writefds, &masterfds, sizeof(masterfds));
    std::map<int, std::string> ips;
    Request *request = new Request();
    int b = 0;
    int o = 0;
    while (1)
    {
        FD_ZERO(&readfds);
        memcpy(&readfds, &masterfds, sizeof(masterfds));
        struct timeval _tv = {30, 0};
        selected = select(maxfd + 1, &readfds, &writefds, NULL, &_tv);
        if (selected < 0)
            throw(SelectFailed());
        else
            for (int sock_fd = 0; sock_fd <= maxfd; sock_fd++)
            {
                if (FD_ISSET(sock_fd, &readfds))
                {
                    int newCnx = 0;
                    for (int j = 0; j < MasterSockets.size(); j++)
                    {
                        if (sock_fd == MasterSockets[j])
                        {
                            std::string str1(inet_ntoa(this->sock->_Get_addr()[j].sin_addr));
                            std::string str0 = ":" + std::to_string(ntohs(this->sock->_Get_addr()[j].sin_port));
                            std::string str = str1 + str0;
                            ips.insert(std::pair<int, std::string>(j, str));
                            newCnx = 1;
                            break;
                        }
                    }
                    if (newCnx)
                        _Accept_client(sock_fd);
                    else
                    {
                        bzero(buffer, BUFFER_SIZE);
                        rc = recv(sock_fd, buffer, BUFFER_SIZE, 0);
                        if (rc > 0)
                        {
                            its = _clients.find(sock_fd);
                            buffer[rc] = '\0';
                            if (its != _clients.end())
                                its->second += buffer;
                            someString = its->second;
                            // std::cout << someString << std::endl;
                            if (checkRequest(its->second) == true)
                            {
                                // puts("here");
                                // someString = it->second;
                                if (FD_ISSET(sock_fd, &writefds))
                                {
                                    //Show Stor
                                    // std::cout << YEL << "***********************************" << reset << std::endl;
                                    // std::cout << someString << std::endl;
                                    // std::cout << YEL << "***********************************" << reset << std::endl;
                                    _GetDataServers(p, response, request);
                                    std::string header = response->getVersion() + " " + response->getStatus() + "\nContent-type: " + response->getContentType() + "; charset= " + response->getCharset() + response->getRedirection() + "\nContent-Length: " + std::to_string(response->getBody().size()) + "\n\n" + response->getBody();
                                    write(sock_fd, header.c_str(), strlen(header.c_str()));
                                    its->second.clear();
                                }
                            }
                        }
                        else
                        {
                            if (rc == 0)
                            {
                                b = 0;
                                // someString = "";
                                its->second.clear();
                                std::cout << sock_fd << "\t  =   Diconnected" << std::endl;
                                close(sock_fd);
                                FD_CLR(sock_fd, &masterfds);
                                FD_CLR(sock_fd, &writefds);
                                // exit(0);
                            }
                            else
                                continue;
                        }
                    }
                }
            }
    }
}

Server::~Server()
{
    delete this->sock;
}