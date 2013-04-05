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


#include "http.h"
#include "utils.h"


struct http_request_s
{
    method_type method;

    et_string *uri;

    char *uri_start;
    char *uri_end;

    char *query_string_start;
    char *query_string_end;

};

typedef struct http_request_s http_request_t;


http_request_t *http_request_create();
void http_request_destroy(http_request_t *req);


#endif /* REQUEST_H */
