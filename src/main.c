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

    if (listen(listenfd, 10) < 0)
    {
        exit(1);
    }

    return listenfd;
}

int main(int argc, char **argv)
{
    int listenfd, connfd;
    char buff[MAXLINE];
    time_t ticks;
    int n;

    listenfd = initialize(ETHTTPD_PORT);
    
    for (;;)
    {
        connfd = accept(listenfd, NULL, NULL);

        ticks = time(NULL);
        snprintf(buff, sizeof(buff),
                 "HTTP/1.0 200 OK\r\n"
                 "Content-Type: text/plain\r\n"
                 "Connection: close\r\n"
                 "\r\n%.24s\r\n", ctime(&ticks));
        n = write(connfd, buff, strlen(buff));
        if (n < 0)
        {
            exit(1);
        }
        close(connfd);
    }

    
    return 0;
}

