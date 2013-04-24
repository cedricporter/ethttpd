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
#include "connection.h"


struct client_info_s
{
    connection_t *conn;
};

typedef struct client_info_s client_info_t;


static int Fcntl(int fd, int cmd, int arg)
{
	int	n;

	if ((n = fcntl(fd, cmd, arg)) == -1)
		perror("fcntl error");
	return n;
}


static void make_nonblock(int fd)
{
    int val;

    /* make listenfd nonblock */
    val = Fcntl(fd, F_GETFL, 0);
    Fcntl(fd, F_SETFL, val | O_NONBLOCK);

    if (val & O_NONBLOCK)
    {
        et_log("fd %d is already nonblock", fd);
    }
}


int mpm_select(int listenfd)
{
    int maxfd;
    int nready;
    client_info_t clients[FD_SETSIZE];
    int connfd, sockfd;
    socklen_t clilen;
    int i, n;
    fd_set allset, rset, wset;
    struct sockaddr_in cliaddr;
    char buf[MAXLINE];
    connection_t *conn;

    et_log("mpm_select");

    maxfd = listenfd;

    /* make listenfd nonblock */
    make_nonblock(listenfd);

    for (i = 0; i < FD_SETSIZE; ++i)
    {
        clients[i].conn = NULL;
    }

    FD_ZERO(&allset);
    FD_ZERO(&wset);
    FD_SET(listenfd, &allset);

    for (;;)
    {
        rset = allset;

        et_log("select()");
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

            make_nonblock(connfd);

            /* new client */
            for (i = 0; i < FD_SETSIZE; ++i)
            {
                if (clients[i].conn == 0)
                {
                    clients[i].conn = connection_create(connfd);
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

        /* serve client */
        for (i = 0; i < FD_SETSIZE; ++i)
        {

            if (!(conn = clients[i].conn) || (sockfd = conn->fd) < 0)
                continue;
            if (FD_ISSET(sockfd, &rset))
            {
                et_log("reading");

                /* for (;;) */
                /* { */
                /*     if ((n = connection_prepare_data(conn)) > 0) */
                /*     { */
                /*         FD_SET(conn->fd, &wset); */
                /*     } */
                /*     else if (n < 0) */
                /*     { */
                /*         if (errno == EWOULDBLOCK) */
                /*             continue; */
                /*     } */
                /*     else */
                /*     { */
                /*         /\* done *\/ */
                /*         close(sockfd); */
                /*         FD_CLR(sockfd, &allset); */
                /*         et_log("close sockfd"); */

                /*         connection_destroy(clients[i].conn); */
                /*         clients[i].conn = NULL; */
                /*     } */
                /* } */

                if ((n = read(sockfd, buf, MAXLINE)) == 0)
                {
                    /* done */
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    et_log("close sockfd");

                    connection_destroy(clients[i].conn);
                    clients[i].conn = NULL;
                }
                else if (n < 0)
                {
                    if (errno == EWOULDBLOCK)
                    {
                        continue;
                    }
                    perror("read in mpm_select");
                    exit(1);
                }
                else                    /* n > 0 */
                {
                    et_log("write and close");
                    write(sockfd, "HTTP/1.0 200 OK\r\n\r\n", strlen("HTTP/1.1 200 OK\r\n\r\n"));
                    write(sockfd, buf, n);
                    close(sockfd);
                    FD_CLR(sockfd, &allset);

                    connection_destroy(clients[i].conn);
                    clients[i].conn = NULL;
                }

                if (--nready <= 0)
                {
                    break;
                }
            }
        }
    }

}
