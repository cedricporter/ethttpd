/**
 * @file   http.h
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#ifndef HTTP_H
#define HTTP_H

#include "ethttpd.h"


typedef enum {
    METHOD_INVALID,
    METHOD_GET,
    METHOD_POST
} method_type;

int handle_request(int connfd);
const char * get_mime_type(char *ext);

method_type check_method(char *req);

int nonblock_handle(connection_t *conn);
int block_handle(connection_t *conn);

#endif /* HTTP_H */
