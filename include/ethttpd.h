/**
 * @file   ethttpd.h
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#ifndef ETHTTPD_H
#define ETHTTPD_H

typedef struct connection_s connection_t;
typedef struct http_request_s http_request_t;
typedef struct buffer_pool_chunk_s buffer_pool_chunk_t;
typedef struct buffer_pool_s buffer_pool_t;

typedef struct et_event_s et_event_t;

typedef void (*et_event_handler_pt)(et_event_t *ev);

typedef struct et_connection_s et_connection_t;
typedef struct et_http_request_s et_http_request_t;


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <netdb.h>

#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#include "utils.h"

#define MAXLINE 4096

#define ET_HTTP_METHOD_GET 		0
#define ET_HTTP_METHOD_POST		1
#define ET_HTTP_METHOD_HEAD		2


#endif /* ETHTTPD_H */
/****/
