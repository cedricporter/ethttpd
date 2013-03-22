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
#include <netdb.h>
#include <stdarg.h>

#define ETHTTPD_PORT 12345

void et_log(char *s, ...)
{
	va_list ap;
	va_start(ap,s);
    vfprintf(stderr, s, ap);
	va_end(ap);
}

int initialize(int port)
{
    int listenfd;
    struct sockaddr_in servaddr;
    int sockopt = 1;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        exit(1);
    }

    setsockopt(listenfd, getprotobyname("tcp")->p_proto,
               SO_REUSEADDR, &sockopt, sizeof(sockopt));

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = PF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        et_log("Bind error");
        exit(1);
    }

    if (listen(listenfd, 100) < 0)
    {
        et_log("listen error");
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

int handle(int connfd)
{
    char read_buff[MAXLINE];
    time_t ticks;
    int n;
    char buff[MAXLINE];
        
    /* read */
    n = read(connfd, read_buff, MAXLINE);

    /* write back */
    ticks = time(NULL);
    snprintf(buff, sizeof(buff),
             "HTTP/1.0 200 OK\r\n"
             "Content-Type: text/plain\r\n"
             "\r\n"
             "%.24s\r\n\r\n%s", ctime(&ticks), read_buff);
    printf("fputs\n");
    n = write(connfd, buff, strlen(buff));
    if (n < 0)
    {
        exit(1);
    }
    close(connfd);

    return 0;
}

int main(int argc, char **argv)
{
    int listenfd, connfd;
    int pid;
    struct sockaddr_in peeraddr;

	struct sigaction sa;
    sa.sa_handler = &child_reaper;
    sa.sa_flags = SA_NOCLDSTOP;

    sigaction(SIGCHLD, &sa, NULL);


    listenfd = initialize(ETHTTPD_PORT);
    
    for (;;)
    {
        socklen_t peerlen=sizeof(peeraddr);

        et_log("Waiting...\n");
        connfd = accept(listenfd, (struct sockaddr *)&peeraddr, &peerlen);
        if (connfd < 0)
        {
            if (EINTR != errno)
                printf("accept error: %s", strerror(errno));
            continue;
        }
        et_log("accept\n");

        pid = fork();

        switch (pid)
        {
        case 0:
            close(listenfd);

            exit(handle(connfd));
            break;
        default:
            close(connfd);
        }
    }

    
    return 0;
}

