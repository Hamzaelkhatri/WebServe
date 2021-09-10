/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_tools.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/03 21:46:25 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/10 18:02:00 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

char * Server::removeHTTPHeader(char *buffer, int &bodySize)
 {
    char *t = strstr(buffer, "\r\n\r\n");
    t = t + 4;

    for (char* it = buffer; it != t; ++it) 
        ++bodySize;

    return t;
}


int Server::check_index(std::string str)
{
    int ret = open(str.c_str(), O_RDONLY);
    if (ret == -1)
        return (0);
    close(ret);
    return (1);
}

int Server::check_dir(std::string dir, std::string str)
{
    DIR *dirp;
    struct dirent *dp;
    dirp = opendir(dir.c_str());
    if (dirp == NULL)
        return (0);
    int i = 0;
    while ((dp = readdir(dirp)) != NULL)
    {
        if (dp->d_name[0] == '.')
            continue;
        if (dp->d_type == DT_DIR)
        {
            i++;
            std::string tmp = dir + "" + dp->d_name;
            // std::cout << tmp << "  " << str << "\n";
            if (tmp.find(str) != std::string::npos)
            {
                if (check_index(tmp + "/index.html"))
                    return (1);
                else
                    return (0);
            }
        }
        else if (dp->d_type == DT_REG && str.find(".") != std::string::npos)
        {
            std::string tmp = dir + "" + dp->d_name;
            // std::cout << tmp << " || " << str << "\n";
            if (tmp.find(str) != std::string::npos)
                return (2);
        }
    }
    closedir(dirp);
    if (str.find(".") == std::string::npos)
    {
        dirp = opendir(str.c_str());
        if (dirp == NULL)
            return (0);
        closedir(dirp);
    }
    else
    {
        int ret = open(str.c_str(), O_RDONLY);
        if (ret == -1)
            return (0);
        close(ret);
    }
    return (2);
}

std::string Server::getBody(std::string path)
{
    std::string body;
    std::ifstream file(path);
    if (file.is_open())
    {
        std::string line;
        while (getline(file, line))
        {
            body += line;
            body += "\n";
        }
        file.close();
    }
    return body;
}

void Server::SaveFile(std::string path, std::string body,int size)
{
    std::ofstream file(path, std::ios::out | std::ios::binary);
    if (file.is_open())
    {
        file.write(body.c_str(), size);
        file.close();
    }
}

