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


/* connection state */


void connection_state_table_init()
{
    connection_state_table = malloc(sizeof(connection_state) * conn_state_done);

    connection_state_table[conn_state_start] = conn_state_reading;
    connection_state_table[conn_state_reading] = conn_state_read_done;
    connection_state_table[conn_state_read_done] = conn_state_parsing;
    connection_state_table[conn_state_parsing] = conn_state_parse_done;
    connection_state_table[conn_state_parse_done] = conn_state_writing;
    connection_state_table[conn_state_writing] = conn_state_write_done;
    connection_state_table[conn_state_write_done] = conn_state_done;

}


/* connection */

connection_t * connection_create(int fd)
{
    connection_t *conn = malloc(sizeof(connection_t));
    conn->fd = fd;
    conn->pool = buffer_pool_create();
    conn->state = conn_state_start;

    return conn;
}


void connection_destroy(connection_t *conn)
{
    buffer_pool_destroy(conn->pool);

    free(conn);
}


int connection_prepare_data(connection_t *conn)
{
    char *buf = buffer_pool_new(conn->pool, MAXLINE);
    int n;
    int success = 0;
    while ((n = read(conn->fd, buf, MAXLINE)) > 0)
    {
        success |= 1;
    }

    if (success)
    {
        /* change to next state */
        conn->state = connection_state_table[conn->state];
    }

    return n;
}

int connection_parse_request_line(connection_t *conn)
{

}


int connection_parse_header(connection_t *conn)
{

}
