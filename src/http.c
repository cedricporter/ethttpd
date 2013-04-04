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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


typedef enum {
    METHOD_INVALID,
    METHOD_GET,
    METHOD_POST
} method_type;


static method_type check_method(char *req)
{
	if(!strncmp("GET",req,3)) return METHOD_GET;
	if(!strncmp("POST",req,4)) return METHOD_POST;
	return METHOD_INVALID;
}


int parse_header(char *buf)
{
    method_type method;
    char *doc, *uri, *t, *query = NULL;
    struct stat st;
    char *ext;
    int i;

    switch (method = check_method(buf))
    {
    case METHOD_GET:
        uri = buf + 4;
    case METHOD_POST:
        uri = buf + 5;
    default:
        exit(1);
    }

    doc = uri;

    if (*doc == '/') ++doc;

    for (t = doc; *t && !isspace(*t); ++t)
    {
        if (*t == '?')
        {
            query = t + 1;
            break;
        }
    }

    *t = '\0';

    if (query)
    {
        for (++t; *t && !isspace(*t); ++t)
            ;
        *t = '\0';
    }

    if (!strlen(doc)) doc = ".";

    if (stat(doc, &st) == 0)
    {
        /* directory */
        if (S_ISDIR(st.st_mode))
        {
            /* leave */

            goto leave;
        }

        /* mime type */
        ext = strrchr(doc, '.');
        if (ext)
        {
            ++ext;
            /* check if it is a cgi */
            for (i = 0; i < 0; ++i)
            {
                /* handle cgi */
                /* return */
            }

            /* check its mime type  */
        }

        if (method == METHOD_GET)
        {
            /* send file */
        }
        else
        {
            /* 400 */
        }
    }
    else
    {
        /* 404 */
    }

leave:

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
