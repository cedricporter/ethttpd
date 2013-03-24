/**
 * @file   mpm_select.c
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#include "mpm_select.h"
#include <sys/select.h>
#include <sys/time.h>
#include "ethttpd.h"
#include <fcntl.h>


static int Fcntl(int fd, int cmd, int arg)
{
	int	n;

	if ((n = fcntl(fd, cmd, arg)) == -1)
		printf("fcntl error");
	return n;
}




int mpm_select(int listenfd)
{
    int maxfd, val;
    int nready, client[FD_SETSIZE];
    int connfd, sockfd;
    socklen_t clilen;
    int i, n;
    fd_set allset, rset;
    struct sockaddr_in cliaddr;
    char buf[MAXLINE];

    maxfd = listenfd;

    val = Fcntl(listenfd, F_GETFL, 0);
    Fcntl(listenfd, F_SETFL, val | O_NONBLOCK);


    for (i = 0; i < FD_SETSIZE; ++i)
    {
        client[i] = -1;
    }

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);

    for (;;)
    {
        rset = allset;

        printf("select()\n");

        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);

        /* new client */
        if (FD_ISSET(listenfd, &rset))
        {
            clilen = sizeof(cliaddr);
            if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0)
            {
                perror("accept()");
                exit(1);
            }

            for (i = 0; i < FD_SETSIZE; ++i)
            {
                if (client[i] < 0)
                {
                    client[i] = connfd;
                    break;
                }
            }

            if (i == FD_SETSIZE)
            {
                exit(1);
            }

            FD_SET(connfd, &allset);
            if (connfd > maxfd)
                maxfd = connfd;

            if (--nready < 0)
                continue;
        }

        for (i = 0; i < FD_SETSIZE; ++i)
        {
            if ((sockfd = client[i]) < 0)
                continue;
            if (FD_ISSET(sockfd, &rset))
            {
                printf("reading\n");
                if ((n = read(sockfd, buf, MAXLINE)) == 0)
                {
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                    printf("close sockfd\n");
                }
                else if (n < 0)
                {
                    exit(1);
                }
                else
                {
                    printf("write and close\n");
                    write(sockfd, "HTTP/1.0 200 OK\r\n\r\n", strlen("HTTP/1.1 200 OK\r\n\r\n"));
                    write(sockfd, buf, n);
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                }

                if (--nready <= 0)
                {
                    break;
                }
            }
        }
    }

}
