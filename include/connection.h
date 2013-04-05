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


typedef enum _connection_state
{
    conn_state_start = 0,
    conn_state_reading,
    conn_state_read_done,
    conn_state_parsing,
    conn_state_parse_done,
    conn_state_writing,
    conn_state_write_done,
    conn_state_done,
} connection_state_t;


struct connection_s
{
    int fd;
    http_request req;

    connection_state_t state;

    buffer_pool_t *pool;
};

typedef struct connection_s connection_t;


connection_state *connection_state_table;

void connection_state_table_init();

#endif /* CONNECTION_H */
