/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorHandling.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/21 16:02:50 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/08/31 17:45:22 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ErrorHandling.hpp"

ErrorHandling::ErrorHandling(char *file)
{
    this->nbr_server = 0;
    this->nbr_location = 0;
    this->server = 0;
    this->location = 0;
    this->file = file;

    set_map(clean_map(get_map(file)));
    // get_map_s();
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
    std::map<int, std::string>::iterator it = this->map_s.begin();
    // for (it = this->map_s.begin(); it != this->map_s.end(); ++it)
    {
        // if (it->second == "server")
            // this->nbr_server++;
        // if (it->second.find("location") != std::string::npos)
            // this->nbr_location++;
        // std::cout << "[ " << it->first;
        // std::cout << " ] \t\t\t ==> [ ";
        // std::cout << it->second << " ]" << std::endl;
    }
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

std::string Those_lines(std::string txt, int nbr_line, int txt_lines)
{
    int i = 0;
    int j = 0;
    int w = 0;
    if (nbr_line >= txt_lines)
        return ("");
    while (i < nbr_line)
    {
        if (txt[j] == '\n')
            i++;
        j++;
    }
    while (txt[j] != '\n' && std::isspace(txt[j])) // for the space befor  txt start from where the txt begins
        j++;
    w = 0;
    while (txt[j + w] && txt[w + j] != '\n' && std::isprint(txt[j + w])) // when the line ends until \n
    {
        w++;
    }
    while (w > 0 && std::isspace(txt[w + j - 1])) // escape space after the txt o the line
    {
        w--;
    }
    return (std::string(txt, j, w));
}
/****************************************************************************/

/************* Map *********************************************************/
std::map<int, std::string> get_map(char *av)
{
    int res = 0;
    std::map<int, std::string> map_s;
    std::string result;
    std::string file = av;
    char line[1024];
    int fd = open(file.c_str(), O_RDONLY);
    if (fd < 0)
        error_msg("Error file not found");
    while ((res = read(fd, line, 1024)) > 0)
    {
        if (std::strcmp(line, "\n") != 0)
            result = result + line;
        int i = 0;
        while (i < 1024)
            line[i++] = 0;
    }
    int i = 0;
    int len = nbr_lines(result);
    while (i < len)
    {
        map_s[i] = Those_lines(result, i, (len)); // no spaces befor or after
        i++;
    }
    return (map_s);
}
/**************************************************************************/

/******************** Error *******************************************/

std::map<int, std::string> clean_map(std::map<int, std::string> error_mp)
{
    std::map<int, std::string>::iterator it = error_mp.begin();
    std::map<int, std::string> res;
    int i = 1;
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
        if (res[it->first].find("#") != std::string::npos)
            error_msg("Error: no comment allowed");
        if (it->first == 1 && res[it->first] != "server")
            error_msg("Error : file should start with server");
        if (res[it->first] == "server" && res[it->first + 1] != "{")
            error_msg("Error : after server should be only  open bracket");
        if( (res[it->first].find("location") != std::string::npos ) &&    res[it->first].find("}") != std::string::npos )
                error_msg("Error : close  bracket in new line alone");
        if( (res[it->first].find("location") != std::string::npos ) &&    res[it->first].find("{") != std::string::npos )
                error_msg("Error : open bracket in new line alone");    
        if (res[it->first].find("}") != std::string::npos)
            k++;
        if (res[it->first].find("{") != std::string::npos)
            k--;
    }
    if (k != 0)
        error_msg("Error : check The number of brackets");
    int m = 0;
    for (it = res.begin(); it != res.end(); ++it)
    {
        if ((res[it->first].find("server") != std::string::npos && m == 1) || (res[it->first].find("server") != std::string::npos && res[it->first - 1].find("}") != std::string::npos))
        {
            std::map<int, std::string>::iterator it1 = it;
            it1++;
            while (it1 != res.end())
            {
                if (res[it1->first].find("server") != std::string::npos && res[it1->first + 1].find("{") != std::string::npos)
                    error_msg("Error!! not 2 server inside another one");
                else
                    it1++;
            }
            m++;
        }
        else if (res[it->first].find("location") != std::string::npos)
        {
            std::map<int, std::string>::iterator it1 = it;
            it1++;

            while (it1 != res.end())
            {
                if (k != 0 && (res[it->first].find("server") != std::string::npos || res[it->first].find("location") != std::string::npos))
                    error_msg("Error!!");
                else
                    it1++;
            }
        }
    }
    return (res);
}

/******************************************************************************/
ErrorHandling::~ErrorHandling()
{
}