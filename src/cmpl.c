#include "cmpl.h"
#include "util.h"

#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

struct isobmff_box_list {
    struct {
        uint32_t size;
        uint32_t type;
    } header;
    void *address;
    struct isobmff_box_list *next;
};

typedef struct {
    void *buffer;
} ISOBMFF_Box;

typedef struct {
    ISOBMFF_Box box;
} Ftyp_Box;

typedef struct {
    ISOBMFF_Box box;
} Mdat_Box;

typedef struct {
    ISOBMFF_Box box;
} Moov_Box;

typedef struct {
    ISOBMFF_Box box;
} Moof_Box;

struct media_data {
    struct {
        ssize_t size;
        void *buffer;
    } raw;
    struct {
        struct isobmff_box_list *boxes;
        uint32_t codec_id;
        size_t type;

    } media;
};

struct media_player {
};

enum isobmff_box_types {
    BOX_TYPE_FTYP = 0x70797466,
    BOX_TYPE_MDAT = 0x7461646d,
    BOX_TYPE_MOOV = 0x766f6f6d,
    BOX_TYPE_MOOF = 0x666f6f6d,
    BOX_TYPE_PNOT = 0x746f6e70,
    BOX_TYPE_FREE = 0x65657266,
};

#define LAST_ISOBMFF_BOX 1
#define READ_NEXT_ISOBMFF_BOX 0

#define EACH_ISOBMFF_BOX(list, element) struct isobmff_box_list *element = list; list != NULL; list = list->next

static void load_media_file(Media_Data data, ssize_t file);
static struct isobmff_box_list *new_isobmff_box();
static size_t get_media_type(struct isobmff_box_list *ftyp_box);
static void delete_isobmff_box_list(struct isobmff_box_list *head);
static void delete_isobmff_box(struct isobmff_box_list *element);
static size_t read_isobmff_box(struct isobmff_box_list *box, uint32_t *buffer);

Media_Data new_media_data()
{
    struct media_data *data = malloc(sizeof(struct media_data));
    explicit_bzero(data, sizeof(*data));

    data->media.boxes = new_isobmff_box();
    return data;
}

void delete_media_data(Media_Data data)
{
    if (data->raw.buffer)
        free(data->raw.buffer);

    if (data->media.boxes)
        delete_isobmff_box_list(data->media.boxes);

    free(data);
}

Media_Player new_media_player()
{
    struct media_player *player = malloc(sizeof(struct media_player));
    explicit_bzero(player, sizeof(*player));
    return player;
}

void delete_media_player(Media_Player player)
{
    free(player);
}

void parse_media_file(Media_Data data, char *file_path)
{
    ssize_t file = open(file_path, O_RDONLY);

    if (file < 0) {
        fprintf(stderr, "ERROR: failed to open media file: %s\n", strerror(errno));
        delete_media_data(data);
        exit(EXIT_FAILURE);
    }

    load_media_file(data, file);
    close(file);

    struct isobmff_box_list *current_box = data->media.boxes;
    void *box_address = data->raw.buffer;

    while (LAST_ISOBMFF_BOX !=  read_isobmff_box(current_box, box_address)) {
        TODO("write the handlers");
        switch(current_box->header.type)
        {
            case BOX_TYPE_FTYP:
                break;
            case BOX_TYPE_MDAT:
                break;
            case BOX_TYPE_MOOV:
                break;
            case BOX_TYPE_MOOF:
                break;
            case BOX_TYPE_FREE:
                break;
            case BOX_TYPE_PNOT:
                break;
        }

        box_address += current_box->header.size;
        current_box = current_box->next;
    }

    struct isobmff_box_list *ftyp_box = data->media.boxes;
    struct isobmff_box_list *box_list = ftyp_box->next;

    data->media.type = get_media_type(ftyp_box);

    for (EACH_ISOBMFF_BOX(box_list, box)) {
    }
}

void play_media_file(Media_Player player, Media_Data data)
{
    IMPL();
}

static size_t get_media_type(struct isobmff_box_list *ftyp_box)
{
}

#define ISOBMFF_BOX_TYPE_OFFSET 1

static size_t read_isobmff_box(struct isobmff_box_list *box, uint32_t *buffer)
{
    box->header.size = be32toh(*buffer);
    box->header.type  = *(buffer + ISOBMFF_BOX_TYPE_OFFSET);

    INFO(STR_SYM_FMT_X(box->header.type));
    INFO(STR_SYM_FMT_X(box->header.size));

    if (box->header.size == 0) {
        box->next = NULL;
        return LAST_ISOBMFF_BOX;
    }

    box->address = buffer; /* save the address for later */
    box->next = new_isobmff_box();
    return READ_NEXT_ISOBMFF_BOX;
}

static struct isobmff_box_list *new_isobmff_box()
{
    struct isobmff_box_list *element = malloc(sizeof(struct isobmff_box_list));
    explicit_bzero(element, sizeof(*element));
    return element;
}

static void delete_isobmff_box(struct isobmff_box_list *element)
{
    free(element);
}

static void delete_isobmff_box_list(struct isobmff_box_list *head)
{
    if (!head)
        return;

    struct isobmff_box_list *next = head->next;
    free(head);

    delete_isobmff_box_list(next);
}

static void load_media_file(Media_Data data, ssize_t file)
{
    data->raw.size = get_file_size(file);
    data->raw.buffer = malloc(data->raw.size);
    read(file, data->raw.buffer, data->raw.size);
}
