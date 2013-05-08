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
static unsigned int nevents;

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
    if (event_index == NULL)
    {
        FD_ZERO(&master_read_fd_set);
        FD_ZERO(&master_write_fd_set);
        nevents = 0;
    }

    if (event_index)
    {
        free(event_index);
    }

    event_index = calloc(1024, sizeof(et_event_t *));

    max_fd = -1;
}


static void
et_select_add_event(et_event_t *ev, int event)
{
    et_connection_t *c;

    c = ev->data;

    if (event == ET_READ_EVENT)
    {
        FD_SET(c->fd, &master_read_fd_set);
    }
    else if (event == ET_WRITE_EVENT)
    {
        FD_SET(c->fd, &master_write_fd_set);
    }

    if (max_fd != -1 && max_fd < c->fd)
    {
        max_fd = c->fd;
    }

    event_index[nevents] = ev;
    ev->index = nevents;
    nevents++;
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
    unsigned int i;
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






/* ------------------------------ */




struct client_info_s
{
    connection_t *conn;
};

typedef struct client_info_s client_info_t;


/* static int Fcntl(int fd, int cmd, int arg) */
/* { */
/* 	int	n; */

/* 	if ((n = fcntl(fd, cmd, arg)) == -1) */
/* 		perror("fcntl error"); */
/* 	return n; */
/* } */


/* static void make_nonblock(int fd) */
/* { */
/*     int val; */

/*     /\* make listenfd nonblock *\/ */
/*     val = Fcntl(fd, F_GETFL, 0); */
/*     Fcntl(fd, F_SETFL, val | O_NONBLOCK); */

/*     if (val & O_NONBLOCK) */
/*     { */
/*         et_log("fd %d is already nonblock", fd); */
/*     } */
/* } */


/* int mpm_select_2(int listenfd) */
/* { */
/*     int maxfd; */
/*     int nready; */
/*     client_info_t clients[FD_SETSIZE]; */
/*     int connfd, sockfd; */
/*     socklen_t clilen; */
/*     int i, n; */
/*     fd_set allset, rset, wset; */
/*     struct sockaddr_in cliaddr; */
/*     char buf[MAXLINE]; */
/*     connection_t *conn; */

/*     et_log("mpm_select"); */

/*     maxfd = listenfd; */

/*     /\* make listenfd nonblock *\/ */
/*     make_nonblock(listenfd); */

/*     for (i = 0; i < FD_SETSIZE; ++i) */
/*     { */
/*         clients[i].conn = NULL; */
/*     } */

/*     FD_ZERO(&allset); */
/*     FD_ZERO(&wset); */
/*     FD_SET(listenfd, &allset); */

/*     for (;;) */
/*     { */
/*         rset = allset; */

/*         et_log("select()"); */
/*         nready = select(maxfd + 1, &rset, NULL, NULL, NULL); */

/*         /\* new client *\/ */
/*         if (FD_ISSET(listenfd, &rset)) */
/*         { */
/*             clilen = sizeof(cliaddr); */
/*             if ((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) < 0) */
/*             { */
/*                 perror("accept()"); */
/*                 exit(1); */
/*             } */

/*             make_nonblock(connfd); */

/*             /\* new client *\/ */
/*             for (i = 0; i < FD_SETSIZE; ++i) */
/*             { */
/*                 if (clients[i].conn == 0) */
/*                 { */
/*                     clients[i].conn = connection_create(connfd); */
/*                     break; */
/*                 } */
/*             } */

/*             if (i == FD_SETSIZE) */
/*             { */
/*                 exit(1); */
/*             } */

/*             FD_SET(connfd, &allset); */
/*             if (connfd > maxfd) */
/*                 maxfd = connfd; */

/*             if (--nready < 0) */
/*                 continue; */
/*         } */

/*         /\* serve client *\/ */
/*         for (i = 0; i < FD_SETSIZE; ++i) */
/*         { */

/*             if (!(conn = clients[i].conn) || (sockfd = conn->fd) < 0) */
/*                 continue; */
/*             if (FD_ISSET(sockfd, &rset)) */
/*             { */
/*                 et_log("reading"); */

/*                 /\* for (;;) *\/ */
/*                 /\* { *\/ */
/*                 /\*     if ((n = connection_prepare_data(conn)) > 0) *\/ */
/*                 /\*     { *\/ */
/*                 /\*         FD_SET(conn->fd, &wset); *\/ */
/*                 /\*     } *\/ */
/*                 /\*     else if (n < 0) *\/ */
/*                 /\*     { *\/ */
/*                 /\*         if (errno == EWOULDBLOCK) *\/ */
/*                 /\*             continue; *\/ */
/*                 /\*     } *\/ */
/*                 /\*     else *\/ */
/*                 /\*     { *\/ */
/*                 /\*         /\\* done *\\/ *\/ */
/*                 /\*         close(sockfd); *\/ */
/*                 /\*         FD_CLR(sockfd, &allset); *\/ */
/*                 /\*         et_log("close sockfd"); *\/ */

/*                 /\*         connection_destroy(clients[i].conn); *\/ */
/*                 /\*         clients[i].conn = NULL; *\/ */
/*                 /\*     } *\/ */
/*                 /\* } *\/ */

/*                 if ((n = read(sockfd, buf, MAXLINE)) == 0) */
/*                 { */
/*                     /\* done *\/ */
/*                     close(sockfd); */
/*                     FD_CLR(sockfd, &allset); */
/*                     et_log("close sockfd"); */

/*                     connection_destroy(clients[i].conn); */
/*                     clients[i].conn = NULL; */
/*                 } */
/*                 else if (n < 0) */
/*                 { */
/*                     if (errno == EWOULDBLOCK) */
/*                     { */
/*                         continue; */
/*                     } */
/*                     perror("read in mpm_select"); */
/*                     exit(1); */
/*                 } */
/*                 else                    /\* n > 0 *\/ */
/*                 { */
/*                     et_log("write and close"); */
/*                     write(sockfd, "HTTP/1.0 200 OK\r\n\r\n", strlen("HTTP/1.1 200 OK\r\n\r\n")); */
/*                     write(sockfd, buf, n); */
/*                     close(sockfd); */
/*                     FD_CLR(sockfd, &allset); */

/*                     connection_destroy(clients[i].conn); */
/*                     clients[i].conn = NULL; */
/*                 } */

/*                 if (--nready <= 0) */
/*                 { */
/*                     break; */
/*                 } */
/*             } */
/*         } */
/*     } */

/* } */
