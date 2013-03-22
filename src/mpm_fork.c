/**
 * @file   mpm_fork.c
 * @author Hua Liang[Stupid ET] 
 * 
 * @brief  
 * 
 * 
 */

#include "ethttpd.h"
#include "mpm_fork.h"
#include "utils.h"
#include "http.h"

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
