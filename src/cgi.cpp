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
    //exemple content type string
    setenv("CONTENT_TYPE", r->getContentType().c_str(), 1);
    std::cout << "CONTENT_LENGTH " << r->getContentLength() << std::endl;
    setenv("HTTP_ACCEPT", "image/gif,image/x-xbitmap,image/jpeg,image/pjeg,image/pgn,*/*", 1);
    setenv("HTTP_COOKIE", "PHPSESSID=1; expires=Mon, 01-Jan-2022 00:00:00 GMT; path=/", 1);
    setenv("HTTP_USER_AGENT", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/83.0.4103.116 Safari/537.36", 1);
    setenv("HTTP_ACCEPT_LANGUAGE", "en-US,en;q=0.9", 1);
    setenv("HTTP_ACCEPT_ENCODING", "gzip, deflate, br", 1);
    setenv("HTTP_HOST", r->getHost().c_str(), 1);
    setenv("HTTP_CONNECTION", "keep-alive", 1);
    setenv("HTTP_CACHE_CONTROL", "max-age=0", 1);
    setenv("HTTP_UPGRADE_INSECURE_REQUESTS", "1", 1);
    setenv("HTTP_ORIGIN", "http://localhost:8050", 1);
    setenv("HTTP_REFERER", "http://localhost:8050", 1);
    setenv("HTTP_ACCEPT_RANGES", "bytes", 1);
    setenv("REDIRECT_STATUS", "1", 1);
    setenv("HTTP_X_REQUESTED_WITH", "XMLHttpRequest", 1);

    std::string params = r->get_params();
    std::string::size_type pos = 0;
    while ((pos = params.find("&", pos)) != std::string::npos)
    {
        params.replace(pos, 1, ";");
        pos += 1;
    }
    //add real exemple of cookies with date

    const char **av = new const char *[3];
    av[0] = strdup(r->getCGIPath().c_str());
    std::cout << r->getCGIPath().c_str() << std::endl;
    extern char **environ;
    av[1] = fullpath.c_str();
    av[2] = NULL;
    if (pipe(fd) == -1) // cat hel.txt | cat -e  FD[1] ---> FD[0] 0 1 2 3
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