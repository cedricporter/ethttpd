/**
 * @file   main.c
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#include "ethttpd.h"
#include <time.h>

#define ETHTTPD_PORT 12345

int initialize(int port)
{
    int listenfd;
    struct sockaddr_in servaddr;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        exit(1);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        exit(1);
    }

    if (listen(listenfd, 100) < 0)
    {
        exit(1);
    }

    return listenfd;
}

/* clean up dead children */
static void child_reaper(int sig)
{
	int status, pid;

	while(0 < (pid = waitpid(-1, &status, WNOHANG)))
		printf("Child process %d exited with status %d\n",pid,status);
}

int main(int argc, char **argv)
{
    int listenfd, connfd;
    char buff[MAXLINE];
    time_t ticks;
    int n, pid;
    FILE *fh;
    struct sockaddr_in peeraddr;

	struct sigaction sa = {
		.sa_handler = &child_reaper,
		.sa_flags = SA_NOCLDSTOP
	};

    /* sigaction(SIGCHLD, &sa, NULL); */


    listenfd = initialize(ETHTTPD_PORT);
    
    for (;;)
    {
        socklen_t peerlen=sizeof(peeraddr);

        connfd = accept(listenfd, (struct sockaddr *)&peeraddr, &peerlen);
        if (connfd < 0)
        {
            printf("accept error: %d", errno);
            exit(1);
        }

        pid = fork();

        switch (pid)
        {
        case 0:
            close(listenfd);
            ticks = time(NULL);
            snprintf(buff, sizeof(buff),
                     "HTTP/1.0 200 OK\r\n"
                     "Content-Type: text/plain\r\n"
                     "\r\n"
                     "\r\n%.24s\r\n", ctime(&ticks));
            fh = fdopen((int)connfd, "r+");
            fputs(buff, fh);
            /* n = write(connfd, buff, strlen(buff)); */
            if (n < 0)
            {
                exit(1);
            }
            /* fclose(fh); */
            /* close(connfd); */
            exit(0);
            break;
        default:
            close(connfd);
        }
    }

    
    return 0;
}

