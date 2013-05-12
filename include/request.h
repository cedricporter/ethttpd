/**
 * @file   request.h
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#ifndef REQUEST_H
#define REQUEST_H

#include "ethttpd.h"
#include "http.h"
#include "utils.h"

typedef void (*et_http_event_handler_pt)(et_http_request_t *r);

struct et_http_request_s
{
    et_connection_t			 	*connection;

    int 						method;

    et_string					*header_in;

    et_string					*uri;

    et_http_event_handler_pt	read_event_handler;
    et_http_event_handler_pt	write_event_handler;

    char						buffer[MAXLINE];
    int							buffer_data_size;
    int							offset;
    int 						file_fd;

    int 						state;
};

et_http_request_t * et_http_request_create();
void et_http_request_free(et_http_request_t * r);

#define ET_REQUEST_NOTHING		0
#define ET_REQUEST_DONE			-1
#define ET_REQUEST_ERROR		-2


#endif /* REQUEST_H */
