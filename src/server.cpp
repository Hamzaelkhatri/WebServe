#include "../includes/Webserv.hpp"
#include "../includes/request.hpp"
#include <dirent.h>

std::string GetValueBykeyLocation(std::multimap<int, std::multimap<std::string, std::string>> locations, int indexOfServer, int indexOfLocation, std::string key)
{
    std::multimap<int, std::multimap<std::string, std::string>>::iterator it;
    std::multimap<std::string, std::string>::iterator it2;

    for (it = locations.begin(); it != locations.end(); ++it)
    {
        if (it->first == indexOfServer)
        {
            for (it2 = it->second.begin(); it2 != it->second.end(); ++it2)
            {
                if (it2->first.find(std::to_string(indexOfLocation) + " " + key) != std::string::npos)
                {
                    return (it2->second);
                }
            }
        }
        // std::string res = locations.find(indexOfServer)->second.find()->second;
    }
    return ("");
}

std::string _GetFirstLocation(std::multimap<int, std::multimap<std::string, std::string>>::iterator locations)
{
    std::multimap<int, std::multimap<std::string, std::string>>::iterator it;
    std::multimap<std::string, std::string>::iterator it2;

    for (it2 = locations->second.begin(); it2 != locations->second.end(); ++it2)
    {
        if (it2->first.find("location") != std::string::npos)
            return (it2->second);
    }
    return (std::string(""));
}

std::string GetValueBykeyServer(std::map<int, std::multimap<std::string, std::string>> servers, int indexOfserver, std::string key)
{
    std::map<int, std::multimap<std::string, std::string>>::iterator it;
    std::multimap<std::string, std::string>::iterator it2;

    for (it = servers.begin(); it != servers.end(); ++it)
    {
        if (it->first == indexOfserver)
        {
            for (it2 = it->second.begin(); it2 != it->second.end(); ++it2)
            {
                if (it2->first.find(key) != std::string::npos)
                    return (it2->second);
            }
        }
        // std::string res = locations.find(indexOfServer)->second.find()->second;
    }
    return (std::string(""));
}

int check_if_file_or_dir(std::string path)
{
    struct stat info;
    if (stat(path.c_str(), &info) == 0)
    {
        if (S_ISREG(info.st_mode))
            return (1);
        else if (S_ISDIR(info.st_mode))
            return (2);
    }
    return (-1);
}

bool is_location(std::multimap<int, std::multimap<std::string, std::string>>::iterator locations, std::string location)
{
    std::multimap<int, std::multimap<std::string, std::string>>::iterator it;
    std::multimap<std::string, std::string>::iterator it2;

    for (it2 = locations->second.begin(); it2 != locations->second.end(); ++it2)
    {
        if (it2->first.find("location") != std::string::npos && it2->second.find(location) != std::string::npos)
            return (true);
    }
    return (false);
}

int is_Path_exists(std::string path)
{
    std::ifstream ifs(path.c_str());
    return (ifs.good() ? 1 : 0);
}

int is_dir(std::string dir)
{
    struct stat st;
    if (stat(dir.c_str(), &st) == 0)
    {
        if (S_ISDIR(st.st_mode))
            return (1);
    }
    return (0);
}

std::string CreateAutoIndexHtmlFile(std::string path, std::string locatioName)
{

    std::string body = "<html><head> <link rel=\"shortcut icon\" href=\"data:image/x-icon;,\" type=\"image/x-icon\"> <title>Auto Index </title/> </head> <body> <h1> index of " + locatioName + " </h1>";
    //get all element in directory

    DIR *dir;
    struct dirent *ent;
    dir = opendir(path.c_str());
    if (dir == NULL)
    {
        return ("");
    }
    while ((ent = readdir(dir)) != NULL)
    {
        if (ent->d_name[0] == '.')
            continue;
        std::string tmp(ent->d_name);
        body += "<a href=\"" + tmp + "\">" + tmp + "</a><br>";
    }
    body += "</body></html>";
    closedir(dir);
    return (body);
}

std::string
getBodyFromFile(std::string path)
{
    std::string res = "";
    //read file
    std::ifstream ifs(path.c_str());
    //line by line
    std::string line;
    while (getline(ifs, line))
    {
        res += line + "\n";
    }
    return (res);
}

void execute_cgi(Response *response, int TargetServer, int TargetLocation, std::string root, Parsing *parsing, cgi *c, Request *request)
{
    std::multimap<int, std::multimap<std::string, std::string>> locations = parsing->Getloc_map();
    std::map<int, std::multimap<std::string, std::string>> servers = parsing->GetServerMap();

    if (GetValueBykeyLocation(locations, TargetServer, TargetLocation, "root") != "")
        root = GetValueBykeyLocation(locations, TargetServer, TargetLocation, "root");
    std::cout << "root: " << root + request->get_path() << std::endl;
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
        std::string Bodytmp = c->CGI(response, parsing->get_env());
        if (Bodytmp.find("\r\n\r\n") != std::string::npos)
            response->setBody(Bodytmp.substr(Bodytmp.find("\r\n\r\n")));
        else
            response->setBody(Bodytmp.substr(Bodytmp.find("()") + 2));
        response->setContentLength("");
    }
    else
        std::cout << " 404 found" << std::endl;
    return;
}

void SaveAsFile(std::string path, std::string body)
{
    std::ofstream file;
    file.open(path, std::ios_base::app);
    file << body;
    file.close();
}

void Server::_GetDataServers(Parsing *parsing, Response *response, Request *request)
{
    std::stringstream ss(someString);
    int t = 0;
    std::map<std::string, std::string> stor;

    // stor.clear();
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
            stor[tmp1] = tmp2.substr(0, tmp2.find_first_of(" "));
            t++;
        }
        else
        {
            t++;
            Content.push_back(line1);
        }
    }
    std::map<int, std::multimap<std::string, std::string>> servers = parsing->GetServerMap();
    std::multimap<int, std::multimap<std::string, std::string>> locations = parsing->Getloc_map();
    std::map<int, std::multimap<std::string, std::string>>::iterator it;
    std::multimap<int, std::multimap<std::string, std::string>>::iterator it2;
    //show data servers
    std::multimap<std::string, std::string>::iterator it3;
    std::multimap<std::string, std::string>::iterator it4;
    std::multimap<std::string, std::string> LocationContent;
    int indexOfServer = 1;
    int indexOfLocation = 1;
    std::string location_tmp = _GetFirstLocation(locations.begin());
    /*
        simulation Of data from Server Request
    */
    std::string Host = stor["Host"].substr(0, stor["Host"].find(":"));
    std::string Port = stor["Host"].substr(stor["Host"].find(":") + 1, stor["Host"].size());
    std::string Methode = (stor.find("GET") != stor.end() ? "GET" : (stor.find("POST") != stor.end()) ? "POST"
                                                                : (stor.find("DELETE") != stor.end()) ? "DELETE"
                                                                                                      : "UNKNOWN");
    std::string Path = stor[Methode];

    request->set_host(Host);
    request->set_port(Port);
    request->set_method(Methode);
    request->set_path(Path);
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
                    }
                    else if (pathLocation.find(".php") != std::string::npos)
                    {
                        if (location_tmp == "*.php")
                            execute_cgi(response, TargetServer, TargetLocation, root, parsing, c, request);
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
                                // someString = it->second;
                                if (FD_ISSET(sock_fd, &writefds))
                                {
                                    //Show Stor
                                    // std::cout << YEL << "***********************************" << reset << std::endl;
                                    // std::cout << someString << std::endl;
                                    // std::cout << YEL << "***********************************" << reset << std::endl;
                                    _GetDataServers(p, response, request);
                                    if (request->get_method() == "POST" || b == 1)
                                    {
                                        SaveAsFile("/home/hamza/Desktop/WebServe/output.txt",its->second);
                                        b=1;
                                    }
                                    // std::cout << "POST" << std::endl;
                                    // std::map<int, std::string>::iterator op = ips.begin();
                                    // for(op = ips.begin(); op != ips.end(); op++)
                                    // // {std::cout << op->first << " ==> " << op->second << std::endl;}
                                    // witch_server(ips, p);
                                    // std::map< int , std::multimap<std::string, std::string> > tmp = p->GetServerMap();
                                    // std::map< int , std::multimap<std::string, std::string> >::iterator  sp;
                                    // std::multimap<std::string, std::string>::iterator sp2;
                                    // for(sp = tmp.begin(); sp!= tmp.end(); sp++)
                                    // {
                                    //     std::cout << sp->first << std::endl;
                                    //     for(sp2 = sp->second.begin(); sp2 != sp->second.end(); sp2++)
                                    //     {
                                    //         if(sp2->first == "listen")
                                    //         {
                                    //         std::cout <<   sp2->first <<  "==>"<< sp2->second << std::endl;
                                    //         std::cout << " Host :" << stor["Host"] << std::endl;
                                    //             // if(sp2->second.find(stor["Host"]) != std::string::npos)
                                    //             //     {
                                    //             //         std::cout <<  sp->first << "==>" << sp2->second << std::endl;
                                    //             //         break;
                                    //             //     }

                                    //         }
                                    //         if(sp2->first == "server_addr")
                                    //         {
                                    //             std::cout <<   sp2->first <<  "==>"<< sp2->second << std::endl;
                                    //             std::cout << " Host :" << stor["Host"] << std::endl;
                                    //         }
                                    //     }
                                    //     std::cout << "" << std::endl;
                                    // }
                                    // loc = p->Getloc_map();
                                    // if (stor.find("GET") != stor.end())
                                    //     Get_methode(c, envp);
                                    // else if (stor.find("POST") != stor.end())
                                    //     Post_methode();
                                    // else if (stor.find("DELETE") != stor.end())
                                    //     Delete_methode();
                                    // std::cout << someString << std::endl;;

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