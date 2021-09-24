/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestRead.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zainabdnayagmail.com <zainabdnayagmail.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 18:05:36 by zdnaya            #+#    #+#             */
/*   Updated: 2021/09/24 14:43:32 by zainabdnaya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Webserv.hpp"

int ContaineOnly(std::string str)
{
    std::regex e ("[\\n\\r\\n\\r]+");
    if (std::regex_match(str, e))
        return 1;
    return 0;
}

int indexOfNewLine(std::string str)
{
    int i = 0;
    while (str[i])
    {
        if(str[i]=='\n')
            return (i);
        i++;
    }
    return (i-1);
}

int isAlpha(std::string str)
{
    std::regex e ("[a-zA-Z]+");//iudeheideigefytuvngjcigaicgwiagaiohdisgsx
    if (std::regex_match(str, e))
        return 1;
    return 0;
}

int isCapital(std::string str)
{
    std::regex e ("[A-Z]+");
    if (std::regex_match(str, e))
        return 1;
    return 0;
}

int numberOfWords(std::string str)
{
    int i = 0;
    int count = 0;
    while (str[i])
    {
        if(str[i]==' ')
            count++;
        i++;
    }
    return (count+1);
}

int getNumberOfline(std::string str)
{
    int i = 0;
    int count = 0;
    while (str[i])
    {
        if(str[i]=='\n')
            count++;
        i++;
    }
    return (count+1);
}

std::string Server::bufferStor()
{
    char buffer[1000];
    int nDataLength = 2;
    int loop = 0;
    int n = 0;
    int first_line = 0;
    error = 0;
    
    someString = "";
    int sizeOfFile=-1;
    while ((nDataLength = recv(sd, (void *)&buffer, 999, 0)) > 0)
    {
        if(someString.find("\r\n\r\n") != std::string::npos && sizeOfFile < loop)
            break;
        bzero(buffer, sizeof(buffer));
        nDataLength = recv(sd, (void *)&buffer, 999, 0);
        loop+=nDataLength;
        if (nDataLength < 0 || nDataLength == 0)
            break;
        if(nDataLength > 2 || n != 0)
        {
            someString += buffer;
            if(!first_line)
            {
                // std::cout << someString << std::endl;
                std::string str = someString.substr(0,someString.find_first_of("\r\n"));
                if(!isCapital(str.substr(0,str.find_first_of(" "))) || someString.find("HTTP/1.1") == std::string::npos || numberOfWords(str) != 3)
                {
                    error = 1;
                    someString ="<html><head><title>400 Bad Request</title></head><body><h1>Bad Request</h1><p>Bad Request</p></body></html>";
                    len = someString.size();
                    return(someString);
                }
                first_line = 1;
            }
            if(someString.find("Content-Length:") != std::string::npos)
            {
                int index = someString.find("Content-Length:")+15;
                sizeOfFile =std::stoi(someString.substr(someString.find("Content-Length:")+15,indexOfNewLine(&someString[index]-1)));
            }
            n++;
        }
        std::cout << buffer << std::endl;
        // std::cout << buffer << std::endl << nDataLength << "  "<< sizeOfFile << std::endl;
    }
    len = someString.size();
    if(sizeOfFile != -1)
    {
        body = someString.substr(someString.find("\r\n\r\n")+4,someString.size());
        // if(body.size() != sizeOfFile)
        {
            body = body.substr(body.find("\r\n\r\n")+4,body.size());
            body = body.substr(0,body.find("\r\n"));
            SaveFile("/home/hamza/Desktop/WebServe/webpage/upload/output.txt", body,body.size());
        }
        // someString = someString.substr(0,someString.find("\r\n\r\n")+4);
    }
    return(someString);

}