/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 17:24:36 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/18 11:02:57 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/tools.hpp"



bool	line_equal(std::string const &line, std::string const &str)
{
	unsigned int	start ;
	unsigned int	end ;
    
    end = str.size();
    start = 0;
	if (line.size() >= start && line.substr(start, end) == str)
		return true;
	return false;
}

bool compare_end(std::string const &s1, std::string const &s2)
{
    int i;
    int j;

    i = s1.size() - 1;
    j = s2.size() - 1;
    while (i >= 0 && j >= 0)
    {
        if (s1[i] != s2[j])
            return false;
        i--;
        j--;
    }
    return true;
}

int nbr_lines(std::string source)
{
    int i;
    int lines;

    i = 0;
    lines = 1;
    while (source[i])
    {
        if (source[i] == '\n')
            lines++;
        i++;
    }
    return (lines);
}

bool is_whitespace(const std::string &s)
{
    for (std::string::const_iterator it = s.begin(); it != s.end(); ++it)
    {
        if (!isspace(*it) || *it != '\n')
        {
            return false;
        }
    }
    return true;
}



std::string ltrim(const std::string &s)
{
    const std::string WHITESPACE1 = " \n\r\t\f\v";

    size_t start = s.find_first_not_of(WHITESPACE1);
    return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string &s)
{
    const std::string WHITESPACE = " \n\r\t\f\v;";
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}
 
