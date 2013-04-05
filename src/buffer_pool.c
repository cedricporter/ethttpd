/**
 * @file   buffer_pool.c
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

#include "buffer_pool.h"
#include <string.h>
#include <stdlib.h>


static buffer_pool_chunk_t * buffer_pool_chunk_create(
    char *data,
    buffer_pool_chunk_t *next)
{
    buffer_pool_chunk_t *pool_chunk;
    pool_chunk->data = data;
    pool_chunk->next = next;

    return pool_chunk;
}


static void buffer_pool_chunk_destroy(buffer_pool_chunk_t *chunk)
{
    if (chunk)
    {
        if (chunk->data)
            free(chunk->data);
        free(chunk);
    }
}

/* append buffer to a pool */
static void buffer_pool_append(buffer_pool_t *pool, char *buf)
{
    if (!pool->head)
    {
        pool->head = buffer_pool_chunk_create(buf, NULL);
    }
    else
    {
        pool->head->next = buffer_pool_chunk_create(buf, NULL);
    }
}


/* create a buffer pool */
buffer_pool_t *buffer_pool_create()
{
    buffer_pool_t *pool = malloc(sizeof(buffer_pool_t));

    pool->head = NULL;

    return pool;
}


void buffer_pool_destroy(buffer_pool_t *pool)
{
    buffer_pool_chunk_t *tmp, *chunk = pool->head;

    while (chunk)
    {
        tmp = chunk;
        chunk = chunk->next;

        buffer_pool_chunk_destroy(tmp);
    }

    free(pool);
}


/* create and return a new buffer with desired size  */
char * buffer_pool_new(buffer_pool_t *pool, unsigned int size)
{
    char * buf = malloc(size);

    buffer_pool_append(pool, buf);

    return buf;
}
