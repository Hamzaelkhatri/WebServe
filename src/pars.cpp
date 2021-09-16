/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/22 19:03:57 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/16 12:16:49 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pars.hpp"

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

Parsing::Parsing(char *av)
{
    int  z = 0;

    int res = 0;
    std::map<int, std::string> map_s;
    std::string result;
    std::string file = av;
    char line[1024];
    int fd = open(file.c_str(), O_RDONLY);
    if (fd < 0)
        throw std::runtime_error("Error opening file");
    while ((res = read(fd, line, 1024)) > 0)
    {
        if (std::strcmp(line, "\n") != 0)
            result = result + line;
        int i = 0;
        while (i < 1024)
            line[i++] = 0;
    }
    int len = nbr_lines(result);
    int i = 0;
    char **ptr = new char * [len + 1];
    int l = 0;
    int serverIndex = 0;
    int d = 0;
    int locIndex = 0;
    while (i < len )
    {
        // while(std::strcmp(Those_lines(result, i, len).c_str(), "") == 0 )
        //             i++;
        if(std::strcmp(Those_lines(result, i, len).c_str(), "server") == 0 )
        {
            i++;
            serverIndex++;
            while(std::strcmp(Those_lines(result, i, len).c_str(), "{") == 0 ||  std::strcmp(Those_lines(result, i, len).c_str(), "}") == 0)
                    i++;
            while(Those_lines(result, i, len) !=  "server"   && i < len && Those_lines(result, i, len).find("location") == std::string::npos)
            {
                locIndex = 0;
                  while(std::strcmp(Those_lines(result, i, len).c_str(), "{") == 0 ||  std::strcmp(Those_lines(result, i, len).c_str(), "}") == 0)
                    i++;
                if(Those_lines(result, i, len) != "" &&  Those_lines(result, i, len).find("location") == std::string::npos )
                {
                    char **ptr = ft_charSplit(Those_lines(result, i, len).c_str(), (char *)" \t");
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
            
                  if (Those_lines(result, i, len).find("location") != std::string::npos)
                {
                    locIndex++;
                    while (Those_lines(result, i, len).find("}") == std::string::npos)
                    {
                        if (Those_lines(result, i, len) != "" && Those_lines(result, i, len) != "}" && Those_lines(result, i, len) != "{")
                        {
                            char **ptr = ft_charSplit(Those_lines(result, i, len).c_str(), (char *)" \t");
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
}

Parsing::~Parsing()
{
}

