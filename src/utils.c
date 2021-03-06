/**
 * @file   utils.c
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#include "utils.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void et_log(const char *s, ...)
{
	va_list ap;
    fprintf(stderr, "[ethttpd log]: ");
	va_start(ap, s);
    vfprintf(stderr, s, ap);
	va_end(ap);
    fprintf(stderr, "\n");
}


et_string * et_string_create(char *buf)
{
    et_string *str = (et_string *)malloc(sizeof(et_string));
    str->length = str->max_length = buf ? strlen(buf) : 0;
    str->buf = (char *)(buf ? malloc(str->length + 1) : 0);

    return str;
}


void et_string_destroy(et_string *str)
{
    if (str)
    {
        if (str->buf) free(str->buf);
        free(str);
    }
}


void et_string_set(et_string *str, const char *buf)
{
    int len = strlen(buf);
    char *p;

    /* realloc memory if necessary */
    if (len > str->max_length)
    {
        p = (char *)realloc(str->buf, len + 1);
        if (p)
        {
            if (str->buf)
            {
                free(str->buf);
            }

            str->buf = p;
            str->max_length = len;
        }
    }

    str->length = len;

    strcpy(str->buf, buf);
}


void
et_string_set_with_end(et_string *str, const char *buf,
                       const char *end)
{
    int len = end - buf + 1;            /* + 1 for '\0' */
    char *p;

    if (len > str->max_length)
    {
        p = (char *)realloc(str->buf, len + 1);
        if (p)
        {
            if (str->buf)
            {
                free(str->buf);
            }
            str->buf = p;
            str->max_length = len;
        }
    }

    str->length = len;

    strncpy(str->buf, buf, len);
    str->buf[len - 1] = '\0';
}


int et_string_length(et_string *str)
{
    return str ? str->length : 0;
}


const char * et_string_get_buf(et_string *str)
{
    if (str)
    {
        return str->buf;
    }
    else
    {
        return "";
    }
}
