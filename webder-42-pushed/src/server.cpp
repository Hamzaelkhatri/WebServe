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
        //Con
        if (line1.find_first_of(":") != std::string::npos && t == 1)
        {
            tmp1 = line1.substr(0, line1.find_first_of(":"));
            tmp2 = line1.substr(line1.find_first_of(":") + 1);
            stor[tmp1] = tmp2;
        }
        else if (t == 0)
        {
            // std::cout << line1 << std::endl;
            tmp1 = line1.substr(0, line1.find_first_of(" "));
            tmp2 = line1.substr(line1.find_first_of(" ") + 1);
            stor[tmp1] = tmp2.substr(0, tmp2.find_first_of(" "));
            t++;
        }
        else
        {
            t++;
            Content.push_back(line1);
            // std::cout << line1 << std::endl;
        }
    }
    //print stor variable without auto
    // for(std::map<std::string, std::string>::iterator it = stor.begin(); it != stor.end(); ++it)
    // {
    //     std::cout << it->first << " " << it->second << std::endl;
    // }
    return (stor);
}

int Server::GetTargetServer(Request *request, Parsing *parsing, std::string &root, std::multimap<std::string, std::string>::iterator it3, std::map<int, std::multimap<std::string, std::string> >::iterator it)
{
    std::map<int, std::multimap<std::string, std::string> > servers = parsing->GetServerMap();
    int TargetServer = 0;
    int i = 0;
    for (it3 = it->second.begin(); it3 != it->second.end(); ++it3)
    {
        if (it3->first.find("listen") != std::string::npos && request->get_port().find(it3->second) != std::string::npos)
            i -= 1;
        if (it3->first.find("server_addr") != std::string::npos && (request->get_host().find(it3->second) != std::string::npos || request->get_host().find("localhost") != std::string::npos))
            i -= 1;
        else if (it3->first.find("server_name") != std::string::npos && request->get_host().find(it3->second) != std::string::npos)
            i -= 1;
        if (i <= -2)
        {
            this->check_server = 1;
            TargetServer = it->first;
            root = GetValueBykeyServer(servers, TargetServer, "root");
            return (TargetServer);
        }
    }

    return (TargetServer);
}

int CountLines(std::string &file)
{
    std::stringstream in(file);
    std::string line;
    int count = 0;
    while (std::getline(in, line, '\n'))
        count++;
    return (count);
}

std::string DeleteHeaderPost(std::string &str)
{
    std::string Boundry = "";
    std::stringstream ss(str);
    std::string line;
    int j = 0;
    while (std::getline(ss, line, '\n'))
    {
        if (line.find("Content-Type: multipart/form-data; boundary=") != std::string::npos)
        {
            Boundry = line.substr(line.find("boundary=") + 9);
            Boundry = Boundry.substr(0, Boundry.find("\""));
            j = 1;
            break;
        }
    }
    if (!j)
        return ("");
    if (str.find("\r\n\r\n") != std::string::npos)
        str = str.substr(str.find("\r\n\r\n") + 4);
    if (str.find("\r\n\r\n") != std::string::npos)
        str = str.substr(str.find("\r\n\r\n"));

    std::stringstream ss2(str);
    std::string line2;
    std::string tmp = "";
    while (std::getline(ss2, line2, '\n'))
    {
        if (line2.find("--" + Boundry) != std::string::npos)
            break;
        else
            tmp += line2 + "\n";
    }
    return (tmp);
}

void Server::Post_Method(Request *request, Parsing *parsing, int indexOfServer, int indexOflocation, Response *response)
{
    std::map<int, std::multimap<std::string, std::string> > servers = parsing->GetServerMap();
    std::multimap<int, std::multimap<std::string, std::string> > locations = parsing->Getloc_map();
    if (request->get_method() == "POST")
    {
        std::string BodySize = GetValueBykeyServer(servers, indexOfServer, "client_body_size");
        std::string upload_path = GetValueBykeyLocation(locations, indexOfServer, indexOflocation, "upload_path");
        std::string upload_status = GetValueBykeyLocation(locations, indexOfServer, indexOflocation, "upload_status");
        std::string methode_http = GetValueBykeyLocation(locations, indexOfServer, indexOflocation, "http_methods");
        if (methode_http.find("POST") == std::string::npos)
        {
            response->setStatus("403");
            std::string root = GetValueBykeyServer(servers, indexOfServer, "root");
            std::string BodyTmp = getBodyFromFile(root + "/errors/403.html");
            response->setBody(BodyTmp);
            return;
        }
        if (upload_path == "")
            upload_path = "~/downloads";
        if (BodySize == "")
            BodySize = "10m";
        if (upload_status == "on" && request->get_content_lenght() >= 0 && std::stol(BodySize) >= 0 && request->get_content_lenght() <= (std::stol(BodySize) * 1048576) && request->get_filename()!= "")
        {
            its->second = DeleteHeaderPost(its->second);
            SaveAsFile(upload_path + request->get_filename(), its->second);
        }
        else if ((upload_status == "on" && request->get_content_lenght() >= 0 && std::stol(BodySize) >= 0) && request->get_content_lenght() > (std::stol(BodySize) * 1048576))
        {
            response->setStatus("413");
            response->setContentLength("");
            std::string root = "";
            root = GetValueBykeyServer(servers, indexOfServer, "root");
            std::string BodyTmp = getBodyFromFile(root + "/errors/413.html");
            response->setBody(BodyTmp);
            return;
        }
        else if (upload_status == "on" && request->get_content_lenght() == 0)
        {
            response->setStatus("411");
            response->setContentLength("");
            std::string root = "";
            root = GetValueBykeyServer(servers, indexOfServer, "root");
            std::string BodyTmp = getBodyFromFile(root + "/errors/411.html");
            return;
        }
        else if (its->second.find("Content-Type: multipart/form-data; boundary=") != std::string::npos && (BodySize == "0m"))
        {
            std::map<std::string, std::string>::iterator it = errors.begin();
            std::string root = GetValueBykeyServer(servers, indexOfServer, "root");
            int err_code = 0;
            for (it = errors.begin(); it != errors.end(); it++)
            {
                if (it->first == "400")
                {
                    std::string tmp = root + it->second;
                    response->setContentLength("");
                    response->setStatus("400");
                    if (check_if_file_or_dir(tmp) == 1)
                        response->setBody(getBodyFromFile(tmp));
                    else
                        response->setBody(getBodyFromFile(root + "/errors/400.html"));
                    err_code = 1;
                    return;
                }
            }
            if (err_code == 0)
            {
                root = GetValueBykeyServer(servers, indexOfServer, "root");
                response->setContentLength("");
                response->setStatus("400");
                std::string BodyTmp = getBodyFromFile(root + "/errors/400.html");
                response->setBody(BodyTmp);
                return;
            }
        }
        else if (BodySize == "0m" && request->get_content_lenght() > 0)
        {
            std::map<std::string, std::string>::iterator it = errors.begin();
            std::string root = GetValueBykeyServer(servers, indexOfServer, "root");
            int err_code = 0;
            for (it = errors.begin(); it != errors.end(); it++)
            {
                if (it->first == "400")
                {
                    std::string tmp = root + it->second;
                    response->setContentLength("");
                    response->setStatus("400");
                    if (check_if_file_or_dir(tmp) == 1)
                        response->setBody(getBodyFromFile(tmp));
                    else
                        response->setBody(getBodyFromFile(root + "/errors/400.html"));
                    err_code = 1;
                    return;
                }
            }
            if (err_code == 0)
            {
                root = GetValueBykeyServer(servers, indexOfServer, "root");
                response->setContentLength("");
                response->setStatus("400");
                std::string BodyTmp = getBodyFromFile(root + "/errors/400.html");
                response->setBody(BodyTmp);
                return;
            }
        }

    }
}

void Server::_GetDataServers(Parsing *parsing, Response *response, Request *request)
{
    stor = pars_request();
    std::map<int, std::multimap<std::string, std::string> > servers = parsing->GetServerMap();
    std::multimap<int, std::multimap<std::string, std::string> > locations = parsing->Getloc_map();
    std::map<int, std::multimap<std::string, std::string> >::iterator it;
    std::multimap<int, std::multimap<std::string, std::string> >::iterator it2;
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
    std::string cookies = stor["Cookie"];
    std::string Content_lenght = (stor.find("Content-Length") != stor.end()) ? stor["Content-Length"] : "0";
    std::string Content_Disposition = "";

     for (int j = 0; j < (int)Content.size(); j++)
    {
        if (Content[j].find("Content-Disposition") != std::string::npos)
        {
            Content_Disposition = Content[j].substr(Content[j].find("filename=") + 9, Content[j].size());
            break;
        }
    }
    std::string Path = stor[Methode];

    request->set_host(Host);
    request->set_port(Port);
    request->set_method(Methode);
    if (Path.find("?") != std::string::npos)
    {
        request->set_path(Path.substr(0, Path.find("?")));
        response->set_params(Path.substr(Path.find("?"), Path.size()));
    }
    else
        request->set_path(Path);
    request->set_content_lenght(std::stoi(Content_lenght));
    request->set_filename(Content_Disposition);
    cgi *c = NULL;
    response->setContentType("text/html");
    response->setVersion("HTTP/1.1");
    response->setCharset("UTF-8");
    response->setSetCookie(cookies);
    std::string root = "";
    int TargetServer = 0;
    int TargetLocation = 0;
    Content.clear();
    for (it = servers.begin(); it != servers.end(); it++)
    {
        indexOfLocation = 1;
        TargetServer = 1;

        TargetServer = GetTargetServer(request, parsing, root, it3, it);
        TargetLocation = 1;
        for (it2 = locations.begin(); it2 != locations.end(); it2++)
        {
            std::string pathLocation = request->get_path();
            if (it2->first == TargetServer)
            {
                location_tmp = _GetFirstLocation(it2);
                for (it4 = it2->second.begin(); it4 != it2->second.end(); it4++)
                {
                    TargetLocation = std::stoi(it4->first.substr(0, it4->first.find(" ")));
                    int i = 0;

                    if (pathLocation.find(".py") != std::string::npos)
                    {
                        if (location_tmp == "*.py")
                        {
                            i = execute_cgi(response, TargetServer, TargetLocation, root, parsing, c, request);
                            if (i == -1)
                            {
                                response->setStatus("404");
                                std::string BodyTmp = getBodyFromFile(root + "/errors/404.html");
                                response->setContentLength("");
                                response->setBody(BodyTmp);
                                return;
                            }
                            if (request->get_method() == "GET")
                            {
                                if (GetValueBykeyLocation(locations, TargetServer, TargetLocation, "http_methods") != "")
                                {
                                    std::string methode_http = GetValueBykeyLocation(locations, TargetServer, TargetLocation, "http_methods");
                                    if (methode_http.find("GET") == std::string::npos)
                                    {
                                        response->setStatus("405");
                                        std::string root = GetValueBykeyServer(servers, TargetServer, "root");
                                        std::string BodyTmp = getBodyFromFile(root + "/errors/405.html");
                                        response->setBody(BodyTmp);
                                        return;
                                    }
                                }
                                else
                                {
                                    response->setStatus("405");
                                    std::string root = GetValueBykeyServer(servers, TargetServer, "root");
                                    std::string BodyTmp = getBodyFromFile(root + "/errors/405.html");
                                    response->setBody(BodyTmp);
                                    return;
                                }
                            }
                            Post_Method(request, parsing, TargetServer, TargetLocation, response);
                            Delete_methode(request, parsing, TargetServer, TargetLocation, response);
                        }
                    }
                    else if (pathLocation.find(".php") != std::string::npos)
                    {
                        if (location_tmp == "*.php")
                        {
                            i = execute_cgi(response, TargetServer, TargetLocation, root, parsing, c, request);
                            if (i == -1)
                            {
                                response->setStatus("404");
                                std::string BodyTmp = getBodyFromFile(root + "/errors/404.html");
                                response->setContentLength("");
                                response->setBody(BodyTmp);
                                return;
                            }
                            if (request->get_method() == "GET")
                            {
                                if (GetValueBykeyLocation(locations, TargetServer, TargetLocation, "http_methods") != "")
                                {
                                    std::string methode_http = GetValueBykeyLocation(locations, TargetServer, TargetLocation, "http_methods");
                                    if (methode_http.find("GET") == std::string::npos)
                                    {
                                        response->setStatus("405");
                                        std::string root = GetValueBykeyServer(servers, TargetServer, "root");
                                        std::string BodyTmp = getBodyFromFile(root + "/errors/405.html");
                                        response->setBody(BodyTmp);
                                        return;
                                    }
                                }
                                else
                                {
                                    response->setStatus("405");
                                    std::string root = GetValueBykeyServer(servers, TargetServer, "root");
                                    std::string BodyTmp = getBodyFromFile(root + "/errors/405.html");
                                    response->setBody(BodyTmp);
                                    return;
                                }
                            }
                            Post_Method(request, parsing, TargetServer, TargetLocation, response);
                            Delete_methode(request, parsing, TargetServer, TargetLocation, response);
                            return;
                        }
                    }
                    else if (pathLocation.find(".php") == std::string::npos && pathLocation.find(".py") == std::string::npos)
                    {
                        if (pathLocation != "/" && pathLocation.find_last_of("/") == pathLocation.size() - 1)
                            pathLocation = pathLocation.substr(0, pathLocation.size() - 1);
                        if (location_tmp == pathLocation)
                        {
                            if (request->get_method() == "GET")
                            {
                                if (GetValueBykeyLocation(locations, TargetServer, TargetLocation, "http_methods") != "")
                                {
                                    std::string methode_http = GetValueBykeyLocation(locations, TargetServer, TargetLocation, "http_methods");
                                    if (methode_http.find("GET") == std::string::npos)
                                    {
                                        response->setStatus("405");
                                        std::string root = GetValueBykeyServer(servers, indexOfServer, "root");
                                        std::string BodyTmp = getBodyFromFile(root + "/errors/405.html");
                                        response->setBody(BodyTmp);
                                        break;
                                    }
                                }
                                else
                                {
                                    response->setStatus("405");
                                    std::string root = GetValueBykeyServer(servers, indexOfServer, "root");
                                    std::string BodyTmp = getBodyFromFile(root + "/errors/405.html");
                                    response->setBody(BodyTmp);
                                    break;
                                }
                            }
                            if (check_if_file_or_dir(root + request->get_path()) == 1)
                            {
                                if (GetValueBykeyLocation(locations, TargetServer, TargetLocation, "root") != "")
                                    root = GetValueBykeyLocation(locations, TargetServer, TargetLocation, "root");
                                if (GetValueBykeyLocation(locations, TargetServer, TargetLocation, "return") != "")
                                {
                                    std::string tmp = GetValueBykeyLocation(locations, TargetServer, TargetLocation, "return");
                                    std::string err = tmp.substr(0, tmp.find(" "));
                                    std::string err_path = tmp.substr(tmp.find_last_of(" "), sizeof(tmp));
                                    if (std::stoi(err) >= 300 && std::stoi(err) < 400)
                                    {
                                        response->setStatus(err);
                                        response->setRedirection("\nlocation:" + err_path + "");
                                        response->setPath(root + request->get_path() + "");
                                    }
                                }
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

                                if (GetValueBykeyLocation(locations, TargetServer, TargetLocation, "root") != "")
                                    root = GetValueBykeyLocation(locations, TargetServer, TargetLocation, "root");
                                if (GetValueBykeyLocation(locations, TargetServer, TargetLocation, "index") != "")
                                {
                                    if (Path[Path.size() - 1] != '/')
                                    {

                                        response->setStatus("301"); //redirection
                                        response->setRedirection("\nlocation:" + Path + "/");
                                        response->setPath(root + request->get_path() + "/");
                                        return;
                                    }
                                    std::string BodyTmp = getBodyFromFile(root + request->get_path() + GetValueBykeyLocation(locations, TargetServer, TargetLocation, "index"));
                                    if (BodyTmp.empty())
                                    {
                                        std::map<std::string, std::string>::iterator it = errors.begin();
                                        int err_code = 0;
                                        for (it = errors.begin(); it != errors.end(); it++)
                                        {
                                            if (it->first == "403")
                                            {
                                                std::string tmp = root + it->second;
                                                response->setContentLength("");
                                                response->setStatus("403");
                                                if (check_if_file_or_dir(tmp) == 1)
                                                    response->setBody(getBodyFromFile(tmp));
                                                else
                                                    response->setBody(getBodyFromFile(root + "/errors/403.html"));
                                                err_code = 1;
                                                return;
                                            }
                                        }
                                        if (err_code == 0)
                                        {
                                            root = GetValueBykeyServer(servers, TargetServer, "root");
                                            response->setContentLength("");
                                            response->setStatus("403");
                                            std::string BodyTmp = getBodyFromFile(root + "/errors/403.html");
                                            response->setBody(BodyTmp);
                                            return;
                                        }
                                    }
                                    response->setBody(BodyTmp);
                                    if (GetValueBykeyLocation(locations, TargetServer, TargetLocation, "return") != "")
                                    {
                                        std::string tmp = GetValueBykeyLocation(locations, TargetServer, TargetLocation, "return");
                                        std::string err = tmp.substr(0, tmp.find(" "));
                                        std::string err_path = tmp.substr(tmp.find_last_of(" "), sizeof(tmp));
                                        if (std::stoi(err) >= 300 && std::stoi(err) < 400)
                                        {
                                            response->setStatus(err);
                                            if(err_path != "")
                                            {
                                                response->setRedirection("\nlocation:" + err_path + "/");
                                                response->setPath(root + request->get_path() + "/");
                                                return;
                                            }
                                        }
                                    }
                                    response->setCookie("");
                                    response->setSetCookie("");
                                    response->setContentLength("");
                                    body = response->getBody();
                                }
                                else if (GetValueBykeyLocation(locations, TargetServer, TargetLocation, "autoindex") != "" && GetValueBykeyLocation(locations, TargetServer, TargetLocation, "autoindex") == "on")
                                {
                                    if (GetValueBykeyLocation(locations, TargetServer, TargetLocation, "root") != "")
                                        root = GetValueBykeyLocation(locations, TargetServer, TargetLocation, "root");
                                    response->setBody(CreateAutoIndexHtmlFile(root + request->get_path(), pathLocation));
                                    response->setStatus("");
                                    response->setContentLength("");
                                    if (Path[Path.size() - 1] != '/')
                                    {
                                        response->setStatus("301"); //moved permanently
                                        response->setRedirection("\nlocation:" + Path + "/");
                                        std::string BodyTmp = getBodyFromFile(root + "/errors/301.html");
                                        response->setBody(BodyTmp);
                                        return;
                                    }
                                    if (GetValueBykeyLocation(locations, TargetServer, TargetLocation, "return") != "")
                                    {
                                        std::string tmp = GetValueBykeyLocation(locations, TargetServer, TargetLocation, "return");
                                        std::string err = tmp.substr(0, tmp.find(" "));
                                        std::string err_path = tmp.substr(tmp.find_last_of(" "), sizeof(tmp));
                                        if (std::stoi(err) >= 300 && std::stoi(err) < 400)
                                        {
                                            response->setStatus(err);
                                            // if(err_path != "")
                                            {
                                                response->setRedirection("\nlocation:" + err_path + "");
                                                response->setPath(root + request->get_path() + "");
                                                return;
                                            }
                                        }
                                    }
                                }
                            }
                            else
                            {
                                std::map<std::string, std::string>::iterator it = errors.begin();
                                int err_code = 0;
                                for (it = errors.begin(); it != errors.end(); it++)
                                {
                                    if (it->first == "404")
                                    {
                                        // std::cout << it->second << std::endl;
                                        response->setContentLength("");
                                        response->setStatus("404");
                                        std::string tmp = root + it->second;
                                        if (check_if_file_or_dir(tmp) == 1)
                                            response->setBody(getBodyFromFile(tmp));
                                        else
                                            response->setBody(getBodyFromFile(root + "/errors/404.html"));
                                        err_code = 1;
                                        break;
                                    }
                                }
                                if (err_code == 0)
                                {
                                    response->setStatus("404");
                                    response->setBody(getBodyFromFile(root + "/errors/404.html"));
                                    response->setContentLength("");
                                    break;
                                }
                            }
                            Post_Method(request, parsing, TargetServer, TargetLocation, response);
                            Delete_methode(request, parsing, TargetServer, TargetLocation, response);
                            break;
                        }
                        else
                        {
                            if (pathLocation.find_last_of("/") != std::string::npos && TargetLocation > 0 && TargetServer > 0)
                                pathLocation = pathLocation.substr(0, pathLocation.find_last_of("/"));
                            else
                                pathLocation = "/";
                        }
                    }
                }
                TargetLocation++;
            }
            indexOfServer++;
        }
    }
    if (check_server == 0)
    {
        std::map<std::string, std::string>::iterator it = errors.begin();
        int err_code = 0;
        for (it = errors.begin(); it != errors.end(); it++)
        {
            if (it->first == "403")
            {

                response->setContentLength("");
                response->setStatus("403");
                std::string tmp = root + it->second;
                if (check_if_file_or_dir(tmp) == 1)
                    response->setBody(getBodyFromFile(tmp));
                else
                    response->setBody(getBodyFromFile(root + "/errors/403.html"));
                err_code = 1;
                break;
            }
        }
        if (err_code == 0)
        {
            response->setContentLength("");
            response->setStatus("403");
            std::string BodyTmp = getBodyFromFile(root + "/errors/403.html");
            response->setBody(BodyTmp);
        }
    }
}

Server::Server(Parsing *p, char **envp)
{
    std::string version;
    Response *response = new Response();

    this->sock = new Socket(p);
    maxfd = 0;
    int rc = 0;
    this->check_server = 0;
    char buffer[BUFFER_SIZE + 1];
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
    chunked = 0;
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
                    for (int j = 0; j < (int)MasterSockets.size(); j++)
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
                            if (checkRequest(its->second) == true)
                            {
                                if (FD_ISSET(sock_fd, &writefds))
                                {
                                    if (chunked == 1)
                                    {
                                        unchunkRequest(its->second, response);
                                        someString = its->second;
                                        chunked = 0;
                                    }
                                    if (!check_header(its->second))
                                    {
                                        response->setBody("<html>\n<body>\n<h1>400 Bad Request</h1>\n</body>\n</html>\n");
                                        response->setStatus("400");
                                        response->setContentLength("");
                                    }
                                    _GetDataServers(p, response, request);

                                    std::string header = response->getVersion() + " " + response->getStatus() + "\nContent-type: text/html; charset=utf-8 ; charset= " + response->getCharset() + response->getRedirection() + "\nContent-Length: " + std::to_string(response->getBody().size()) + "\nSet-Cookie: " + response->getSSID() + "\nSet-Cookie: " + response->get_params() + "\n\n" + response->getBody();
                                    send(sock_fd, header.c_str(), header.size(), 0);
                                    its->second.clear();
                                    this->check_server = 0;
                                    response->setStatus("");
                                }
                            }
                        }
                        else
                        {
                            if (rc == 0)
                            {
                                b = 0;
                                its->second.clear();

                                std::cout << RED << "[-]"
                                          << reset << "Client Diconnected " << std::endl;
                                close(sock_fd);
                                FD_CLR(sock_fd, &masterfds);
                                FD_CLR(sock_fd, &writefds);
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