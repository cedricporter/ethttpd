/**
 * @file   connection.c
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#include "connection.h"
#include "request.h"
#include "ethttpd.h"
#include "http.h"
#include "buffer_pool.h"
#include "utils.h"
#include <string.h>
#include <ctype.h>
#include "et_event.h"

et_connection_t *
et_get_connection(int fd)
{
    et_connection_t	*c;
    et_event_t		*rev, *wev;

    c = calloc(1, sizeof(et_connection_t));
    rev = calloc(1, sizeof(et_event_t));
    wev = calloc(1, sizeof(et_event_t));

    c->fd = fd;
    c->write = wev;
    c->read = rev;

    rev->data = c;
    wev->data = c;

    /* wev->write = 1; */

    return c;
}

void
et_free_connetion(et_connection_t *c)
{
    free(c->read);
    free(c->write);
    free(c);
}


/* /\* connection state *\/ */

/* void connection_state_table_init() */
/* { */
/*     connection_state_table = malloc(sizeof(connection_state_t) * conn_state_done); */

/*     connection_state_table[conn_state_start] = conn_state_reading; */
/*     connection_state_table[conn_state_reading] = conn_state_read_done; */
/*     connection_state_table[conn_state_read_done] = conn_state_parsing; */
/*     connection_state_table[conn_state_parsing] = conn_state_parse_done; */
/*     connection_state_table[conn_state_parse_done] = conn_state_writing; */
/*     connection_state_table[conn_state_writing] = conn_state_write_done; */
/*     connection_state_table[conn_state_write_done] = conn_state_done; */
/* } */


/* static void connection_change_next_state(connection_t *conn) */
/* { */
/*     conn->state = connection_state_table[conn->state]; */
/* } */


/* /\* connection *\/ */

/* connection_t * connection_create(int fd) */
/* { */
/*     connection_t *conn = malloc(sizeof(connection_t)); */
/*     conn->fd = fd; */
/*     conn->pool = buffer_pool_create(); */
/*     conn->state = conn_state_start; */

/*     return conn; */
/* } */


/* void connection_destroy(connection_t *conn) */
/* { */
/*     buffer_pool_destroy(conn->pool); */

/*     free(conn); */
/* } */


/* int connection_prepare_data(connection_t *conn) */
/* { */
/*     char buf[MAXLINE]; */
/*     int n; */

/*     buffer_pool_chunk_t *chunk = buffer_pool_get_last(conn->pool); */

/*     /\* TODO: 这里应该考虑到post文件，而文件可能会很大。 *\/ */
/*     if ((n = read(conn->fd, buf, MAXLINE)) > 0) */
/*     { */
/*         /\* save to buffer pool *\/ */
/*         if (buffer_pool_chunk_set_at(chunk, */
/*                                      buffer_pool_chunk_length(chunk), */
/*                                      buf, */
/*                                      n) != 0) */
/*         { */
/*             /\* chunk is not big enough  *\/ */
/*             chunk = buffer_pool_new(conn->pool, MAXLINE); */
/*             buffer_pool_chunk_set(chunk, buf, n); */
/*         } */
/*     } */
/*     else if (n < 0) */
/*     { */
/*         if (errno != EWOULDBLOCK) */
/*             perror("read"); */
/*     } */

/*     return n; */
/* } */


/* int connection_response(connection_t *conn) */
/* { */
/*     buffer_pool_chunk_t *chunk; */
/*     int n; */
/*     char buf[MAXLINE]; */
/*     char *chunk_buf; */

/*     chunk = buffer_pool_get_head(conn->pool); */
/*     chunk_buf = buffer_pool_chunk_get_buf(chunk); */


/*     /\* write back *\/ */
/*     snprintf(buf, sizeof(buf), */
/*              "HTTP/1.0 200 OK\r\n" */
/*              "Content-Type: text/plain\r\n" */
/*              "\r\n" */
/*              "%s", chunk_buf); */

/*     if ((n = write(conn->fd, buf, strlen(buf))) > 0) */
/*     { */

/*     } */
/*     else if (n < 0) */
/*     { */
/*         perror("write"); */
/*     } */

/*     return 0; */
/* } */


/* int connection_parse_request_line(connection_t *conn) */
/* { */
/*     buffer_pool_chunk_t *chunk; */
/*     char *buf; */
/*     method_type method; */
/*     char *uri_start, *uri_end, *t; */
/*     et_string *uri; */
/*     http_request_t *req; */

/*     /\* grab head of data, because request line MUST be in first chunk  *\/ */
/*     chunk = buffer_pool_get_head(conn->pool); */
/*     buf = chunk->data; */

/*     switch (method = check_method(buf)) */
/*     { */
/*     case METHOD_GET: */
/*         uri_start = buf + 4; */
/*         break; */
/*     case METHOD_POST: */
/*         uri_start = buf + 5; */
/*         break; */
/*     default: */
/*         et_log("parse_header exit"); */
/*         exit(1); */
/*     } */

/*     /\* find the end of the uri  *\/ */
/*     for (t = uri_start; !isspace(*t); ++t) */
/*         ; */
/*     uri_end = t; */

/*     req = http_request_create(); */
/*     req->uri_start = uri_start; */
/*     req->uri_end = uri_end; */

/*     uri = et_string_create(NULL); */
/*     et_string_set_with_end(uri, uri_start, uri_end); */
/*     req->uri = uri; */

/*     conn->req = req; */

/*     et_log("[req]: uri: %s", et_string_get_buf(req->uri)); */

/*     return 0; */
/* } */


/* int connection_parse_header(connection_t *conn) */
/* { */

/*     return 0; */
/* } */



/* int connection_handle(connection_t *conn) */
/* { */
/*     switch (conn->state) */
/*     { */
/*     case conn_state_start: */
/*         et_log("state: start"); */
/*         connection_change_next_state(conn); */
/*         break; */
/*     case conn_state_reading: */
/*         et_log("state: reading"); */
/*         if (connection_prepare_data(conn) > 0) */
/*             connection_change_next_state(conn); */
/*         break; */
/*     case conn_state_read_done: */
/*         et_log("state: read_done"); */
/*         connection_change_next_state(conn); */
/*         break; */
/*     case conn_state_parsing: */
/*         et_log("state: parsing"); */
/*         connection_parse_request_line(conn); */
/*         connection_parse_header(conn); */
/*         connection_change_next_state(conn); */
/*         break; */
/*     case conn_state_parse_done: */
/*         et_log("state: parse done"); */
/*         connection_change_next_state(conn); */
/*         break; */
/*     case conn_state_writing: */
/*         et_log("state: writing"); */
/*         connection_response(conn); */
/*         connection_change_next_state(conn); */
/*         break; */
/*     case conn_state_write_done: */
/*         et_log("state: write done"); */
/*         connection_change_next_state(conn); */
/*         break; */
/*     case conn_state_done: */
/*         et_log("state: done"); */
/*         return 0; */
/*     default: */
/*         et_log("programming error at connection_handle"); */
/*         exit(EXIT_FAILURE); */
/*     } */

/*     return -1; */
/* } */
