/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/22 19:03:57 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/17 19:35:54 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pars.hpp"
#include <fstream>

Server_element::Server_element()
{
    this->port = 0;
}

std::multimap <int, std::multimap<std::string, std::string> > Parsing::Getloc_map()
{
    return (this->_loc_map);
}

std::map< int , std::multimap<std::string, std::string> > Parsing::GetServerMap()
{
    return (this->_server_map);
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
    int  z = 0;

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
    myfile.open(file);
    if(myfile.is_open())
    {
        int i = 0;
        std::string tmp;
        while (!myfile.eof() &&  i < len)
        {
            std::getline(myfile, tmp);
            if(trim(tmp).length() > 0) 
            {   
                line[i] = trim(tmp);
                // std::cout << "|" << line [i]<< "|" << std::endl;   
                i++;
            }
        }
    }
    else
        throw std::runtime_error("Error opening file");    
    char **ptr = new char * [len + 1];
    int l = 0;
    int serverIndex = 0;
    int d = 0;
    int locIndex = 0;
    int i = 0;
    while (i < len)
    {
            
        if(line[i] ==  "server")
        {
            i++;
            serverIndex++;
            while(line[i] == "{" ||  line[i] ==  "}")
                    i++;
            while(line[i] !=  "server"   && i < len && line[i].find("location") == std::string::npos)
            {
                locIndex = 0;
                  while(line[i] ==  "{" ||  line[i] == "}")
                    i++;
                if(line[i] != "" &&  line[i].find("location") == std::string::npos )
                {
                    char **ptr = ft_charSplit(line[i].c_str(), (char *)"\t ");
                    std::string str ;
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
            this->_server_map[serverIndex] = tmp ;
            this->server_map.clear();      
            }
            
                  if (line[i].find("location") != std::string::npos)
                {
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
                            this->loc_map.insert(std::pair<std::string, std::string>(std::to_string(locIndex)+" "+key, str));
                        }
                        i++;
                    }
            std::multimap<std::string, std::string> tmp1;
            tmp1 = this->loc_map;
            this->_loc_map.insert(std::pair<int, std::multimap<std::string, std::string> >(serverIndex, tmp1));
            this->loc_map.clear();
            i--;
            }
            i++;
    }
    myfile.close();
}

Parsing::~Parsing()
{
}

