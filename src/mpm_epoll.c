/**
 * @file   mpm_epoll.c
 * @author Hua Liang[Stupid ET] 
 * 
 * @brief  
 * 
 * 
 */

#include "mpm_epoll.h"
#include <sys/epoll.h>
#include <sys/time.h>
#include "ethttpd.h"
#include <fcntl.h>
#include "connection.h"
#include "et_event.h"
#include "mpm_select.h"

static void et_epoll_add_event(et_event_t *ev, int event);
static void et_epoll_del_event(et_event_t *ev, int event);
static void et_epoll_init();
static void et_epoll_process_event();

static struct epoll_event	*event_list;
static int ep;

et_event_module_t et_epoll_module = {
    {
        et_epoll_add_event,
        et_epoll_del_event,
        et_epoll_process_event,
        et_epoll_init
    }
};


static void
et_epoll_init()
{
    ep = epoll_create(4096);
    event_list = (struct epoll_event *)malloc(4096 * sizeof(struct epoll_event));
    et_log("epoll_create : %d", ep);
}


static void
et_epoll_add_event(et_event_t *ev, int event)
{
    et_connection_t 	*c;
    int 				op;
    struct epoll_event 	ee;
    unsigned int		events;

    c = (et_connection_t *)ev->data;

    if (event == ET_READ_EVENT)
    {
        events = EPOLLIN;
    }
    else
    {
        events = EPOLLOUT;
    }

    op = EPOLL_CTL_ADD;

    ee.events = events | 0;
    ee.data.ptr = (void *)c;

    et_log("epoll_ctl %d", c->fd);

    if (epoll_ctl(ep, op, c->fd, &ee) == -1)
    {
        et_log("epoll_ctl error");
    }
}

static void
et_epoll_del_event(et_event_t *ev, int event)
{
    et_connection_t 	*c;
    int 				op;
    struct epoll_event 	ee;
    /* unsigned int		events; */

    c = (et_connection_t *)ev->data;

    op = EPOLL_CTL_DEL;
    ee.events = 0;
    ee.data.ptr = NULL;

    if (epoll_ctl(ep, op, c->fd, &ee) == -1)
    {
        et_log("epoll_ctl error");
    }
}


static void
et_epoll_process_event()
{
    int				events;
    int				i;
    et_connection_t	*c;
    et_event_t		*rev;
    unsigned int 	revents;
    et_event_t 		**queue;


    events = epoll_wait(ep, event_list, 100, 100000);

    if (events == -1)
    {
        if (errno == EINTR)
        {
            return;
        }
        /* et_log("epoll_wait error"); */
        perror("epoll_wait");
        return;
    }

    if (events == 0)
    {
        et_log("no events");
    }

    for (i = 0; i < events; i++)
    {
        c = (et_connection_t *)event_list[i].data.ptr;
        rev = c->read;

        revents = event_list[i].events;

        if (revents & (EPOLLERR | EPOLLHUP))
        {
            et_log("epoll_wait revents error");
        }

        if (revents & EPOLLIN)
        {
            queue = (et_event_t **) (rev->accept ? &et_posted_accept_events :
                                     &et_posted_events);

            et_locked_post_event(rev, queue);
        }
        
    }
}
