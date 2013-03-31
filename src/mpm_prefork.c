/**
 * @file   mpm_prefork.c
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#include "mpm_prefork.h"
#include "ethttpd.h"
#include "http.h"

#define PREFORK_MAX_CHIIDREN 20

static int nchildren = PREFORK_MAX_CHIIDREN;
static int pids[PREFORK_MAX_CHIIDREN];

static void sig_int(int signo)
{
    int i;

    for (i = 0; i < nchildren; ++i)
    {
        kill(pids[i], SIGTERM);
    }

    while (wait(NULL) > 0)
        ;

    if (errno != ECHILD)
    {
        printf("wait error");
        exit(1);
    }
    exit(0);
}

static int child_main(int i, int listenfd)
{
    int connfd;
    socklen_t clilen;
    struct sockaddr_in cliaddr;

    for (;;)
    {
        clilen = sizeof(cliaddr);
        if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0)
        {
            if (errno == EINTR)
                continue;

            perror("accept error");
            exit(1);
        }

        handle_request(connfd);
        close(connfd);
    }

    return 0;
}

static int child_make(int i, int listenfd)
{
    pid_t pid;

    if ((pid = fork()) > 0)             /* parent */
        return pid;

    child_main(i, listenfd);

    return 0;
}

int mpm_prefork(int listenfd)
{
    int i;

    struct sigaction sa;
    sa.sa_handler = &sig_int;

    sigaction(SIGINT, &sa, NULL);

    for (i = 0; i < nchildren; ++i)
    {
        pids[i] = child_make(i, listenfd);
    }

    for (;;)
    {
        pause();
    }

    return 0;
}
