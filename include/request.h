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


typedef enum {
    METHOD_INVALID,
    METHOD_GET,
    METHOD_POST
} method_type;


struct http_request_s
{
    method_type method;

    char *uri_start;
    char *uri_end;

};

typedef struct http_request_s http_request_t;

#endif /* REQUEST_H */
