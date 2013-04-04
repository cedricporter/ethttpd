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

void et_log(char *s, ...)
{
	va_list ap;
    fprintf(stderr, "[ethttpd log]: ");
	va_start(ap, s);
    vfprintf(stderr, s, ap);
	va_end(ap);
    fprintf(stderr, "\n");
}
