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
#include "ethttpd.h"


/* chunk */

static buffer_pool_chunk_t * buffer_pool_chunk_create(int size)
{
    buffer_pool_chunk_t *pool_chunk = malloc(sizeof(buffer_pool_chunk_t));
    pool_chunk->data = malloc(size);
    pool_chunk->length = 0;
    pool_chunk->max_length = size;

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


int buffer_pool_chunk_set(buffer_pool_chunk_t *chunk, char *data, int length)
{
    if (length > chunk->max_length)
    {
        return -1;
    }

    memcpy(chunk->data, data, length);
    chunk->length = length;

    return 0;
}


int buffer_pool_chunk_set_at(buffer_pool_chunk_t *chunk, int where, char *data, int length)
{
    if (0 <= where && length + where < chunk->max_length)
    {
        memcpy(chunk->data + where, data, length);
        chunk->length = where + length;

        return 0;
    }

    return -1;
}

int buffer_pool_chunk_length(buffer_pool_chunk_t *chunk)
{
    return chunk->length;
}


int buffer_pool_chunk_max_length(buffer_pool_chunk_t *chunk)
{
    return chunk->max_length;
}


/* pool */

/* append buffer to a pool */
static void buffer_pool_append(buffer_pool_t *pool, buffer_pool_chunk_t *chunk)
{
    if (!pool->head)
    {
        pool->head = chunk;
    }
    else
    {
        pool->head->next = chunk;
    }
}


/* create a buffer pool */
buffer_pool_t *buffer_pool_create()
{
    buffer_pool_t *pool = malloc(sizeof(buffer_pool_t));
    pool->head = buffer_pool_chunk_create(MAXLINE);

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
buffer_pool_chunk_t * buffer_pool_new(buffer_pool_t *pool, int size)
{
    buffer_pool_chunk_t *chunk = buffer_pool_chunk_create(size);
    buffer_pool_append(pool, chunk);
    return chunk;
}


buffer_pool_chunk_t *buffer_pool_get_head(buffer_pool_t *pool)
{
    return pool->head;
}


buffer_pool_chunk_t *buffer_pool_get_last(buffer_pool_t *pool)
{
    buffer_pool_chunk_t *chunk;

    for (chunk = pool->head; chunk->next; chunk = chunk->next)
        ;

    return chunk;
}
