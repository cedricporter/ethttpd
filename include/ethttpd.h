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

#endif /* ETHTTPD_H */
/****/
