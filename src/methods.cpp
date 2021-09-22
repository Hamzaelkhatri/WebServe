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
#include "../includes/Webserv.hpp"
#include "../includes/request.hpp"
#include <dirent.h>

std::string Server::GetValueBykeyLocation(std::multimap<int, std::multimap<std::string, std::string>> locations, int indexOfServer, int indexOfLocation, std::string key)
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

std::string Server::_GetFirstLocation(std::multimap<int, std::multimap<std::string, std::string>>::iterator locations)
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

std::string Server::GetValueBykeyServer(std::map<int, std::multimap<std::string, std::string>> servers, int indexOfserver, std::string key)
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

int Server::check_if_file_or_dir(std::string path)
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

bool Server::is_location(std::multimap<int, std::multimap<std::string, std::string>>::iterator locations, std::string location)
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

std::string Server::CreateAutoIndexHtmlFile(std::string path, std::string locatioName)
{

    std::string body = "<html><head> <link rel=\"shortcut icon\" href=\"data:image/x-icon;,\" type=\"image/x-icon\"> <title>Auto Index </title/> </head> <body> <h1> index of " + locatioName + " </h1>";
    std::vector<std::string> files;
    DIR *dir;
    struct dirent *ent;
    dir = opendir(path.c_str());
    if (dir == NULL)
    {
        return ("");
    }
    while ((ent = readdir(dir)) != NULL)
    {
        std::string tmp(ent->d_name);
        files.push_back(tmp);
    }
    std::sort(files.begin(), files.end());
    for (int i = 0; i < files.size(); i++)
    {
        std::string tmp = files[i];
        body += "<a href=\"" + tmp + "\">" + tmp + "</a><br>";
    }
    body += "</body></html>";
    closedir(dir);
    return (body);
}

std::string Server::getBodyFromFile(std::string path)
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

void Server::execute_cgi(Response *response, int TargetServer, int TargetLocation, std::string root, Parsing *parsing, cgi *c, Request *request)
{
    std::multimap<int, std::multimap<std::string, std::string>> locations = parsing->Getloc_map();
    std::map<int, std::multimap<std::string, std::string>> servers = parsing->GetServerMap();

    if (GetValueBykeyLocation(locations, TargetServer, TargetLocation, "root") != "")
        root = GetValueBykeyLocation(locations, TargetServer, TargetLocation, "root");
    // std::cout << "root: " << root + request->get_path() << std::endl;
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

void Server::SaveAsFile(std::string path, std::string body, int b)
{
    std::ofstream file;
    file.open(path);
    file << body;
    file.close();
}
