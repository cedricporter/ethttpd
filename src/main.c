/**
 * @file   main.c
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#include "ethttpd.h"
#include "utils.h"
#include <time.h>

#define ETHTTPD_PORT 12345


/* clean up dead children */
static void sig_chld(int sig)
{
	int status, pid;

	while (0 < (pid = waitpid(-1, &status, WNOHANG)))
		printf("Child process %d exited with status %d\n", pid, status);
}

int initialize(int port)
{
    int listenfd;
    struct sockaddr_in servaddr;
    int sockopt = 1;

    
	struct sigaction sa;
    sa.sa_handler = &sig_chld;
    sa.sa_flags = SA_NOCLDSTOP;

    sigaction(SIGCHLD, &sa, NULL);
    

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

int handle_request(int connfd)
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
        return 1;
    }
    close(connfd);

    return 0;
}

int mpm_fork(int listenfd)
{
    int pid;
    int connfd;
    struct sockaddr_in peeraddr;

    for (;;)
    {
        socklen_t peerlen = sizeof(peeraddr);

        et_log("Waiting...\n");
        
        if ((connfd = accept(listenfd, (struct sockaddr *)&peeraddr, &peerlen)) < 0)
        {
            if (EINTR != errno)
                printf("accept error: %s", strerror(errno));
            continue;
        }

        if ((pid = fork()) == 0)
        {
            close(listenfd);
            exit(handle_request(connfd));
        }
        
        close(connfd);
    }        
}

int main(int argc, char **argv)
{
    int listenfd;

    listenfd = initialize(ETHTTPD_PORT);

    mpm_fork(listenfd);
    
    return 0;
}

