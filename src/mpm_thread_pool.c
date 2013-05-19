/**
 * @file   mpm_thread_pool.c
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#include "mpm_thread_pool.h"
#include "ethttpd.h"
#include "http.h"
#include <pthread.h>

#define THREAD_POOL_MAXTHREADS 20

static pthread_t threads[THREAD_POOL_MAXTHREADS];

static pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;

static void * handle_it(void *arg)
{
    struct sockaddr_in peeraddr;
    int listenfd, connfd;
    socklen_t peerlen;

    listenfd = *(int *)arg;
    free(arg);

    peerlen = sizeof(peeraddr);

    for (;;)
    {
        pthread_mutex_lock(&mlock);
        if ((connfd = accept(listenfd, (struct sockaddr *)&peeraddr, &peerlen)) < 0)
        {
            perror("accept");
            exit(1);
        }
        pthread_mutex_unlock(&mlock);

        handle_request(connfd);

        close(connfd);
    }

    return NULL;
}

int mpm_thread_pool(int listenfd)
{
    int i;
    int *iptr;

    et_log("mpm_thread_pool with %d children", THREAD_POOL_MAXTHREADS);

    for (i = 0; i < THREAD_POOL_MAXTHREADS; ++i)
    {
        iptr = (int *)malloc(sizeof(int));
        *iptr = listenfd;
        pthread_create(threads + i, NULL, &handle_it, iptr);
    }

    for (;;)
    {
        pause();
    }

    return 0;
}
