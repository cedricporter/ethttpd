/**
 * @file   main.c
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#include "ethttpd.h"
#include "utils.h"

#include "mpm_fork.h"
#include "mpm_thread.h"
#include "mpm_select.h"

#define ETHTTPD_PORT 12345


/* clean up dead children */
static void sig_chld(int sig)
{
	int status, pid;

	while (0 < (pid = waitpid(-1, &status, WNOHANG)))
		printf("Child process %d exited with status %d\n", pid, status);
}

void set_signal_func()
{
    struct sigaction sa;
    sa.sa_handler = &sig_chld;
    sa.sa_flags = SA_NOCLDSTOP;

    sigaction(SIGCHLD, &sa, NULL);
}

int initialize(int port)
{
    int listenfd;
    struct sockaddr_in servaddr;
    int sockopt = 1;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        exit(1);
    }

    setsockopt(listenfd, getprotobyname("tcp")->p_proto,
               SO_REUSEADDR, &sockopt, sizeof(sockopt));

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = PF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        et_log("Bind error");
        exit(1);
    }

    if (listen(listenfd, 100) < 0)
    {
        et_log("listen error");
        exit(1);
    }

    return listenfd;
}


int main(int argc, char **argv)
{
    int listenfd;

    set_signal_func();

    listenfd = initialize(ETHTTPD_PORT);

    mpm_select(listenfd);

    return 0;
}
