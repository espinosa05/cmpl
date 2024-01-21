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

#ifndef CMPL_H
#define CMPL_H

enum media_types {
    EMPTY_MEDIA_TYPE = 0,
    MEDIA_STANDARD_ISOBMFF,
/* supported ISOBMFF sub types:
*   mp4, mkv
* */
    MEDIA_STANDARD_MP3,
    MEDIA_STANDARD_WAV,
};

typedef struct media_data* Media_Data;
typedef struct media_player* Media_Player;

Media_Data new_media_data();
Media_Player new_media_player();

void delete_media_data(Media_Data data);
void delete_media_player(Media_Player player);

void parse_media_file(Media_Data data, char *file_path);
void play_media_file(Media_Player player, Media_Data data);

#endif /* CMPL_H */
