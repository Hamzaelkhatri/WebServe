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

int main(int argc, char *argv[], char *envp[])
{
    int fd;
    char buf[1024];
    int n;
    int i;
    int pid;
    int status;
    int fd_in;
    int fd_out;
    int fd_err;
    int fd_pipe[2];

    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(1);
    }
    /*

    export GATEWAY_INTERFACE="CGI/1.1"
	export SERVER_PROTOCOL="HTTP/1.1"
	export SERVER_PORT="8011"
	export REQUEST_METHOD="GET"
	export SERVER_NAME="localhost";
	export REDIRECT_STATUS=1;
	export REMOTE_ADDR="127.0.0.1";
	export PATH_INFO="php-cgi7.4";


    */
}