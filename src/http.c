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

#include <time.h>


int handle_request(int connfd)
{
    char read_buff[MAXLINE];
    char buff[MAXLINE];
    time_t ticks;
    int n;
        
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
