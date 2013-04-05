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


/* clean up dead children */
static void sig_chld(int sig)
{
	int status, pid;

	while (0 < (pid = waitpid(-1, &status, WNOHANG)))
		printf("Child process %d exited with status %d\n", pid, status);
}

static void set_signal_func()
{
    struct sigaction sa;
    sa.sa_handler = &sig_chld;
    sa.sa_flags = SA_NOCLDSTOP;

    sigaction(SIGCHLD, &sa, NULL);
}

int mpm_fork(int listenfd)
{
    int pid;
    int connfd;
    struct sockaddr_in peeraddr;

    et_log("mpm_fork");

    set_signal_func();

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
