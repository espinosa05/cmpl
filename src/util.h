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

#define _ISO_C99_SOURCE /* feature macro for PRIx... marcos */
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>

#define TRUE 1
#define FALSE 0
#define SUCCESS TRUE
#define FAILURE FALSE

/* stolen from the public domain >:) */
#include "ANSI-color-codes.h"

/* attribute macros */
#define __CMPL_ATTR_UNUSED__ __attribute__((unused))
#define __CMPL_ATTR_DEPRECATED__ __attribute__((deprecated))
#define __CMPL_ATTR_ALWAYS_INLINE__ __attribute__((always_inline))

#define CMPL_UNUSED_SYMBOL(s) (void)s
#define CMPL_PRINTF_FUNCTION(fmt_start, arg_start) __attribute__((format (printf, fmt_start, arg_start)))
#define STR_SYM(id) #id

#define STR_SYM_FMT_X8(id) STR_SYM(id) "="BHBLU "%#018"PRIx8 CRESET, id
#define STR_SYM_FMT_X16(id) STR_SYM(id) "="BHBLU "%#018"PRIx16 CRESET, id
#define STR_SYM_FMT_X32(id) STR_SYM(id) "="BHBLU "%#018"PRIx32 CRESET, id
#define STR_SYM_FMT_X64(id) STR_SYM(id) "="BHBLU "%#018"PRIx64 CRESET, id

#define STR_SYM_FMT_S(id) STR_SYM(id) "="BHGRN "%s" CRESET, id
#define STR_SYM_FMT_D(id) STR_SYM(id) "="BHBLU "%d" CRESET, id
#define STR_SYM_FMT_P(id) STR_SYM(id) "="BHMAG "%p" CRESET, id
#define STR_SYM_FMT_F(id) STR_SYM(id) "="BHBLU "%f" CRESET, id
#define STR_SYM_FMT_ZU(id) STR_SYM(id) "="BHBLU "%zu" CRESET, id
#define STR_SYM_FMT_LU(id) STR_SYM(id) "="BHBLU "%lu" CRESET, id

/* default for 32-bit unsigned integers */
#define STR_SYM_FMT_X(id) STR_SYM_FMT_X32(id)

/* Versions with carriage returns */
#define STR_SYM_FMTCR_X8(id) STR_SYM(id) "="BHBLU "%#018"PRIx8 CRESET "\n", id
#define STR_SYM_FMTCR_X16(id) STR_SYM(id) "="BHBLU "%#018"PRIx16 CRESET "\n", id
#define STR_SYM_FMTCR_X32(id) STR_SYM(id) "="BHBLU "%#018"PRIx32 CRESET "\n", id
#define STR_SYM_FMTCR_X64(id) STR_SYM(id) "="BHBLU "%#018"PRIx64 CRESET "\n", id

/* default for 32-bit unsigned integers */
#define STR_SYM_FMTCR_X(id) STR_SYM_FMTCR_X32(id)

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

#ifdef _DEBUG
#   define log_msg(...) log_impl(__VA_ARGS__)
#else
/* disable all kind of logging in production mode */
#   define log_msg(...) do { } while (0)
#endif /* _DEBUG*/

#define USAGE(...) log_msg(BHCYN "USAGE" CRESET, 1, __VA_ARGS__)
#define INFO(...) log_msg(BHCYN "INFO" CRESET, 0, __VA_ARGS__)
#define TODO(str) log_msg(BHYEL "TODO" CRESET, 1, "function: '%s' @ %s:%d: %s", __func__, __FILE__, __LINE__, str)
#define WARN(...) log_msg(BHYEL "WARN" CRESET, 1, __VA_ARGS__)
#define ERRO(...) log_msg(BHRED "ERRO" CRESET, 1, __VA_ARGS__)
#define IMPL() log_msg(BHMAG "IMPL" CRESET, 1, "function: '%s' needs implementation @ %s:%d", __func__, __FILE__, __LINE__)

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
#define sizeof_array(a) (sizeof(a) / sizeof(a[0]))

ssize_t get_file_size(ssize_t fd);

#endif /* CMPL_UTIL_H */
