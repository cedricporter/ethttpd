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
#include "mpm_prefork.h"
#include "mpm_thread_pool.h"

#define ETHTTPD_PORT 12345


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

    listenfd = initialize(ETHTTPD_PORT);

    mpm_thread_pool(listenfd);

    return 0;
}
