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


et_http_request_t *
et_http_request_create()
{
    et_http_request_t	*r;

    r = (et_http_request_t *)calloc(1, sizeof(et_http_request_t));
    r->header_in = et_string_create(0);
    r->uri = et_string_create(0);

    r->file_fd = -1;
    r->offset = 0;

    r->state = ET_REQUEST_NOTHING;

    return r;
}

void
et_http_request_free(et_http_request_t * r)
{
    et_string_destroy(r->header_in);
    et_string_destroy(r->uri);

    free(r);
}
