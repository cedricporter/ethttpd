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
#include <fcntl.h>
#include <ctype.h>
#include "request.h"
#include <sys/types.h>
#include <sys/stat.h>

et_event_t *et_posted_accept_events;
et_event_t *et_posted_events;
et_event_module_t et_event_module;
et_event_actions_t et_event_actions;

void et_http_init_request(et_event_t *ev);
int et_http_read_request_header(et_http_request_t *r);
void et_http_finalize_request(et_http_request_t *r);
void et_http_process_request(et_http_request_t *r);



static int
Fcntl(int fd, int cmd, int arg)
{
	int	n;

	if ((n = fcntl(fd, cmd, arg)) == -1)
		perror("fcntl error");
	return n;
}


static void make_nonblock(int fd)
{
    int val;

    /* make listenfd nonblock */
    val = Fcntl(fd, F_GETFL, 0);
    Fcntl(fd, F_SETFL, val | O_NONBLOCK);

    if (val & O_NONBLOCK)
    {
        et_log("fd %d is already nonblock", fd);
    }
}


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

    make_nonblock(connfd);

    conn = et_get_connection(connfd);
    rev = conn->read;

    /* 为新连接添加一个读事件，处理函数设置为读取 */
    et_add_event(rev, ET_READ_EVENT);
    rev->handler = et_http_init_request; /* et_event_read; */
}


static void
et_http_close_connection(et_connection_t *c)
{
    et_http_request_t 	*r;

    et_log("close fd: %d", c->fd);

    r = c->data;

    /* et_del_event(ev, ET_WRITE_EVENT); */
    et_del_event(c->read, ET_READ_EVENT);

    close(c->fd);

    if (r->file_fd > 0)
    {
        close(r->file_fd);
    }

    et_http_request_free(r);
    et_free_connetion(c);
}


/* 最后完成请求 */
void
et_http_finalize_request(et_http_request_t *r)
{
    int				n, rdsize;
    et_connection_t	*c;
    const char		*filename;
    int 			file_fd;

    et_log("et_http_finalize_request, state %d", r->state);

    c = r->connection;

    switch (r->state)
    {
    case ET_REQUEST_ERROR:
        write(c->fd, "HTTP/1.0 500 Server Error\r\n\r\nServer Error",
              strlen("HTTP/1.0 500 Server Error\r\n\r\nServer Error"));

        goto done;
    case ET_REQUEST_DONE:
        et_log("re enter %s", __func__);
        return;
    default:
        break;
    }

    filename = et_string_get_buf(r->uri) + 1;

    if (r->file_fd != -1)
    {
        file_fd = r->file_fd;
    }
    else if ((file_fd = open(filename, O_RDONLY)) == -1)
    {
        perror("open");
        et_log("open file error: %s", filename);
        write(c->fd, "HTTP/1.0 404 NOTFOUND\r\n\r\nNot Found",
              strlen("HTTP/1.0 404 NOTFOUND\r\n\r\nNot Found"));

        goto done;
    }

    et_log("open file: %s, fd = %d", filename, file_fd);

    /* first write, add header */
    if (r->file_fd == -1)
    {
        strcpy(r->buffer, "HTTP/1.0 200 OK\r\n\r\n");
        r->offset = r->buffer_data_size = strlen(r->buffer);
    }

    r->file_fd = file_fd;

    if (r->buffer_data_size > 0)
    {
        /* write */
    }

    /* 发送文件 */
    while ((rdsize = read(file_fd, r->buffer + r->offset, MAXLINE)) > 0)
    {
        et_log("read size: %d", rdsize);

        r->buffer_data_size += rdsize;

        if ((n = write(c->fd, r->buffer, r->buffer_data_size)) > 0)
        {
            if (n < r->buffer_data_size) /* 没写完 */
            {
                et_log("write is not complete!");
                exit(0);

                r->offset += rdsize - n;
            }
            else
            {
                et_log("write all data");

                /* 全部写完了，将数据大小和offset清空 */
                r->buffer_data_size = 0;
                r->offset = 0;
            }
        }
        else if (n == 0)
        {

        }
        else                            /* n < 0 */
        {
            if (errno == EWOULDBLOCK)
            {
                r->buffer_data_size = rdsize;
            }
        }
    }

    if (rdsize == 0)
    {

    }

    et_log("write done");

done:
    et_http_close_connection(c);
    r->state = ET_REQUEST_DONE;
}

/* Web Server 业务逻辑处理 */
void
et_http_core_run_phases(et_http_request_t *r)
{

}


void
et_http_request_handler(et_event_t *ev)
{
    /* et_connection_t		*c; */
    /* et_http_request_t	*r; */

    /* c = ev->data; */
    /* r = c->data; */

    /* if (ev->write) */
    /* { */
    /*     /\* r->write_event_handler(r); *\/ */
    /* } */
    /* else */
    /* { */
	/* 	/\* r->read_event_handler(r); *\/ */
    /* } */
}

void
et_http_handler(et_http_request_t *r)
{
    /* r->write_event_handler = et_http_core_run_phases; */
    /* et_http_core_run_phases(r); */

    et_http_finalize_request(r);
}

void
et_http_process_request(et_http_request_t *r)
{
    et_connection_t		*c;

    et_log("et_http_process_request");

    c = r->connection;

	c->read->handler = et_http_request_handler;
    c->write->handler = et_http_request_handler;

    et_http_handler(r);

    /* run_phases */

}


void
et_http_process_request_headers(et_event_t *ev)
{
    et_connection_t		*c;
    et_http_request_t	*r;
    /* int					n, rc; */

	et_log("et_http_process_request_headers");

    c = ev->data;
    r = c->data;

    /* rc = ET_AGAIN; */

    for (;;)
    {
        /* if (rc == ET_AGAIN) */
        /* { */
        /*     n = et_http_read_request_header(r); */
        /*     if (n == ET_AGAIN || n == ET_ERROR) */
        /*     { */
        /*         return; */
        /*     } */
        /* } */

        /* rc = et_http_parse_request_header(r); */

        /* 如果解析完成，就开始处理请求 */
        /* if (rc == ET_OK) */
        /* { */
        et_http_process_request(r);
        return;
        /* } */
    }
}

int
et_http_parse_request_line(et_http_request_t *r)
{
    const char	*buf;
    const char	*uri_start, *uri_end;
    const char	*t;

    buf = et_string_get_buf(r->header_in);

    if (buf && buf[0] == 'G' && buf[1] == 'E' && buf[2] == 'T')
    {
        et_log("request method: GET");

        uri_start = buf + 4;

        for (t = uri_start; *t; t++)
        {
            if (isspace(*t))
            {
                uri_end = t;
                break;
            }
        }

        et_string_set_with_end(r->uri, uri_start, uri_end);

        et_log("uri: %s", et_string_get_buf(r->uri));

        return ET_OK;
    }
    else
    {
        return ET_ERROR;
    }

}


static void
et_http_process_request_line(et_event_t *ev)
{
    et_connection_t		*c;
    et_http_request_t	*r;
    int					rc;
    int					n;

    et_log("et_http_process_request_line");

    c = ev->data;
    r = c->data;

    rc = ET_AGAIN;

    for (;;)
    {
        if (rc == ET_AGAIN)
        {
            n = et_http_read_request_header(r);
            /* 未就绪或者出错了 */
            if (n == ET_AGAIN || n == ET_ERROR)
            {
                return;
            }
        }

        if ((rc = et_http_parse_request_line(r)) == ET_OK)
        {
            ev->handler = et_http_process_request_headers;
            ev->handler(ev);
            return;
        }

        if (rc == ET_ERROR)
        {
            r->state = ET_REQUEST_ERROR;
            et_http_finalize_request(r);
            return;
        }
    }
}


void
et_http_init_request(et_event_t *ev)
{
    et_connection_t *c;
    et_http_request_t *r;

    et_log("et_http_init_request");

    /* create a request object */
    c = ev->data;
    r = et_http_request_create();

    c->data = r;
    r->connection = c;

	ev->handler = et_http_process_request_line;
    ev->handler(ev);
}


int
et_http_read_request_header(et_http_request_t *r)
{
    et_connection_t *c;
    et_event_t		*rev;
    char 			buf[MAXLINE];
    int 			n;

    c = r->connection;
    rev = c->read;

    et_log("et_event_read fd:%d", c->fd);

    if ((n = read(c->fd, buf, MAXLINE)) == 0) /* connection close */
    {
        /* accept后还没来得及read就会发生这种情况 */
        et_log("connection close");

        et_del_event(rev, ET_READ_EVENT);
        et_log("del event: %d", c->fd);
        close(c->fd);

        free(c->data);
        et_free_connetion(c);

        return ET_ERROR;
    }
    else if (n == -1)
    {
        if (errno == EWOULDBLOCK)
        {
            et_log("read %d EWOULDBLOCK", c->fd);
            return ET_AGAIN;
        }
        return ET_ERROR;
    }
    else                                /* > 0 */
    {
        et_log("read header size: %d", n);
        /* rev->handler = et_http_init_request; */
        /* rev->handler(rev); */

        et_string_set(r->header_in, buf);

        return ET_OK;
    }

}


void
et_event_process_posted(et_event_t **posted)
{
    et_event_t *ev;

    for (;;)
    {
        ev = (et_event_t *) *posted;

        /* pop event */
        if (ev == NULL)
        {
            return;
        }

        et_delete_posted_event(ev);

        ev->handler(ev);
    }
}


void
et_delete_posted_event(et_event_t *ev)
{
    *(ev->prev) = ev->next;             /* modified head */

    if (ev->next)
    {
        ev->next->prev = ev->prev;
    }

    ev->prev = NULL;

    et_log("delete posted event: %d ", ((et_connection_t *) ev->data)->fd);
}
