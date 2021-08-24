/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zainabdnayagmail.com <zainabdnayagmail.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 16:39:32 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/08/24 09:09:13 by zainabdnaya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Webserv.hpp"

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cerr << "Add conf file\n";
        exit(0);
    }
    if (compare_end(av[1], ".conf") == true)
    {
        ErrorHandling error(av[1]);
        Parsing(av[1], error.get_nbr_server(), error.get_nbr_location());
    }
    else
        std::cerr << "wrong file \n";
    return (0);
}

/*----------------------------------------------------------------
std::map<std::string, std::string> map_simpl(std::ifstream &my_file)
{
    std::map <std::string,std::string> map_s;
    std::string key;
    std::string str1;
    std::string str;
    while (my_file >> str)
    {
        while ((str == "{" || str == "server"))
            my_file >> str;
        if (str == "}" && my_file.eof())
            my_file >> str;
        else if (str == "}" && !my_file.eof())
            break;
        else if (str == "http_methods")
        {
            key = str;
            my_file >> str;
            str1 = "";
            while (str == "GET" || str == "HEAD" || str == "POST" || str == "PUT" || str == "OPTIONS" || str == "DELETE")
            {
                str1 += str + " ";
                my_file >> str;
            }
            map_s[key] = str1;
            if (str == "error_page")
            {
                key = str;
                my_file >> str;
                str1 = str;
                my_file >> str;
                map_s[key] = str1 + " " + str;
            }
            else
            {
                key = str;
                my_file >> str;
                map_s[key] = str;
            }
        }
        else if (str == "error_page")
        {
            key = str;
            my_file >> str;
            str1 = str;
            my_file >> str;
            map_s[key] = str1 + " " + str;
        }
        else
        {
            key = str;
            my_file >> str;
            map_s[key] = str;
        }
    }
    return (map_s);
}
*/
