/**
 * This is free and unencumbered software released into the public domain.
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 */

#ifndef CMPL_UTIL_H
#define CMPL_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define TRUE 1
#define FALSE 0
#define SUCCESS TRUE
#define FAILURE FALSE

/* stolen from the public domain >:) */
#include "ANSI-color-codes.h"

#define CMPL_PRINTF_FUNCTION(fmt_start, arg_start) __attribute__((format (printf, fmt_start, arg_start)))
#define STR_SYM(id) #id
#define STR_SYM_FMT_X(id) STR_SYM(id) "="BHBLU "0x%08X"CRESET, id
#define STR_SYM_FMT_S(id) STR_SYM(id) "="BHGRN "%s" CRESET, id
#define STR_SYM_FMT_D(id) STR_SYM(id) "="BHBLU "%d" CRESET, id
#define STR_SYM_FMT_P(id) STR_SYM(id) "="BHMAG "%p" CRESET, id
#define STR_SYM_FMT_F(id) STR_SYM(id) "="BHBLU "%f" CRESET, id
#define STR_SYM_FMT_ZU(id) STR_SYM(id) "="BHBLU "%zu" CRESET, id
#define STR_SYM_FMT_LU(id) STR_SYM(id) "="BHBLU "%lu" CRESET, id

#define STR_SYM_FMTCR_X(id) STR_SYM(id) "="BHBLU "0x%08X"CRESET "\n", id
#define STR_SYM_FMTCR_S(id) STR_SYM(id) "="BHGRN "%s" CRESET "\n", id
#define STR_SYM_FMTCR_D(id) STR_SYM(id) "="BHBLU "%d" CRESET "\n", id
#define STR_SYM_FMTCR_P(id) STR_SYM(id) "="BHMAG "%p" CRESET "\n", id
#define STR_SYM_FMTCR_F(id) STR_SYM(id) "="BHBLU "%f" CRESET "\n", id
#define STR_SYM_FMTCR_ZU(id) STR_SYM(id) "="BHBLU "%zu" CRESET "\n", id
#define STR_SYM_FMTCR_LU(id) STR_SYM(id) "="BHBLU "%lu" CRESET "\n", id

/* for new lines in log function calls */
#define LOG_NL "\n"YEL">"CRESET"\t"

#define FATAL 1
#define NON_FATAL !FATAL

#define NULL_TERM_SIZE sizeof(char)

void log_impl(const char *tag, int lf, const char *fmt, ...) CMPL_PRINTF_FUNCTION(3, 4);
#define USAGE(...) log_impl(BHCYN "USAGE" CRESET, 1, __VA_ARGS__)
#define INFO(...) log_impl(BHCYN "INFO" CRESET, 0, __VA_ARGS__)
#define TODO(str) log_impl(BHYEL "TODO" CRESET, 1, "function: '%s' @ %s:%d: %s", __func__, __FILE__, __LINE__, str)
#define WARN(...) log_impl(BHYEL "WARN" CRESET, 1, __VA_ARGS__)
#define ERRO(...) log_impl(BHRED "ERRO" CRESET, 1, __VA_ARGS__)
#define IMPL() log_impl(BHMAG "IMPL" CRESET, 1, "function: '%s' needs implementation @ %s:%d", __func__, __FILE__, __LINE__)

#define assert_f(cnd, fatal, ...)                           \
    do {                                                    \
        if (!(cnd)) {                                       \
            log_impl(BHRED "ASRT" CRESET, 0,                \
                    "assertion failed: '"#cnd"' => ");      \
            fprintf(stderr, "\"");                          \
            fprintf(stderr, __VA_ARGS__);                   \
            fprintf(stderr, "\"");                          \
            fprintf(stderr, "\n");                          \
            if (fatal)                                      \
                exit(1);                                    \
        }                                                   \
    } while (0)

#define const_strlen(s) (sizeof(s) - NULL_TERM_SIZE)

ssize_t get_file_size(ssize_t fd);

#endif /* CMPL_UTIL_H */
