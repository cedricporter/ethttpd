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
    char						*request_line;
    char						*uri;

    et_http_event_handler_pt	read_event_handler;
    et_http_event_handler_pt	write_event_handler;
};



struct http_request_s
{
    method_type method;

    et_string *uri;

    char *uri_start;
    char *uri_end;

    char *query_string_start;
    char *query_string_end;

};



http_request_t *http_request_create();
void http_request_destroy(http_request_t *req);


#endif /* REQUEST_H */
