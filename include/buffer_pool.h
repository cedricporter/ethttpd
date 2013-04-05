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

struct buffer_pool_chunk_s
{
    char *data;
    struct buffer_pool_chunk_s *next;
};

typedef struct buffer_pool_chunk_s buffer_pool_chunk_t;

struct buffer_pool_s
{
    buffer_pool_chunk_t *head;
};

typedef struct buffer_pool_s buffer_pool_t;

#endif /* BUFFER_POOL_H */
