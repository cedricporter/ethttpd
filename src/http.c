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
#include "request.h"
#include "utils.h"
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "connection.h"

method_type check_method(char *req)
{
	if (!strncmp("GET", req, 3))
    {
        return METHOD_GET;
     }
	if (!strncmp("POST", req, 4))
    {
        return METHOD_POST;
    }
	return METHOD_INVALID;
}

static int send_file(char *file_name, int connfd, const char *mime_type)
{
    return 0;
}

static int add_header(const char *key, const char *value)
{

    return 0;
}

int parse_header(char *buf)
{
    method_type method;
    char *doc, *uri, *t, *query = NULL;
    struct stat st;
    char *ext;
    int i;
    const char *mime_type = NULL;

    switch (method = check_method(buf))
    {
    case METHOD_GET:
        uri = buf + 4;
        break;
    case METHOD_POST:
        uri = buf + 5;
        break;
    default:
        printf("exit");
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

    et_log("doc: %s, uri: %s", doc, uri);

    if (stat(doc, &st) == 0)
    {
        /* directory */
        if (S_ISDIR(st.st_mode))
        {
            et_log("directory");
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
                et_log("handle cgi");
                /* handle cgi */
                return 0;
            }

            /* check its mime type  */
            mime_type = get_mime_type(ext);
        }

        if (!mime_type)
        {
            mime_type = "text/plain";
        }

        et_log("mime_type: %s", mime_type);

        if (method == METHOD_GET)
        {
            /* send file */
            et_log("send file");
        }
        else
        {
            et_log("400");
        }
    }
    else
    {
        /* 404 */
        et_log("404");
    }

leave:

    return 0;
}


int handle_request(int connfd)
{
    connection_t *conn = connection_create(connfd);

    while (connection_handle(conn) != 0)
    {
    }

    free(conn);

    return 0;
}


/* int handle_request(int connfd) */
/* { */
/*     char read_buff[MAXLINE]; */
/*     char buff[MAXLINE]; */
/*     int n; */


/*     /\* read *\/ */
/*     n = read(connfd, read_buff, MAXLINE); */


/*     /\* write back *\/ */
/*     snprintf(buff, sizeof(buff), */
/*              "HTTP/1.0 200 OK\r\n" */
/*              "Content-Type: text/plain\r\n" */
/*              "\r\n" */
/*              "%s", read_buff); */

/*     parse_header(read_buff); */

/*     n = write(connfd, buff, strlen(buff)); */
/*     if (n < 0) */
/*     { */
/*         return 1; */
/*     } */

/*     return 0; */
/* } */
