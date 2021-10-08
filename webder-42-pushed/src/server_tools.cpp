/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_tools.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zainabdnayagmail.com <zainabdnayagmail.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/03 21:46:25 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/28 16:29:01 by zainabdnaya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

int Server::_Accept_client(int sock)
{
    int addrlen = sizeof(client);
    csock = accept(sock, (struct sockaddr *)&client, (socklen_t *)&addrlen);
    if (csock != -1)
    {
        fcntl(csock, F_SETFL, O_NONBLOCK);
        FD_SET(csock, &masterfds);
        FD_SET(csock, &writefds);
        if (csock > maxfd)
            maxfd = csock;
        _clients.insert(std::pair<int, std::string>(csock, ""));
        std::string ip = inet_ntoa(client.sin_addr);
        int port = ntohs(client.sin_port);
        std::cout << CYN << "[+]"
                  << reset << "New connection from : " << ip << ":" << port << std::endl;
    }
    else
        throw(AcceptFailed());
    return (csock);
}

int Server::_Get_request(int sock)
{
    char buf[BUFFER_SIZE + 1];
    int n;
    int i = 0;
    bzero(buf, BUFFER_SIZE + 1);
    if ((n = recv(sock, buf, BUFFER_SIZE, 0)) > 0)
    {
        if (i == 0)
        {
            std::strcpy(this->request, buf);
            i++;
        }
        else
            std::strcat(this->request, buf);
    }
    return (n);
}

int Server::check_header(std::string header)
{
    int i = 0;
    //check if header is valid
    if ((header.find("HTTP/1.1") == std::string::npos))
        return (0);
    else if ((header.find("GET") == std::string::npos) && (header.find("POST") == std::string::npos) && (header.find("DELETE") == std::string::npos))
        return (0);
    std::string line;
    std::stringstream ss(header);
    while (std::getline(ss, line))
    {
        if (line.find("Host:") != std::string::npos)
            i++;
    }
    if (i == 0)
        return (0);
    return (1);
}

bool Server::checkRequest(std::string &req)
{
    if ((req.find("\r\n\r\n") != std::string::npos))
    {
        std::string headers = req.substr(0, req.find("\r\n\r\n") + 4);
        if (headers.find("Transfer-Encoding: chunked") != std::string::npos)
        {
            chunked = true;
            if (req.find("\r\n\r\n") == std::string::npos)
                return false;
        }
        else if (headers.find("Content-Length") != std::string::npos)
        {
            size_t length = std::atoi(headers.substr(headers.find("Content-Length: ")).c_str() + 16);
            std::string body = req.substr(req.find("\r\n\r\n") + 4);
            if (body.length() < length)
                return false;
        }
        return true;
    }
    return false;
}

int get_size_of_chunked(std::string str)
{
    int i = 0;
    std::stringstream sstream(str);
    sstream >> std::hex >> i;
    return (i);
}

void Server::unchunkRequest(std::string &req, Response *res)
{
    int new_size = 0;
    int i = 0;
    std::cout << its->second << std::endl;
    std::string output = req.substr(0, req.find("\r\n\r\n") + 4);
    req = req.substr(req.find("\r\n\r\n") + 4);
    std::string line;
    int size = 0;
    std::stringstream bodyStream(req);
    while (line != "\r\n")
    {
        i = 0;
        std::getline(bodyStream, line);
        new_size = get_size_of_chunked(line);
        std::getline(bodyStream, line);
        if (i == 0)
            output += line;
        if (new_size == 0)
            break;
        size += new_size;
    }
    res->setContentLength(std::to_string(size));
    its->second = output;
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

// std::string Server::getBody(std::string path)
// {
//     std::string body;
//     std::ifstream file(path);
//     if (file.is_open())
//     {
//         std::string line;
//         while (getline(file, line))
//         {
//             body += line;
//             body += "\n";
//         }
//         file.close();
//     }
//     return body;
// }

// void Server::SaveFile(std::string path, std::string body,int size)
// {
//     std::ofstream file(path, std::ios::out | std::ios::binary);
//     if (file.is_open())
//     {
//         file.write(body.c_str(), size);
//         file.close();
//     }
// }
