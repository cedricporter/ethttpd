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
#include "connection.h"

#define ETHTTPD_PORT 12345

void et_process_events_and_timers();
int initialize(int port);
void et_init_cycle(int listenfd);
void et_process_cycle();


int initialize(int port)
{
    int listenfd;
    struct sockaddr_in servaddr;
    int on=1;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        exit(1);
    }

    et_log("listenfd: %d", listenfd);

    if((setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0)
    {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = PF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind");
        et_log("Bind error");
        exit(1);
    }

    if (listen(listenfd, 100) < 0)
    {
        et_log("listen error");
        exit(1);
    }

    /* connection_state_table_init(); */

    return listenfd;
}


void et_init_cycle(int listenfd)
{
    et_event_t *rev;
    et_connection_t *c;

    c = et_get_connection(listenfd);

    rev = c->read;

    rev->accept = 1;

    et_add_event(rev, ET_READ_EVENT);

    rev->handler = et_event_accept;
}


void
et_process_cycle()
{
    for (;;)
    {
        et_process_events_and_timers();
    }
}


void
et_process_events_and_timers()
{
    et_process_events();

    /* accept events */
    if (et_posted_accept_events)
    {
        et_event_process_posted(&et_posted_accept_events);
    }

    /* common events */
    if (et_posted_events)
    {
        et_event_process_posted(&et_posted_events);
    }
}


int main(int argc, char **argv)
{
    int listenfd;
    int config;

    listenfd = initialize(ETHTTPD_PORT);

    config = 1;

    switch (config)
    {
    case 1:
        et_event_module = et_empty_module;
        et_event_actions = et_event_module.actions;

        mpm_prefork(listenfd);
        break;
    case 2:
        et_event_module = et_select_module;
        et_event_actions = et_event_module.actions;

        et_event_actions.init();

        et_init_cycle(listenfd);

        et_process_cycle();
        break;
    }
    return 0;
}
