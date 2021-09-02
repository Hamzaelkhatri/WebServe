#include "../includes/cgi.hpp"

std::string cgi::CGI(char *argv[], char *envp[])
{
    int link[2];
    pid_t pid;
    char foo[4096];
    std::string str = "";

    setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
    setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
    setenv("REDIRECT_STATUS", "1", 1);
    setenv("PATH_INFO", "/home/hamza/Desktop/WebServe/webpage/cgi/home.php", 1);
    setenv("SERVER_PORT", "8011", 1);
    setenv("REQUEST_METHOD", "GET", 1);
    setenv("REMOTE_ADDR", "127.0.0.1", 1);
    setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
    setenv("SERVER_NAME", "localhost", 1);
    setenv("SCRIPT_FILENAME", "/home/hamza/Desktop/WebServe/webpage/cgi/home.php", 1);
    setenv("SCRIPT_NAME", "home.php", 1);
    setenv("REDIRECT_STATUS", "200", 1);

    if (pipe(link) == -1)
        die("pipe");

    if ((pid = fork()) == -1)
        die("fork");

    if (pid == 0)
    {

        dup2(link[1], STDOUT_FILENO);
        close(link[0]);
        close(link[1]);
        chdir("/home/hamza/Desktop/WebServe/webpage/cgi/");
        execve("/usr/bin/php-cgi", argv, envp);
        perror("execve");
        exit(0);
    }
    else
    {

        close(link[1]);
        int nbytes;
        while ((nbytes = read(link[0], foo, sizeof(foo))))
                str.append(foo, nbytes);
        wait(NULL);
    }
    return (str);
}
