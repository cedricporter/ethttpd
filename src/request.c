/**
 * @file   request.c
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#include "request.h"
#include "ethttpd.h"


http_request_t *http_request_create()
{
    http_request_t *req = malloc(sizeof(http_request_t));
    memset(req, 0, sizeof(http_request_t));

    return req;
}


void http_request_destroy(http_request_t *req)
{
    free(req);
}
