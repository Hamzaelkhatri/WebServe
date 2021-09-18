/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorHandling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/21 16:02:50 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/18 16:18:21 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ErrorHandling.hpp"
#include "../includes/tools.hpp"

ErrorHandling::ErrorHandling(char *file)
{
    this->nbr_server = 0;
    this->nbr_location = 0;
    this->server = 0;
    this->location = 0;
    this->file = file;

    set_map(clean_map(get_map(file)));
}

int ErrorHandling::get_nbr_server(void)
{
    return (this->nbr_server);
}

int ErrorHandling::get_nbr_location(void)
{
    return (this->nbr_location);
}
void ErrorHandling::set_map(std::map<int, std::string> map)
{
    this->map_s = map;
}

std::map<int, std::string> ErrorHandling::get_map_s()
{
    // std::map<int, std::string>::iterator it = this->map_s.begin();
    // for (it = this->map_s.begin(); it != this->map_s.end(); ++it)
    // {
    //     std::cout << "[" << CYN << it->first << RESET;
    //     std::cout << "] \t\t\t ==> [";
    //     std::cout << CYN << it->second << RESET  << "]" << std::endl;
    // }
    return (this->map_s);
}

/****************  Those lines **********************************************/

int check_end_of_string(std::string str, int i, int j)
{
    while (i < j)
    {
        if (str[i] == '\0')
            return (0);
        i++;
    }
    return (1);
}

/****************************************************************************/

/************* Map *********************************************************/
std::map<int, std::string> get_map(char *av)
{
    int res = 0;
    std::map<int, std::string> map_s;
    std::string result;
    std::string file = av;
    char line1[1024];
    int fd = open(file.c_str(), O_RDONLY);
    if (fd < 0)
        throw ErrorHandling::FileNotFound();
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
                i++;
            }
        }
    }
    else
        throw std::runtime_error("Error opening file");   
    int i = 0;
    while (i < len)
    {
        map_s[i] = line[i]; // no spaces befor or after
        i++;
    }
    return (map_s);
}
/******************** Error *******************************************/
std::map<int, std::string> clean_map(std::map<int, std::string> error_mp)
{
    std::map<int, std::string>::iterator it = error_mp.begin();
    std::map<int, std::string> res;
    int i = 1;
    int m = 0;
    while (it != error_mp.end())
    {
        if (is_whitespace(it->second) == true)
        {
                it++;
        }
        else
        {
            res[i] = error_mp[it->first];
            i++;
            it++;
        }
    
    }
    int k = 0;
    for (it = res.begin(); it != res.end(); ++it)
    {
        m++;
        if (res[it->first].find("#") != std::string::npos)
            throw ErrorHandling::NoComment();
        if (m == 1 && res[it->first] != "server")
            throw ErrorHandling::StartWithServer();
        if (res[it->first] == "server" && res[it->first + 1] != "{")
            throw ErrorHandling::OpenBracket();
        if(res[it->first].find("location") != std::string::npos && res[it->first].find("}") != std::string::npos )
            throw ErrorHandling::CloseBracket();
        if(res[it->first].find("location") != std::string::npos && res[it->first].find("{") != std::string::npos )
            throw ErrorHandling::NewlineBracket();
        if((res[it->first].find("}") != std::string::npos || res[it->first].find("{") != std::string::npos) && res[it->first].length() > 1)
            throw std::runtime_error("Error the brackets are allowed just after Server and location  in newLine Not other place");
        if (res[it->first].find("}") != std::string::npos && res[it->first].length() ==  1)
            k++;
        if (res[it->first].find("{") != std::string::npos && res[it->first].length() ==  1)
            k--;
        if ( m > 0 && k != 0 && res[it->first] == "server")
           throw std::runtime_error("Error You can't put sever inside a server");
        if(res[it->first].find("location") != std::string::npos)
        {
            while(res[it->first].find("}") != std::string::npos)
            {
                if(res[it->first].find("location") != std::string::npos)
                    throw std::runtime_error("Error You can't put location inside location");
                if(res[it->first].find("server") != std::string::npos)
                    throw std::runtime_error("Error You can't put server inside location");
                it++;
            }
        }
    }
    if (k != 0)
        throw ErrorHandling::NumberBrackets();
    for(it = res.begin(); it != res.end(); ++it)
    {
        if(res[it->first] == "listen")
            throw std::runtime_error("Error Add a PORT infront of listen");
        if(res[it->first].find(":") != std::string::npos && res[it->first].find("listen") != std::string::npos && res[it->first].find(".") != std::string::npos)
            throw std::runtime_error("Error seperate Port in listen and the IP in server_addr");
        if(res[it->first].find(":") != std::string::npos && res[it->first].find("server_addr") != std::string::npos && res[it->first].find(".") == std::string::npos)
            throw std::runtime_error("Error seperate Port in listen and the IP in server_addr");
        if(res[it->first] == "server_name")
                res[it->first] += " localhost";
        if(res[it->first].find("\"") != std::string::npos)
            throw std::runtime_error("Error You can't put double quotes on Anything");
    }
    return (res);
}

/******************************************************************************/
ErrorHandling::~ErrorHandling()
{
}