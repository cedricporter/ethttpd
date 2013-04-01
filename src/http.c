/**
 * @file   http.c
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#include "ethttpd.h"
#include "http.h"
#include "utils.h"
#include <ctype.h>

typedef struct request
{
    int method;
} HttpRequest;

typedef enum {
    M_INVALID,
    M_GET,
    M_POST
} reqtype;

static reqtype check_reqtype(char *req)
{
	if(!strncmp("GET",req,3)) return M_GET;
	if(!strncmp("POST",req,4)) return M_POST;

    return M_INVALID;
}

static int parse_header(char *buf)
{
    reqtype method;
    char *uri, *doc, *p;
    char *uri_start, *uri_end;
    char *protocol_start, *protocol_end;
    char ch;

    enum {
        sw_start = 0,
        sw_method,
        sw_spaces_before_uri,
        sw_uri,
        sw_spaces_before_protocol,
        sw_protocol,
        sw_key,
        sw_value
    } state;

    p = buf;
    state = sw_start;

    while (*p)
    {
        ch = *p;
        switch (state)
        {
        case sw_start:
        case sw_method:
            switch (method = check_reqtype(buf))
            {
            case M_GET:
                p += 3;
                break;
            case M_POST:
                p += 4;
                break;
            default:
                exit(-1);
            }
            state = sw_spaces_before_uri;
            break;
        case sw_spaces_before_uri:
            if (isspace(ch))
            {
                p++;
            }
            else
            {
                state = sw_uri;
                uri_start = p;
            }
            break;
        case sw_uri:
            if (!isspace(ch))
                p++;
            else
            {
                uri_end = p;
                state = sw_spaces_before_protocol;
            }
            break;
        case sw_spaces_before_protocol:
            if (isspace(ch))
            {
                p++;
            }
            else
            {
                state = sw_protocol;
                protocol_start = p;
            }
            break;
        case sw_protocol:
            if (!isspace(ch))
            {
                p++;
            }
            else
            {
                protocol_end = p;
            }
        }

    }


    return 0;
}

int handle_request(int connfd)
{
    char read_buff[MAXLINE];
    char buff[MAXLINE];
    int n;

    /* read */
    n = read(connfd, read_buff, MAXLINE);



    /* write back */
    snprintf(buff, sizeof(buff),
             "HTTP/1.0 200 OK\r\n"
             "Content-Type: text/plain\r\n"
             "\r\n"
             "%s", read_buff);

    n = write(connfd, buff, strlen(buff));
    if (n < 0)
    {
        return 1;
    }

    return 0;
}
