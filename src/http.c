/**
 * @file   http.c
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#include "ethttpd.h"
#include "http.h"
#include "utils.h"


int handle_request(int connfd)
{
    char read_buff[MAXLINE];
    char buff[MAXLINE];
    int n;

    /* read */
    n = read(connfd, read_buff, MAXLINE);

    /* write back */
    snprintf(buff, sizeof(buff),
             "HTTP/1.0 200 OK\r\n"
             "Content-Type: text/plain\r\n"
             "\r\n"
             "%s", read_buff);

    n = write(connfd, buff, strlen(buff));
    if (n < 0)
    {
        return 1;
    }

    return 0;
}
