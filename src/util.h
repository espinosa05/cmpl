#ifndef CMPL_UTIL_H
#define CMPL_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define TRUE 1
#define FALSE 0

#define CMPL_PRINTF_FUNCTION(fmt_start, arg_start) __attribute__((format (printf, fmt_start, arg_start)))
#define STR_SYM(id) #id
#define STR_SYM_FMT_X(id) STR_SYM(id) ": %08X\n", id
#define STR_SYM_FMT_S(id) STR_SYM(id) ": %s\n", id
#define STR_SYM_FMT_D(id) STR_SYM(id) ": %d\n", id
#define STR_SYM_FMT_P(id) STR_SYM(id) ": %p\n", id
#define STR_SYM_FMT_F(id) STR_SYM(id) ": %f\n", id
#define STR_SYM_FMT_ZU(id) STR_SYM(id) ": %zu\n", id
#define STR_SYM_FMT_LU(id) STR_SYM(id) ": %lu\n", id

#define FATAL 1
#define NON_FATAL !FATAL

void log_impl(const char *tag, int lf, const char *fmt, ...) CMPL_PRINTF_FUNCTION(3, 4);
#define USAGE(...) log_impl("USAGE", 1, __VA_ARGS__)
#define INFO(...) log_impl("INFO", 0, __VA_ARGS__)
#define TODO(str) log_impl("TODO", 1, "function: '%s' @ %s:%d: %s", __func__, __FILE__, __LINE__, str)
#define WARN(...) log_impl("WARN", 1, __VA_ARGS__)
#define ERRO(...) log_impl("ERRO", 1, __VA_ARGS__)
#define IMPL() log_impl("IMPL", 1, "function: '%s' needs implementation @ %s:%d", __func__, __FILE__, __LINE__)
#define ASSERT(fatal, cnd, msg)\
    do {                \
        if (!(cnd)) {   \
            log_impl("ASRT", 1, "assertion failed: '"#cnd"' => \"%s\"", msg); \
            if (fatal)  \
                exit(1);\
        }               \
    } while (0)
ssize_t get_file_size(ssize_t fd);

#endif /* CMPL_UTIL_H */
