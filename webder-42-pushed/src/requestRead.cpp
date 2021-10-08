/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requestRead.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/10 18:05:36 by zdnaya            #+#    #+#             */
/*   Updated: 2021/10/04 17:35:42 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Webserv.hpp"

int ContaineOnly(std::string str)
{
    std::regex e("[\\n\\r\\n\\r]+");
    if (std::regex_match(str, e))
        return 1;
    return 0;
}

int indexOfNewLine(std::string str)
{
    int i = 0;
    while (str[i])
    {
        if (str[i] == '\n')
            return (i);
        i++;
    }
    return (i - 1);
}

int isAlpha(std::string str)
{
    std::regex e("[a-zA-Z]+"); //iudeheideigefytuvngjcigaicgwiagaiohdisgsx
    if (std::regex_match(str, e))
        return 1;
    return 0;
}

int isCapital(std::string str)
{
    std::regex e("[A-Z]+");
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
        if (str[i] == ' ')
            count++;
        i++;
    }
    return (count + 1);
}

int getNumberOfline(std::string str)
{
    int i = 0;
    int count = 0;
    while (str[i])
    {
        if (str[i] == '\n')
            count++;
        i++;
    }
    return (count + 1);
}