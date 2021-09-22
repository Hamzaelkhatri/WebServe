/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zainabdnayagmail.com <zainabdnayagmail.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/22 19:03:57 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/22 10:01:01 by zainabdnaya      ###   ########.fr       */
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
    for (int j = 0; j < index.size(); j++)
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
            // std::cout << "\t|" << it1->first << "|\t\t\t|" << it1->second << "|" << "\n\n";
            if (it1->first == "listen")
                port++;
            if (it1->first == "server_addr")
                addr++;
            if (it1->first == "root")
                root++;
            if(it1->first == "server_name")
                name++;
        }
        if (port == 0)
            throw std::runtime_error("No port in server in server nbr " + std::to_string(it->first));
        if (addr == 0)
            throw std::runtime_error("No address  in server nbr " + std::to_string(it->first));
        if (root == 0)
            TargetServer.push_back(it->first);
        if(addr > 1)
            throw std::runtime_error("More than one server_addr in server nbr " + std::to_string(it->first));
        if(name > 1)
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
            // std::cout << " I m here" << std::endl;
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
                // std::cout << "|" << line [i]<< "|" << std::endl;
                i++;
            }
        }
    }
    else
        throw std::runtime_error("Error opening file");
    char **ptr = new char *[len + 1];
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

            i++;
            serverIndex++;
            while (line[i] == "{" || line[i] == "}")
                i++;
            while (line[i] != "server" && i < len && line[i].find("location") == std::string::npos)
            {
                locIndex = 0;
                while (line[i] == "{" || line[i] == "}")
                    i++;
                if (line[i] != "" && line[i].find("location") == std::string::npos)
                {
                    char **ptr = ft_charSplit(line[i].c_str(), (char *)"\t ");
                    std::string str;
                    std::string key = ptr[0];
                    int k = 1;
                    while (ptr[k])
                    {
                        if (k == 1)
                            str = ptr[k];
                        else
                            str = str + " " + ptr[k];
                        k++;
                    }
                    this->server_map.insert(std::pair<std::string, std::string>(key, str));
                }
                i++;
            }
            std::multimap<std::string, std::string> tmp;
            tmp = this->server_map;
            this->_server_map[serverIndex] = tmp;
            this->server_map.clear();
            s = 0;
        }
        if (line[i].find("location") != std::string::npos)
        {
            s++;
            locIndex++;
            while (line[i].find("}") == std::string::npos)
            {
                if (line[i] != "" && line[i] != "}" && line[i] != "{")
                {
                    char **ptr = ft_charSplit(line[i].c_str(), (char *)" \t");
                    std::string str;
                    std::string key = ptr[0];
                    int k = 1;
                    while (ptr[k])
                    {
                        if (k == 1)
                            str = ptr[k];
                        else
                            str = str + " " + ptr[k];
                        k++;
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
        // if (s == 0 && line[i] == "}")
        // {
        //     std::multimap<std::string, std::string> tmp;
        //     locIndex++;
        //     this->loc_map.insert(std::pair<std::string, std::string>(std::to_string(locIndex) + " location", "/"));
        //     this->loc_map.insert(std::pair<std::string, std::string>(std::to_string(locIndex) + " index", "/index.html"));
        //     this->loc_map.insert(std::pair<std::string, std::string>(std::to_string(locIndex) + " method", "GET"));
        //     tmp = this->loc_map;
        //     this->_loc_map.insert(std::pair<int, std::multimap<std::string, std::string> >(serverIndex, tmp));
        //     this->loc_map.clear();
        //     //  i--;
        // }

        i++;
    }
    myfile.close();
    set_serverMap(this->_server_map, this->_loc_map);
}

Parsing::~Parsing()
{
}
