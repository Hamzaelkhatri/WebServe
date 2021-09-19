/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/22 19:03:57 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/19 12:39:30 by zdnaya           ###   ########.fr       */
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

void Parsing::set_serverMap(std::map<int, std::multimap<std::string, std::string> > srv, std::multimap<int, std::multimap<std::string, std::string> > loc)
{
    int addr = 0;
    int port = 0;
    int name = 0;
    int root = 0;
    std::map<int, std::multimap<std::string, std::string> >::iterator it;
    std::multimap<std::string, std::string>::iterator it1;
    srv = this->_server_map;
    loc = this->_loc_map;
    for (it = srv.begin(); it != srv.end(); it++)
    {
        for (it1 = it->second.begin(); it1 != it->second.end(); ++it1)
        {
            // std::cout << "|" << it->first << "|[" << it1->first << "]|" << it1->second << "|" << std::endl;
            if (it1->first == "listen")
                port++;
            if (it1->first == "server_addr")
                addr++;
            if (it1->first == "server_name")
                name++;
            if (it1->first == "root")
                root++;

        }
            std::cout  << it->first << "port |" << port << "| addr |" << addr << "| name |" << name << "| root |" << root << std::endl;
        // if (port == 0 || addr == 0)
        //     throw std::runtime_error("add listne and server_addr");
        // if (name == 0)
        //     it->second.insert(std::pair<std::string, std::string>("server_name", "localhost"));
        
    }
    this->_server_map = srv;
    // std::multimap<int, std::multimap<std::string, std::string> >::iterator it2;
    // for (it2 = loc.begin(); it2 != loc.end(); it2++)
    // {
    //     for (it1 = it2->second.begin(); it1 != it2->second.end(); ++it1)
    //     {
    //         // std::cout << it1->first << std::endl;
    //         if (it1->first.find("root") != std::string::npos)
    //             root++;
    //     }
    // }
    // if (root == 0)
    //     throw std::runtime_error("No root element");
    // if (loc.size() == 0)
    // {
    //     std::multimap<std::string, std::string> tmp;
    //     tmp.insert(std::pair<std::string, std::string>("location", "/"));
    //     tmp.insert(std::pair<std::string, std::string>("index", "/index.html"));
    //     tmp.insert(std::pair<std::string, std::string>("method", "GET"));
    //     loc.insert(std::pair<int, std::multimap<std::string, std::string> >(1, tmp));
    //     this->_loc_map = loc;
    // }
}

void Parsing::set_env(char **env)
{
    this->env = env;
}

char **Parsing::get_env()
{
    return (this->env);
}

std::vector<std::string> split(std::string s, std::string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
    {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

Parsing::Parsing(char *av)
{
    int z = 0;
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
                i++;
            }
        }
    }
    else
        throw std::runtime_error("Error opening file");
    int l = 0;
    int serverIndex = 0;
    int d = 0;
    int locIndex = 0;
    int i = 0;
    int s = 0;
    while (i < len)
    {
        if (line[i] == "server")
        {
            s = 0;
            i++;
            serverIndex++;
            while (line[i] == "{" || line[i] == "}")
                i++;
            while (line[i] != "server" && i < len && line[i].find("location") == std::string::npos)
            {
                locIndex = 0;
                while (line[i] == "{" || line[i] == "}")
                    i++;
                if (line[i].find("location") == std::string::npos)
                {
                    std::string str;
                    std::string delimiter = " ";
                    std::vector<std::string> tmp = split(line[i], delimiter);
                    std::string key = "";
                    int j = 0;
                    for (j = 0; j < tmp.size(); j++)
                    {
                        trim(tmp[j]);
                        if (tmp[j].size() > 0)
                        {
                            if (j == 0 )
                                key = tmp[j];
                            else
                                str += " " + tmp[j];
                        }
                    }
                    if(str.size() > 0 && key.size() > 0)
                        this->server_map.insert(std::pair<std::string, std::string>(key, str));
                }
                i++;
            }
            std::multimap<std::string, std::string> tmp;
            tmp = this->server_map;
            this->_server_map[serverIndex] = tmp;
            this->server_map.clear();
            i--;
        }
        if (line[i].find("location") != std::string::npos)
        {
            s++;
            locIndex++;
            while (line[i].find("}") == std::string::npos)
            {
                if (line[i] != "}" && line[i] != "{")
                {
                    std::string str;
                    std::string delimiter = " ";

                    std::vector<std::string> tmp = split(line[i], delimiter);
                    std::string key = "";
                    int j = 0;
                    for (j = 0; j < tmp.size(); j++)
                    {
                        trim(tmp[j]);

                        if (j == 0 && tmp.size() >= 1)
                            key = tmp[j];
                        else if (tmp.size() >= 1)
                            str += " " + tmp[j];
                    }
                    this->loc_map.insert(std::pair<std::string, std::string>(std::to_string(locIndex) + " " + key, str));
                }
                i++;
            }
            std::multimap<std::string, std::string> tmp1;
            tmp1 = this->loc_map;
            this->_loc_map.insert(std::pair<int, std::multimap<std::string, std::string> >(serverIndex, tmp1));
            this->loc_map.clear();
            i--;
        }
        if (s == 0)
        {
            std::multimap<std::string, std::string> tmp;
            this->loc_map.insert(std::pair<std::string, std::string>(std::to_string(1) + " location", "/"));
            this->loc_map.insert(std::pair<std::string, std::string>(std::to_string(1) + " index", "index.html"));
            this->loc_map.insert(std::pair<std::string, std::string>(std::to_string(1) + " method", "GET"));
            tmp = this->loc_map;
            this->_loc_map.insert(std::pair<int, std::multimap<std::string, std::string> >(serverIndex, tmp));
            this->loc_map.clear();
        }

        i++;
    }
    myfile.close();
    std::cout << "Number of servers: " << serverIndex << std::endl;
}

Parsing::~Parsing()
{
}
