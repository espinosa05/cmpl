#ifndef MEDIA_TYPES_MP3_H
#define MEDIA_TYPES_MP3_H

#include "generic.h"

#include "../util.h"

void load_mp3_media(Media_Info **info);

ssize_t play_mp3_media(Media_Info *info);
ssize_t stop_mp3_media(Media_Info *info);
ssize_t delete_mp3_media(Media_Info *info);

#endif /* MEDIA_TYPES_MP3_H */
