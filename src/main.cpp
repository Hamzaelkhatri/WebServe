/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 16:39:32 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/17 19:39:50 by zdnaya           ###   ########.fr       */
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
            Parsing *pars = new Parsing(av[1]);
            std::map< int , std::multimap<std::string, std::string> >::iterator it;
            std::multimap<std::string, std::string>::iterator  it2;
            std::map< int , std::multimap<std::string, std::string> > tmp = pars->GetServerMap();
            for(it= tmp.begin(); it != tmp.end(); ++it)
            {
                std::cout <<  YEL << "\tLocation" << reset<< std::endl; 
                std::cout <<  RED << "\t\tServer\t\t" << it->first << ":" << reset << std::endl;
                for(it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                {
                std::cout << "\t" << it2->first << "\t\t\t" << it2->second << "\n\n";
                }
                std::cout << YEL  << "\t" << reset<< std::endl;

            }
            std::multimap<std::string, std::string> it1;


            // Server  serv(pars , envp);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
      
    }
    else
        std::cerr << "wrong file \n";
    return (0);
}

