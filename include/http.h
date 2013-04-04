/**
 * @file   http.h
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#ifndef HTTP_H
#define HTTP_H


int handle_request(int connfd);
const char * get_mime_type(char *ext);

#endif /* HTTP_H */
