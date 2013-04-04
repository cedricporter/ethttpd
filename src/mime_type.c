/**
 * @file   mime_type.c
 * @author Hua Liang[Stupid ET]
 *
 * @brief
 *
 *
 */

/* #include "mime_type.h" */

#include <string.h>


typedef struct EXT2MIME
{
    const char *ext;
    const char *mime;
} ext2mime;


static const ext2mime html = {"html", "text/html"};
static const ext2mime htm = {"htm", "text/html"};
static const ext2mime jpg = {"jpg", "image/jpeg"};
static const ext2mime jpeg = {"jpeg", "image/jpeg"};

static const ext2mime * mime_types[] = {
    &html, &htm, &jpg, &jpeg, NULL
};

const char * get_mime_type(const char *ext)
{
    int i;
    for (i = 0; mime_types[i]; ++i)
    {
        if (!strcmp(ext, mime_types[i]->ext))
        {
            return mime_types[i]->mime;
        }
    }

    return NULL;
}
