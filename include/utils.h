/**
 * @file   utils.h
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#ifndef UTILS_H
#define UTILS_H


typedef struct
{
    char *buf;
    int max_length;
    int length;
} et_string;

et_string * et_string_create(char *buf);
void et_string_set(et_string *str, char *buf);
void et_string_destroy(et_string *str);
int et_string_length(et_string *str);
const char * et_string_get_buf(et_string *str);
void et_string_set_with_end(et_string *str, char *buf, char *end);


void et_log(char *s, ...);

#endif /* UTILS_H */
