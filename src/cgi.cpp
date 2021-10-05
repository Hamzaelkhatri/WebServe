/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zdnaya <zdnaya@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/03 12:22:53 by zdnaya            #+#    #+#             */
/*   Updated: 2021/10/04 13:33:14 by zdnaya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cgi.hpp"

std::string cgi::CGI(Response *r)
{
    int fd[2];
    pid_t pid;
    char foo[4096];
    std::string str = "";

    // std::string page = av[1];
    std::string fullpath = r->getPath();
    std::string filename = fullpath.substr(fullpath.find_last_of("/") + 1);
    std::string path = fullpath.substr(0, fullpath.find_last_of("/") + 1);
    // setenv("CONTENT_LENGTH", r->getContentLength().c_str(), 1);
    setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);             //version Of Gate away interface
    setenv("SERVER_PROTOCOL", r->getVersion().c_str(), 1); //version of http , hyper text protocole
    setenv("SCRIPT_FILENAME", fullpath.c_str(), 1);
    setenv("SERVER_PORT", r->getPort().c_str(), 1);       // PORT OF SERVER
    setenv("REQUEST_METHOD", r->getMethod().c_str(), 1);  // METHODE HTTP
    setenv("REMOTE_ADDR", r->getHost().c_str(), 1);       //
    setenv("SERVER_NAME", r->getServerName().c_str(), 1); //
    setenv("SCRIPT_NAME", filename.c_str(), 1);           //name of file
    setenv("REDIRECT_STATUS", r->getStatus().c_str(), 1); //status of cnx
    setenv("QUERY_STRING", r->get_params().c_str(), 1);
    std::string params = r->get_params();
    params = params.substr(params.find("?") + 1, params.length());

    std::string res = "";
    std::vector<std::string> parm;
    std::string tmp = "";
    int j = 0;
    for (int i = 0; i < (int)params.size(); i++)
    {
        if (params[i] != '&')
            res += params[i];
        else
        {
            parm.push_back("\nSet-Cookie: " + res + "\n");
            tmp += res + ";";
            res = "";
            j = 1;
        }
    }
    parm.push_back("Set-Cookie: " + res + ";\n");
    tmp += res + ";\n";
    if (!r->getSetCookie().empty())
        setenv("HTTP_COOKIE", r->getSetCookie().c_str(), 1);
    tmp = "";
    for (int i = 0; i < (int)parm.size(); i++)
    {
        tmp += parm[i];
    }
    if (r->getSetCookie().empty())
        setenv("HTTP_COOKIE", tmp.c_str(), 1);
    r->set_params(tmp);
    const char **av = new const char *[3];
    av[0] = strdup(r->getCGIPath().c_str());
    extern char **environ;
    av[1] = fullpath.c_str();
    av[2] = NULL;
    if (pipe(fd) == -1)
        std::cout << "pipe" << std::endl;
    if ((pid = fork()) == -1)
        std::cout << "fork" << std::endl;
    if (pid == 0) //child
    {
        dup2(fd[1], STDOUT_FILENO); // 1
        close(fd[0]);
        chdir(path.c_str());
        if (execve(av[0], (char **)av, environ) < 0)
        {
            std::cout << "CGI_PATH NOT FOUND" << std::endl;
            return ("NO");
        }
    }
    else
    {
        close(fd[1]);
        int nbytes;
        while ((nbytes = read(fd[0], foo, sizeof(foo))))
            str.append(foo, nbytes);
        wait(NULL);
    }
    if (av[0])
        delete av[0];
    delete[] av;
    return (str);
}