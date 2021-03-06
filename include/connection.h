/**
 * @file   connection.h
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#ifndef CONNECTION_H
#define CONNECTION_H

#include "buffer_pool.h"
#include "request.h"
#include "ethttpd.h"

struct et_connection_s
{
    void *data;

    et_event_t *read;
    et_event_t *write;

    int fd;
};

et_connection_t * et_get_connection(int fd);
void et_free_connetion(et_connection_t *c);




/* typedef enum _connection_state */
/* { */
/*     conn_state_start = 0, */
/*     conn_state_reading, */
/*     conn_state_read_done, */
/*     conn_state_parsing, */
/*     conn_state_parse_done, */
/*     conn_state_writing, */
/*     conn_state_write_done, */
/*     conn_state_done, */
/* } connection_state_t; */


/* struct connection_s */
/* { */
/*     int fd; */

/*     connection_state_t state; */

/*     http_request_t *req; */
/*     buffer_pool_t *pool; */
/* }; */


/* connection_state_t *connection_state_table; */

/* void connection_state_table_init(); */

/* connection_t * connection_create(int fd); */
/* int connection_handle(connection_t *conn); */
/* int connection_parse_header(connection_t *conn); */
/* int connection_parse_request_line(connection_t *conn); */
/* int connection_prepare_data(connection_t *conn); */
/* int connection_response(connection_t *conn); */
/* void connection_destroy(connection_t *conn); */


#endif /* CONNECTION_H */
