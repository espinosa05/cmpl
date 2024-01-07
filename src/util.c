#include "util.h"
#include <unistd.h>

ssize_t get_file_size(ssize_t fd)
{
    ssize_t size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    return size;
}

void log_impl(const char *tag, int lf, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "[%s]: ", tag);
    vfprintf(stderr, fmt, args);
    if (lf)
        printf("\n");
    va_end(args);
}
