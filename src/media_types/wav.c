#include "generic.h"
#include "wav.h"

#include "../util.h"

#include <string.h>

struct wav_media_info {
};

/* static function declarations start */
static struct wav_media_info *new_wav_media_info();
static void delete_wav_media_info();
/* static function declarations end */

ssize_t load_wav_media(Media_Info **info)
{
    struct wav_media_info *wav_info = new_wav_media_info();
    IMPL();
}

ssize_t play_wav_media(Media_Info *info)
{
    struct wav_media_info *wav_info = info;
    IMPL();
}

static void delete_wav_media_info(Media_Info *info)
{
    struct wav_media_info *wav_info = info;
    IMPL();
}

static struct wav_media_info *new_wav_media_info()
{
    struct wav_media_info *wav_info = malloc(sizeof(struct wav_media_info));
    explicit_bzero(wav_info, sizeof(struct wav_media_info));

    return wav_info;
}
