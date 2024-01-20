#include "cmpl.h"
#include "util.h"

#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <endian.h>
#include <errno.h>
#include <fcntl.h>

#include "media_types/common.h"

struct media_data {
    struct {
        ssize_t size;
        void *buffer;
    } raw;
    struct {
        size_t ext;
        size_t type;
        Media_Info *info; /* generic pointer into the different APIs */
    } media;
};

struct media_player {
    // TODO implement media player
};
/* static declarations start */
static int load_media_file(Media_Data data, char *file_path);
/* different media type loading */
static size_t get_media_type(Media_Data data);
static size_t get_media_file_ext(char *path);
/* static declarations end */


Media_Data new_media_data()
{
    struct media_data *data = malloc(sizeof(struct media_data));
    explicit_bzero(data, sizeof(*data));

    return data;
}

void delete_media_data(Media_Data data)
{
    if (data->raw.buffer)
        free(data->raw.buffer);

    free(data);
}

Media_Player new_media_player(Media_Data data)
{
    struct media_player *player = malloc(sizeof(struct media_player));
    explicit_bzero(player, sizeof(*player));


    TODO("setup video player structure");

    return player;
}

void delete_media_player(Media_Player player)
{
    free(player);
}

enum {
    MAJOR_BRAND = 0,
    MINOR_VERSION,
    COMPAT_BRANDS,
};

void parse_media_file(Media_Data data, char *file_path)
{
    if (!load_media_file(data, file_path)) {
        delete_media_data(data);
        exit(EXIT_FAILURE);
    }

    data->media.ext = get_media_file_ext(file_path); /* read file extension */
    /* override value if the file contents differ from the extension */
    data->media.type = get_media_type(data);

    switch (data->media.type) {
        case MEDIA_STANDARD_ISOBMFF: {
            load_isobmff_media(&data->media.info,
                    data->raw.buffer,
                    data->raw.size); /* load subtype for ISOBMFF */
        } break;
        case MEDIA_STANDARD_MP3: {
            load_mp3_media(&data->media.info);
        } break;
        case MEDIA_STANDARD_WAV: {
            load_wav_media(&data->media.info);
        } break;
        case EMPTY_MEDIA_TYPE:
            ERRO("file '%s' is of unknown file format", file_path);
            exit(1);
    }
}

void play_media_file(Media_Player player)
{
    IMPL();
}

int load_media_file(Media_Data data, char *file_path)
{
    ssize_t fd = open(file_path, O_RDONLY);
    assert_f(fd > 0, FATAL, "failed to open media file %s", strerror(errno));

    data->raw.size = get_file_size(fd);
    data->raw.buffer = calloc(1, data->raw.size);
    read(fd, data->raw.buffer, data->raw.size);
    close(fd);
    return TRUE;
}

/* get to the last 'dot' for the file extension */
static size_t get_media_file_ext(char *path)
{
    char *path_tok_ptr;
    char *path_copy = strdup(path);
    char *ext = NULL;
    char *tok = strtok_r(path_copy, ".", &path_tok_ptr);

    while (NULL != tok) {
        ext = tok;
        tok = strtok_r(NULL, ".", &path_tok_ptr);
    }

    free(path_copy);

    if (strcmp("mp3", ext) == 0)
        return MEDIA_STANDARD_MP3;

    if (strcmp("wav", ext) == 0)
        return MEDIA_STANDARD_WAV;

    if (strcmp("mp4", ext) == 0
            || strcmp("mkv", ext) == 0)
        return MEDIA_STANDARD_ISOBMFF;

    return EMPTY_MEDIA_TYPE;
}

#define FTYP_OFFSET sizeof(uint32_t)

static size_t get_media_type(Media_Data data)
{
    if (0 == memcmp(data->raw.buffer + FTYP_OFFSET, "ftyp", const_strlen("ftyp")))
        return MEDIA_STANDARD_ISOBMFF;

    if (0 == memcmp(data->raw.buffer, "ID3", const_strlen("ID3")))
        return MEDIA_STANDARD_MP3;

    if (0 == memcmp(data->raw.buffer, "RIFF", const_strlen("RIFF")))
        return MEDIA_STANDARD_WAV;
    /* if the previous call to 'get_media_file_ext' returned non empty we just use the file extension
     * and rely on its implications (old mp3 files don't have any metadata and just come with raw frames for example)
     * */
    return data->media.ext ? data->media.ext : EMPTY_MEDIA_TYPE;
}

