/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/22 19:03:57 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/30 17:29:33 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pars.hpp"
#include <fstream>

Server_element::Server_element()
{
    this->port = 0;
}

std::multimap<int, std::multimap<std::string, std::string> > Parsing::Getloc_map()
{
    return (this->_loc_map);
}

std::map<int, std::multimap<std::string, std::string> > Parsing::GetServerMap()
{
    return (this->_server_map);
}

int SearchIntoVector(std::vector<int> index, int i)
{
    for (int j = 0; j < (int)index.size(); j++)
    {
        if (index[j] == i)
        {
            return (1);
        }
    }
    return (0);
}

void Parsing::set_serverMap(std::map<int, std::multimap<std::string, std::string> > srv, std::multimap<int, std::multimap<std::string, std::string> > loc)
{
    int addr = 0;
    int port = 0;
    int name = 0;
    int root = 0;
    std::map<int, std::multimap<std::string, std::string> >::iterator it;
    std::multimap<std::string, std::string>::iterator it1;
    srv = this->_server_map;
    int indexOfServer = 1;
    std::vector<int> TargetServer;
    for (it = srv.begin(); it != srv.end(); it++)
    {
        // std::cout << "\t\tServer\t\t" << it->first << ":" << std::endl;
        port = 0;
        addr = 0;
        root = 0;
        name = 0;
        for (it1 = it->second.begin(); it1 != it->second.end(); ++it1)
        {
            // std::cout << "\t|" << it1->first << "|\t\t\t|" << it1->second << "|"
            //           << "\n\n";
            if (it1->first == "listen")
                port++;
            if (it1->first == "server_addr")
                addr++;
            if (it1->first == "root")
                root++;
            if (it1->first == "server_name")
                name++;
        }
        if (port == 0)
            throw std::runtime_error("No port in server in server nbr " + std::to_string(it->first));
        if (addr == 0)
            throw std::runtime_error("No address  in server nbr " + std::to_string(it->first));
        if (root == 0)
            TargetServer.push_back(it->first);
        if (addr > 1)
            throw std::runtime_error("More than one server_addr in server nbr " + std::to_string(it->first));
        if (name > 1)
            throw std::runtime_error("More than one server_name in server nbr " + std::to_string(it->first));
        indexOfServer++;
    }

    loc = this->_loc_map;
    std::multimap<int, std::multimap<std::string, std::string> >::iterator it2;
    int indexOfLoc = 1;
    for (it2 = loc.begin(); it2 != loc.end(); it2++)
    {
        for (it1 = it2->second.begin(); it1 != it2->second.end(); ++it1)
        {
            if (it1->first.find("root") != std::string::npos)
                root++;
        }
        if (root == 0 && SearchIntoVector(TargetServer, it2->first))
        {
            it = srv.begin();
            for (it = srv.begin(); it != srv.end(); it++)
            {
                if (it->first == it2->first)
                    it->second.insert(it1, std::pair<std::string, std::string>("root", "/Users/helkhatr/Desktop/WebServe/webpage"));
                break;
            }
        }
        root = 0;
        indexOfLoc++;
    }
    this->_loc_map = loc;
    this->_server_map = srv;
}

void Parsing::set_env(char **env)
{
    this->env = env;
}

char **Parsing::get_env()
{
    return (this->env);
}

Parsing::Parsing(char *av)
{
    int res = 0;
    std::map<int, std::string> map_s;
    std::string result = "";
    std::string file = av;
    char line1[1024];
    int fd = open(file.c_str(), O_RDONLY);
    while ((res = read(fd, line1, 1024)) > 0)
    {
        if (std::strcmp(line1, "\n") != 0)
            result = result + line1;
        int i = 0;
        while (i < 1024)
            line1[i++] = 0;
    }
    int len = nbr_lines(result);
    std::string line[len];
    std::ifstream myfile;
    int nbr_server = 0;
    myfile.open(file);
    if (myfile.is_open())
    {
        int i = 0;
        std::string tmp;
        while (!myfile.eof() && i < len)
        {
            std::getline(myfile, tmp);
            if (trim(tmp).length() > 0)
            {
                line[i] = trim(tmp);
                if (line[i] == "server")
                    nbr_server++;
                // std::cout << "|" << line [i]<< "|" << std::endl;
                i++;
            }
        }
    }
    else
        throw std::runtime_error("Error opening file");
    int serverIndex = 0;
    int locIndex = 0;
    int i = 0;
    int s = 0;
    int t = 0;
    while (i < len)
    {
        std::string key = "";
        std::string str = "";
        if (line[i] == "server")
        {
            i++;
            serverIndex++;
            while (line[i] == "{" || line[i] == "}")
                i++;
            while (line[i] != "server" && i < len && line[i].find("location") == std::string::npos)
            {
                key = "";
                str = "";
                locIndex = 0;
                while (line[i] == "{" || line[i] == "}")
                    i++;
                if (line[i] != "" && line[i].find("location") == std::string::npos)
                {
                    key = line[i].substr(0, line[i].find(" "));
                    str = line[i].substr(key.length() + 1, line[i].length());
                    this->server_map.insert(std::pair<std::string, std::string>(trim(key), trim(str)));
                }
                i++;
            }
            std::multimap<std::string, std::string> tmp;
            tmp = this->server_map;
            this->_server_map[serverIndex] = tmp;
            this->server_map.clear();
            s = 1;
        }
        int loc = 1;
        t = i;
        if (line[i].find("location") != std::string::npos)
        {
            // s++;
            locIndex++;
            while (line[i].find("}") == std::string::npos)
            {
                std::string key = "";
                std::string str = "";
                if (line[i] != "" && line[i] != "}" && line[i] != "{")
                {
                    key = line[i].substr(0, line[i].find(" "));
                    str = line[i].substr(key.length() + 1, line[i].length());
                    this->loc_map.insert(std::pair<std::string, std::string>(std::to_string(locIndex) + " " + trim(key), trim(str)));
                }
                i++;
            }
            std::multimap<std::string, std::string> tmp1;
            tmp1 = this->loc_map;
            this->_loc_map.insert(std::pair<int, std::multimap<std::string, std::string> >(serverIndex, tmp1));
            this->loc_map.clear();
            i--;
        }
        if (i + 2 < len && line[i + 2] != "" && line[i + 2].find("{") == std::string::npos && line[i + 2].find("location") == std::string::npos && line[i + 2].find("server") == std::string::npos && line[i + 2].find("}") == std::string::npos)
        {
            key = line[i + 2].substr(0, line[i + 2].find(" "));
            str = line[i + 2].substr(key.length() + 1, line[i + 2].length());
            this->_server_map[serverIndex].insert(std::pair<std::string, std::string>(trim(key), trim(str)));
            this->server_map.clear();
            loc = 0;
        }
        i++;
    }
    myfile.close();

    set_serverMap(this->_server_map, this->_loc_map);
}

Parsing::~Parsing()
{
}