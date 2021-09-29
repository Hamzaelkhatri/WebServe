/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zainabdnayagmail.com <zainabdnayagmail.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/03 12:22:53 by zdnaya            #+#    #+#             */
/*   Updated: 2021/09/28 19:49:10 by zainabdnaya      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cgi.hpp"

std::string cgi::CGI(Response *r, char *envp[])
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
    for (int i = 0; i < params.size(); i++)
    {
        if(params[i] != '&')
            res+=params[i];
        else 
        {
            parm.push_back("\nSet-Cookie: "+res);
            res = "";
        }
        
    }
    parm.push_back("\nSet-Cookie: "+res);
    std::string tmp = "";
    for (int i = 0; i < parm.size(); i++)
        tmp +=parm[i];
    r->set_params(tmp);
    std::string::size_type pos = 0;
    const char **av = new const char *[3];
    av[0] = strdup(r->getCGIPath().c_str());
    extern char **environ;
    av[1] = fullpath.c_str();
    av[2] = NULL;
    if (pipe(fd) == -1)
        std::cout << "pipe" << std::endl;
    if ((pid = fork()) == -1)
        std::cout << "fork" << std::endl;
    //child --> parent
    if (pid == 0) //child
    {
        dup2(fd[1], STDOUT_FILENO); // 1
        close(fd[0]);
        chdir(path.c_str());
        if (execve(av[0], (char **)av, environ) < 0)
            perror("exeve");
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

// const char *cgi::cgiFailed::what() const throw()
// {
//     return "CGI failed";
// }