/**
 * @file   mpm_thread.c
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#include "mpm_thread.h"
#include "ethttpd.h"
#include <pthread.h>
#include "http.h"

static void * handle_it(void *arg)
{
    int connfd;

    connfd = *((int *)arg);
    free(arg);

    pthread_detach(pthread_self());

    handle_request(connfd);

    close(connfd);

    return 0;
}


int mpm_thread(int listenfd)
{
    struct sockaddr_in peeraddr;
    pthread_t tid;
    int *iptr;

    et_log("mpm_thread");

    for (;;)
    {
        socklen_t peerlen = sizeof(peeraddr);
        iptr = (int *)malloc(sizeof(int));

        if ((*iptr = accept(listenfd, (struct sockaddr *)&peeraddr, &peerlen)) < 0)
        {
            exit(1);
        }

        pthread_create(&tid, NULL, &handle_it, iptr);
    }

    return 0;
}
