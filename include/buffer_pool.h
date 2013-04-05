/**
 * @file   buffer_pool.h
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#ifndef BUFFER_POOL_H
#define BUFFER_POOL_H

#include "ethttpd.h"


struct buffer_pool_chunk_s
{
    char *data;
    int length;
    int max_length;
    struct buffer_pool_chunk_s *next;
};


struct buffer_pool_s
{
    buffer_pool_chunk_t *head;
};



/* pool */
buffer_pool_t *buffer_pool_create();
void buffer_pool_destroy(buffer_pool_t *pool);
buffer_pool_chunk_t * buffer_pool_new(buffer_pool_t *pool, int size);
buffer_pool_chunk_t *buffer_pool_get_head(buffer_pool_t *pool);
buffer_pool_chunk_t *buffer_pool_get_last(buffer_pool_t *pool);


/* chunk */
int buffer_pool_chunk_length(buffer_pool_chunk_t *chunk);
int buffer_pool_chunk_max_length(buffer_pool_chunk_t *chunk);
int buffer_pool_chunk_set(buffer_pool_chunk_t *chunk, char *data, int length);
int buffer_pool_chunk_set_at(buffer_pool_chunk_t *chunk, int where, char *data, int length);
char * buffer_pool_chunk_get_buf(buffer_pool_chunk_t *chunk);



#endif /* BUFFER_POOL_H */
