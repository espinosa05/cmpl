#ifndef CMPL_UTIL_H
#define CMPL_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define CMPL_PRINTF_FUNCTION(fmt_start, arg_start) __attribute__((printf(fmt_start, arg_start)))
#define STR_SYM(id) #id
#define STR_SYM_FMT_X(id) STR_SYM(id) ": %08X\n", id
#define STR_SYM_FMT_S(id) STR_SYM(id) ": %s\n", id
#define STR_SYM_FMT_D(id) STR_SYM(id) ": %d\n", id
#define STR_SYM_FMT_P(id) STR_SYM(id) ": %p\n", id
#define STR_SYM_FMT_F(id) STR_SYM(id) ": %f\n", id
#define STR_SYM_FMT_ZU(id) STR_SYM(id) ": %zu\n", id
#define STR_SYM_FMT_LU(id) STR_SYM(id) ": %lu\n", id

void log_impl(const char *tag, int lf, const char *fmt, ...) CMPL_PRINTF_FUNCTION(3, 4);
#define INFO(...) log_impl("INFO", 0, __VA_ARGS__)
#define TODO(...) log_impl("TODO", 1, __VA_ARGS__)
#define WARN(...) log_impl("WARN", 1, __VA_ARGS__)
#define ERRO(...) log_impl("ERRO", 1, __VA_ARGS__)
#define IMPL() log_impl("IMPL", "function: %s needs implementation @ %s:%d", __func__, __FILE__, __LINE__)


ssize_t get_file_size(ssize_t fd);

#endif /* CMPL_UTIL_H */
