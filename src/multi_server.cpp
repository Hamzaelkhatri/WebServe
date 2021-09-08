/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 18:49:10 by zdnaya            #+#    #+#             */
/*   Updated: 2021/09/08 16:10:07 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"


int calcul_liten(std::map< int , std::multimap<std::string, std::string> > tmp)
{
    int i = 0;
    std::map<int, std::multimap<std::string, std::string>  >::iterator it0;
    std::multimap<std::string, std::string>::iterator it;
    for(it0 = tmp.begin(); it0 != tmp.end(); ++it0)
    {
        for(it = it0->second.begin(); it != it0->second.end(); ++it)
        {
            if(it->first == "listen")
            {
                // listen[i] = std::stoi(it->second);
                // std::cout<< " listen to port " << it->second<< std::endl;
                i++;
            }
        }
    }
    return (i);
}
void Server::multi_server(Parsing *p,char *envp[])
{
    int h = calcul_liten(p->GetServerMap());
    // std::cout << "h ==> " << h << std::endl;
    int i = 0;
    address = new struct sockaddr_in[h + 1];
    /***** creatSocket_fd *********************************/
    server_fds = new int[h + 1];
    for( i = 0; i <  h ; i++)
    {
        server_fds[i] = socket(AF_INET, SOCK_STREAM, 0);
        // std::cout <<  i <<  " ==> The |" << server_fds[i] << "| is creted" << std::endl;
        if (server_fds[i] == 0)
        {
            std::cerr << "socket failed" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    std::map< int , std::multimap<std::string, std::string> > tmp = p->GetServerMap() ;
    for( i = 0; i <  h; i++)
    {
        int buffsize = 1;
        if (setsockopt(server_fds[i], SOL_SOCKET, SO_REUSEADDR, &buffsize, sizeof(buffsize)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        std::memset((char *)&this->address[i], 0, sizeof(&this->address));
        std::cout <<  i <<  " ==> The |" << server_fds[i] << "| is creted" << std::endl;
    }
    /*************************************************************************************************/
    std::map<int, std::multimap<std::string, std::string>  >::iterator it0 = tmp.begin();
    std::multimap<std::string, std::string>::iterator it;
    for(it0 = tmp.begin(); it0 != tmp.end(); ++it0)
    {
        for (it = it0->second.begin(); it != it0->second.end() ; it++ )
        {
            if(it->first == "listen" )
            {
                // std::cout << "\t\t\t Listening " << it->second << std::endl;
                address[i].sin_port = htons(std::stoi(it->second));
            }
            if (it->second == "server_addr" )
                address[i].sin_addr.s_addr = inet_addr(mtmp.find("server_addr")->second.c_str());
            else
                address[i].sin_addr.s_addr = INADDR_ANY;
            address[i].sin_family = AF_INET;
            memset(address[i].sin_zero, '\0', sizeof address[i].sin_zero); // why help to pad from sockaddr_in to sockaddr
            char *s = inet_ntoa(address[i].sin_addr);
            std::cout << "host  " << address[i].sin_port << "\t:\t" <<  s << std::endl;
        }
     }
    
}