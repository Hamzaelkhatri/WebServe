/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 16:39:32 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/07 18:47:37 by zdnaya           ###   ########.fr       */
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
        ErrorHandling error(av[1]);
        Parsing *pars = new Parsing(av[1]);
        // std::map< int , std::multimap<std::string, std::string> > tmp = pars->GetServerMap() ;
        // std::map<int, std::multimap<std::string, std::string>  >::iterator it0;
        // std::multimap<std::string, std::string>::iterator it;
        // for(it0 = tmp.begin(); it0 != tmp.end(); ++it0)
        // {
        //     std::cout << "-----------------Server-----------------\n";
        //     std::cout << "for server " << it0->first << std::endl;
        //     for(it = it0->second.begin(); it != it0->second.end(); ++it)
        //     {
        //         std::cout << it->first << " ==> " << it->second << std::endl;
        //     }
        // }
        //     std::cout << "----------------------------------\n";
        // std::multimap< int , std::multimap<std::string, std::string> > tmp1 = pars->Getloc_map() ;
        // std::multimap<int, std::multimap<std::string, std::string>  >::iterator it2;
        // std::multimap<std::string, std::string>::iterator it1 ;
        // for(it2 = tmp1.begin(); it2 != tmp1.end(); ++it2)
        // {
        // std::cout << "------------------- Location ---------------\n";
        //     std::cout << "for location " << it2->first << std::endl;
        //     for(it1 = it2->second.begin(); it1 != it2->second.end(); ++it1)
        //     {
        //         std::cout << it1->first << " ==> " << it1->second << std::endl;
        //     }
        // }
        //    std::cout << "here\n";
    
        Server server(pars,envp);
    }
    else
        std::cerr << "wrong file \n";
    return (0);
}

