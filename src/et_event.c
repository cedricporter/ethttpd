/**
 * @file   et_event.c
 * @author Hua Liang[Stupid ET] 
 * 
 * @brief  
 * 
 * 
 */

#include "et_event.h"
#include "ethttpd.h"
#include "connection.h"
#include "mpm_select.h"

et_event_t *et_posted_accept_events;
et_event_t *et_posted_events;
et_event_module_t et_event_module;
et_event_actions_t et_event_actions;

void et_event_read(et_event_t *ev);

void
et_event_accept(et_event_t *ev)
{
    int 		connfd;
    socklen_t 	clilen;
    et_connection_t *c, *conn;
    et_event_t *rev;
    
    struct sockaddr_in cliaddr;

    c = ev->data;
    
    et_log("%s", __func__);

    clilen = sizeof(cliaddr);
    if ((connfd = accept(c->fd, (struct sockaddr *)&cliaddr, &clilen)) == -1)
    {
        perror("accept()");
        exit(1);
    }

    conn = et_get_connection(connfd);
    rev = conn->read;

    et_add_event(rev, ET_READ_EVENT);
    rev->handler = et_event_read;
    
}

void
et_http_finalize_request(et_event_t *ev)
{
    int n;
    et_connection_t *c;

    c = ev->data;
    
    if ((n = write(c->fd, "HTTP/1.0 200 OK\r\n\r\nHello",
                   strlen("HTTP/1.1 200 OK\r\n\r\nHello"))) > 0)
    {
        et_log("write back");

        close(c->fd);
        et_del_event(ev, ET_WRITE_EVENT);
        et_del_event(ev, ET_READ_EVENT);

    }
    else if (n == 0)
    {
        et_log("et_http_finalize_request n = 0");
    }
    else                                /* < 0 */
    {
        perror("write");
    }
}

void
et_http_process_request(et_event_t *ev)
{
    /* et_connection_t *c; */

    /* c = ev->data; */
    
    ev->handler = et_http_finalize_request;
    et_http_finalize_request(ev);
}

void
et_event_read(et_event_t *ev)
{
    et_connection_t *c;
    char buf[MAXLINE];
    int n;
    
    c = ev->data;
    
    et_log("et_event_read fd:%d", c->fd);

    if ((n = read(c->fd, buf, MAXLINE)) == 0)
    {
        et_log("connection close");
        et_del_event(ev, ET_READ_EVENT);
        et_log("del event: %d", c->fd);
        return;
    }
    else if (n == -1)
    {
        if (errno == EWOULDBLOCK)
        {
            et_log("EWOULDBLOCK");
            return;
        }
    }
    else
    {
        ev->handler = et_http_process_request;
        et_http_process_request(ev);
    }
}
