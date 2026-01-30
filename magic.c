#include <stddef.h>
#include <magic.h>
#include "magic.h"

const char *
get_mimetype(const char *path) {
    const char *format;

    magic_t cookie = magic_open(MAGIC_MIME_TYPE);
    magic_load(cookie , NULL);
    format = magic_file(cookie, path);
    return format;
}
