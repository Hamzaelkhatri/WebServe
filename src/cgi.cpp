#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);

int main(int argc, char *argv[], char *envp[])
{
  int link[2];
  pid_t pid;
  char foo[4096];


    setenv("GATEWAY_INTERFACE","CGI/1.1",1);
	setenv("SERVER_PROTOCOL","HTTP/1.1",1);
	setenv("REDIRECT_STATUS","1",1);
	setenv("PATH_INFO","/Users/zdnaya/Desktop/WebServe/webpage/cgi/home.php",1);
	setenv("SERVER_PORT","8011",1);
	setenv("REQUEST_METHOD","POST",1);
	setenv("REMOTE_ADDR","127.0.0.1",1);
    setenv("GATEWAY_INTERFACE","CGI/1.1",1);
	setenv("SERVER_NAME","localhost",1);
    setenv("SERVER_PROTOCOL","HTTP/1.1",1);
    setenv("SCRIPT_FILENAME","/Users/zdnaya/Desktop/WebServe/webpage/cgi/home.php",1);
    setenv("SCRIPT_NAME","home.php",1);
    setenv("REDIRECT_STATUS","200",1);

  if (pipe(link)==-1)
    die("pipe");

  if ((pid = fork()) == -1)
    die("fork");

  if(pid == 0) {

    dup2 (link[1], STDOUT_FILENO);
    close(link[0]);
    close(link[1]);
    execve("/Users/zdnaya/goinfre/.brew/bin/php-cgi", argv, envp);
    die("execl");

  } else {

    close(link[1]);
    int nbytes = read(link[0], foo, sizeof(foo));
    printf("Output: (%.*s)\n", nbytes, foo);
    wait(NULL);

  }
  return 0;

}
