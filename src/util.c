#include "util.h"
#include <unistd.h>

ssize_t get_file_size(ssize_t fd)
{
    ssize_t size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    return size;
}
