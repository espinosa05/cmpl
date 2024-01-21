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

#include "wav.h"

#include <string.h>

struct wav_media_info {
    char buffer[0x20];
};

/* static function declarations start */
static struct wav_media_info *new_wav_media_info();
/* static function declarations end */

ssize_t load_wav_media(Media_Info **info, void *buffer, ssize_t size)
{
    /* set global contexts */
    play_media = play_wav_media;
    delete_media = delete_wav_media;

    struct wav_media_info *wav_info = new_wav_media_info();
    CMPL_UNUSED_SYMBOL(wav_info);

    IMPL();

    *info = wav_info;

    return SUCCESS;
}

ssize_t play_wav_media(Media_Info *info)
{
    struct wav_media_info *wav_info = info;
    CMPL_UNUSED_SYMBOL(wav_info);
    IMPL();

    return SUCCESS;
}

ssize_t delete_wav_media(Media_Info *info)
{
    struct wav_media_info *wav_info = info;
    CMPL_UNUSED_SYMBOL(wav_info);
    IMPL();

    return SUCCESS;
}

static struct wav_media_info *new_wav_media_info()
{
    struct wav_media_info *wav_info = malloc(sizeof(struct wav_media_info));
    explicit_bzero(wav_info, sizeof(struct wav_media_info));

    return wav_info;
}
