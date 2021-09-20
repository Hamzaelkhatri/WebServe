/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 16:39:32 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/18 20:06:53 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Webserv.hpp"

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
            // error.get_map_s();
            Parsing *pars = new Parsing(av[1]);
            // std::map<int, std::multimap<std::string, std::string> >::iterator it;
            // std::multimap<std::string, std::string>::iterator it2;
            // std::map<int, std::multimap<std::string, std::string> > tmp = pars->GetServerMap();
            // for (it = tmp.begin(); it != tmp.end(); ++it)
            // {
            //     std::cout << YEL << "Server" << reset << std::endl;
            //     std::cout << RED << "\t\tServer\t\t" << it->first << ":" << reset << std::endl;
            //     for (it2 = it->second.begin(); it2 != it->second.end(); ++it2)
            //     {
            //         std::cout << "\t" << it2->first << "\t\t\t" << it2->second << "\n\n";
            //     }
            // }
            // std::cout << YEL << "\n\n";

            // std::multimap<int, std::multimap<std::string, std::string> > tmp1 = pars->Getloc_map();
            // std::multimap<int, std::multimap<std::string, std::string> >::iterator it3;
            // std::multimap<std::string, std::string>::iterator it1;
            // for (it3 = tmp1.begin(); it3 != tmp1.end(); ++it3)
            // {
            //     std::cout << YEL << "Location" << reset << std::endl;
            //     std::cout << RED << "\t\tLocation\t\t" << it3->first << ":" << reset << std::endl;
            //     for (it1 = it3->second.begin(); it1 != it3->second.end(); ++it1)
            //     {
            //         std::cout << "\t" << it1->first << "\t\t\t" << it1->second << "\n\n";
            //     }
            // }
            // std::cout << YEL << "" << reset << std::endl;
            Server serv(pars, envp);
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
