/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/02 16:39:32 by zainabdnaya       #+#    #+#             */
/*   Updated: 2021/09/19 13:44:30 by zdnaya           ###   ########.fr       */
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
