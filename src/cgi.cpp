/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/03 12:22:53 by zdnaya            #+#    #+#             */
/*   Updated: 2021/09/16 12:15:49 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cgi.hpp"

std::string cgi::CGI(char *av[], char *envp[])
{
    int fd[2];
    pid_t pid;
    char foo[4096];
    std::string str = "";

    std::string page = av[1];
    std::string fullpath = "/home/hamza/Desktop/WebServe/webpage" + page;
    setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);//version Of Gate away interface
    setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);//version of http , hyper text protocole
    setenv("PATH_INFO", "/home/hamza/Desktop/WebServe/webpage/home.php", 1);
    setenv("SERVER_PORT", "8011", 1);// PORT OF SERVER
    setenv("REQUEST_METHOD", "GET", 1);// METHODE HTTP
    setenv("REMOTE_ADDR", "127.0.0.1", 1);//
    setenv("SERVER_NAME", "localhost", 1);//
    // setenv("SCRIPT_FILENAME", "/home/hamza/Desktop/WebServe/webpage/home.php", 1);
    setenv("SCRIPT_NAME", "home.php", 1);//name of file
    setenv("REDIRECT_STATUS", "200", 1);//status of cnx
    if (pipe(fd) == -1) // cat hel.txt | cat -e  FD[1] ---> FD[0] 0 1 2 3
        die("pipe");
    if ((pid = fork()) == -1)
        die("fork");
    //child --> parent 
    if (pid == 0)//child
    {
        dup2(fd[1], STDOUT_FILENO);// 1
        close(fd[0]);
        chdir("/home/hamza/Desktop/WebServe/webpage/cgi");
        execve(av[0], av, envp);
        throw std::runtime_error("execve");
    }
    else
    {
        close(fd[1]);
        int nbytes;
        while ((nbytes = read(fd[0], foo, sizeof(foo))))
                str.append(foo, nbytes);
        wait(NULL);
    }
    return (str);
}

// const char *cgi::cgiFailed::what() const throw()
// {
//     return "CGI failed";
// }