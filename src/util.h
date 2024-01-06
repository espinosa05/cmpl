#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>

#define WARN(str) fprintf(stderr, str)
#define IMPLEMENT_ME() fprintf(stderr, "unimplemented function in %s@%s:%d", __FILE__, __func__, __LINE__)
#define TODO(str) fprintf(stderr, "TODO: %s\n", str)
ssize_t get_file_size(ssize_t fd);

#endif /* UTIL_H */
