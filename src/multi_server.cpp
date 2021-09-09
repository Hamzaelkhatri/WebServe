/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/07 18:49:10 by zdnaya            #+#    #+#             */
/*   Updated: 2021/09/09 17:15:16 by zdnaya           ###   ########.fr       */
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
        // std::cout <<  i <<  " ==> The |" << server_fds[i] << "| is creted" << std::endl;
    }
    /*************************************************************************************************/
    std::map<int, std::multimap<std::string, std::string>  >::iterator it0 = tmp.begin();
    std::multimap<std::string, std::string>::iterator it;
    std::map<int, std::string>mini;
    int j =0;
    int k = 0;
    for(it0 = tmp.begin(); it0 != tmp.end(); ++it0)
    {
        j++;
        k = 0;
        for (it = it0->second.begin(); it != it0->second.end() ; it++ )
        {
            if (it->first== "server_addr")
                {
                    k++;
                    mini[j] = it->second;
                }
            // address[i].sin_family = AF_INET;
            // memset(address[i].sin_zero, '\0', sizeof address[i].sin_zero); // why help to pad from sockaddr_in to sockaddr
            // char *s = inet_ntoa(address[i].sin_addr);
            // std::cout << "host  " << address[i].sin_port << "\t:\t" <<  s << std::endl;
        }
        if( k == 0 )
        {
            mini[j] = "INADDR_ANY";
        }
     }
    // for(std::map<int, std::string>::iterator o = mini.begin(); o != mini.end() ; o++)
    //     std::cout << o->first << "\t\t" <<  o->second << std::endl;
        i = 0;
        std::map<int, std::string>::iterator o = mini.begin();
        it0 = tmp.begin();
    while( it0 != tmp.end() && o != mini.end())
    {
    
            for (it = it0->second.begin(); it != it0->second.end() ; it++ )
            {
                if( it->first.find("listen") != std::string::npos )
                {
                    // std::cout << i << std::endl;
                    std::string str = it->second;
                    // std::cout << str << std::endl;
                    while( i < h && str != "NULL")
                    {
                            address[i].sin_port =  htons(std::stoi(str));
                            address[i].sin_family = AF_INET;
                            if ( o->second == "INADDR_ANY" )
                                address[i].sin_addr.s_addr = INADDR_ANY;
                            else
                                address[i].sin_addr.s_addr = inet_addr(o->second.c_str());
                            memset(address[i].sin_zero, '\0', sizeof address[i].sin_zero); // why help to pad from sockaddr_in to sockaddr
                           
                            str = "NULL";
                            i++;
                    }
                }
            }
        it0++;
        o++;
     }
     i = 0;

     /**********************Bind**************************************/
    while(i < h)
    {
        char *s = inet_ntoa(address[i].sin_addr);
        if (bind(server_fds[i], (struct sockaddr *)(&address[i]), sizeof(address[i])) < 0)
            perror("Bind");
        if (listen(server_fds[i], 100) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        i++;
    }
        i = 0;
        for (i = 0; i < 10; i++) 
        {
            client_fds[i] = 0;
        }
    int sd = 0;
    int nfd[10];
    while(1)
    {
        i = 0;
        FD_ZERO(&readfds);
        std::cout << "\t\t\t ---waiting for connection--- " << std::endl;
        while(i < h )
        {
            FD_SET(server_fds[i], &readfds);
            nfds = server_fds[i];
            i++;
        }
        for ( i = 0 ; i < 10 ; i++) 
        {
			sd = client_fds[i];
			if(sd > 0)
			    FD_SET( sd , &readfds);
            if(sd > nfds)
				nfds = sd;
        }
        if (select (nfds+1 , &readfds, NULL, NULL, NULL) < 0)
        {
          perror ("select");
          exit (EXIT_FAILURE);
        }
        struct sockaddr_in tmp;
        i = 0;
        while(i < h )
        {
            if (FD_ISSET(server_fds[i], &readfds))
            {
                int addrlen = sizeof(address[i]);
                new_socket = accept(server_fds[i], (struct sockaddr *)&address[i], (socklen_t*)&addrlen);
                if ( new_socket < 0)
                {
                    perror("accept");
                    exit(EXIT_FAILURE);
                }
                someString = bufferStor();
                std::cout << someString;
                break;
            }
            FD_CLR(server_fds[i],&readfds);
            i++;
        }
    close(new_socket);
    }
    i = 0;
    while( i < h)
            close(server_fds[++i]);
}
