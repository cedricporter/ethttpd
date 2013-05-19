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
#include "http.h"
#include "buffer_pool.h"
#include "utils.h"
#include <string.h>
#include <ctype.h>
#include "et_event.h"

et_connection_t *
et_get_connection(int fd)
{
    et_connection_t	*c;
    et_event_t		*rev, *wev;

    c = (et_connection_t *)calloc(1, sizeof(et_connection_t));
    rev = (et_event_t *)calloc(1, sizeof(et_event_t));
    wev = (et_event_t *)calloc(1, sizeof(et_event_t));

    c->fd = fd;
    c->write = wev;
    c->read = rev;

    rev->data = c;
    wev->data = c;

    /* wev->write = 1; */

    return c;
}

void
et_free_connetion(et_connection_t *c)
{
    free(c->read);
    free(c->write);
    free(c);
}
