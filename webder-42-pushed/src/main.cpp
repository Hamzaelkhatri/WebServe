/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 16:39:32 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/10/04 13:42:18 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Webserv.hpp"

void check_server_keys(std::string key)
{
    if (key != "listen"  && key != "server_name" && key != "server_addr" && key != "root" && key != "error_page" && key != "client_body_size")
        throw std::runtime_error("Key " + key + " is not valid In server ");
}

void check_location_keys(std::string key)
{
    if (key != "index" && key != "location" && key != "http_methods" && key != "upload_path" && key != "root" && key != "autoindex" && key != "return" && key != "cgi_path" && key != "upload_status")
        throw std::runtime_error("Key " + key + " is not valid In location");
}

int check_if_executable_file(std::string path)
{
    struct stat s;
    if (stat(path.c_str(), &s) == 0)
    {
        if (s.st_mode & S_IXUSR)
            return (1);
    }
    return (0);
}

int main(int ac, char *av[], char *envp[])
{
    if (ac != 2)
    {
        std::cerr << "Add conf file\n";
        exit(0);
    }
    if (compare_end(av[1], ".conf") == true)
    {
        try
        {
            ErrorHandling error(av[1]);
            Parsing *pars = new Parsing(av[1]);
            std::map<int, std::multimap<std::string, std::string> >::iterator it;
            std::multimap<std::string, std::string>::iterator it2;
            std::map<int, std::multimap<std::string, std::string> > tmp = pars->GetServerMap();
            for (it = tmp.begin(); it != tmp.end(); ++it)
            {
                for (it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                {
                    check_server_keys( it2->first);
                }
            }
            std::multimap<int, std::multimap<std::string, std::string> > tmp1 = pars->Getloc_map();
            std::multimap<int, std::multimap<std::string, std::string> >::iterator it3;
            std::multimap<std::string, std::string>::iterator it1;
            for (it3 = tmp1.begin(); it3 != tmp1.end(); ++it3)
            {
                for (it1 = it3->second.begin(); it1 != it3->second.end(); ++it1)
                {
                    // std::cout << it1->second << std::endl;
                    if("cgi_path" == it1->first.substr(it1->first.find(" ")+1))
                    {
                        if(check_if_executable_file(it1->second))
                            continue;
                        else
                            throw std::runtime_error("Cgi_path is not executable");
                    }
                    check_location_keys(it1->first.substr(it1->first.find(" ")+1));
                }
            }
            Server serv(pars, envp);
            delete pars;
        }

        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    else
        std::cerr << "wrong file \n";
    return (0);
}
