/**
 * @file   mpm_select.c
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#include "mpm_select.h"
#include <sys/select.h>
#include <sys/time.h>
#include "ethttpd.h"
#include <fcntl.h>
#include "connection.h"
#include "et_event.h"


static void et_select_add_event(et_event_t *ev, int event);
static void et_select_del_event(et_event_t *ev, int event);
static void et_select_init();
static void et_select_process_event();


static fd_set master_read_fd_set;
static fd_set master_write_fd_set;
static fd_set work_read_fd_set;
static fd_set work_write_fd_set;

static int max_fd;
static int nevents;

static et_event_t **event_index;


et_event_module_t et_select_module = {
    {
        et_select_add_event,
        et_select_del_event,
        et_select_process_event,
        et_select_init
    }
};

static void
et_select_init()
{
    et_log("et_select_init");

    if (event_index == NULL)
    {
        FD_ZERO(&master_read_fd_set);
        FD_ZERO(&master_write_fd_set);
        nevents = 0;
    }
    else
    {
        free(event_index);
    }

    event_index = calloc(FD_SETSIZE, sizeof(et_event_t *));

    max_fd = -1;
}


static void
et_select_add_event(et_event_t *ev, int event)
{
    et_connection_t *c;

    c = ev->data;

    if (c->fd > FD_SETSIZE)
    {
        et_log("fd is too large: fd = %d", c->fd);
        exit(0);
    }

    if (event == ET_READ_EVENT)
    {
        FD_SET(c->fd, &master_read_fd_set);
    }
    else if (event == ET_WRITE_EVENT)
    {
        FD_SET(c->fd, &master_write_fd_set);
    }

    if (max_fd < c->fd)
    {
        max_fd = c->fd;
    }

    event_index[nevents] = ev;
    ev->index = nevents;
    nevents++;

    et_log("add event: fd: %d, index: %d",
           c->fd, ev->index);
}


static void
et_select_del_event(et_event_t *ev, int event)
{
    et_connection_t *c;
    et_event_t *e;

    c = ev->data;

    if (event == ET_READ_EVENT)
    {
        FD_CLR(c->fd, &master_read_fd_set);
    }
    else if (event == ET_WRITE_EVENT)
    {
        FD_CLR(c->fd, &master_write_fd_set);
    }

    if (ev->index < --nevents)
    {
        if (nevents < 0)
        {
            printf("nevents < 0, ev->index = %d", ev->index);
            exit(0);
        }

        /* 将最后一个event换到删除了的event的位置 */
        e = event_index[nevents];
        event_index[ev->index] = e;
        e->index = ev->index;
    }

    ev->index = INVALID_INDEX;
}

void
et_locked_post_event(et_event_t *ev, et_event_t **queue)
{
    if (ev->prev == NULL)
    {
        ev->next = (et_event_t *) *queue;
        ev->prev = (et_event_t **) queue;
        *queue = ev;

        if (ev->next)
        {
            ev->next->prev = &ev->next;
        }
    }
    else
    {
        et_log("error");
    }
}


static void
et_select_process_event()
{
    int i;
    et_connection_t *c;
    et_event_t *ev, **queue;
    int ready, nready, found;

    if (max_fd == -1)
    {
        for (i = 0; i < nevents; ++i)
        {
            c = event_index[i]->data;
            if (max_fd < c->fd)
            {
                max_fd = c->fd;
            }
        }
    }

    work_read_fd_set = master_read_fd_set;
    work_write_fd_set = master_write_fd_set;

    ready = select(max_fd + 1, &work_read_fd_set,
                   &work_write_fd_set, NULL, NULL);
    if (ready == -1)
    {
		perror("select");
        exit(-1);
    }
    et_log("select ready: %d", ready);

    nready = 0;

    for (i = 0; i < nevents; ++i)
    {
        ev = event_index[i];
        c = ev->data;
        found = 0;

        if (ev->write)
        {
            if (FD_ISSET(c->fd, &work_write_fd_set))
            {
                found = 1;
                et_log("select write: fd %d", c->fd);
            }
        }
        else
        {
            if (FD_ISSET(c->fd, &work_read_fd_set))
            {
                found = 1;
                et_log("select read: fd %d", c->fd);
            }
        }

        if (found)
        {
            ev->ready = 1;

            queue = (et_event_t **) (ev->accept ? &et_posted_accept_events :
                                     &et_posted_events);

            et_locked_post_event(ev, queue);

            nready++;
        }
    }
}
