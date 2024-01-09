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
        uint32_t *brands;
        uint32_t codec_id;
    } media;
};

struct media_player {
    // TODO implement media player
};

enum isobmff_box_types {
    BOX_TYPE_FTYP = 0x70797466U,
    BOX_TYPE_MDAT = 0x7461646dU,
    BOX_TYPE_MOOV = 0x766f6f6dU,
    BOX_TYPE_MOOF = 0x666f6f6dU,
    BOX_TYPE_PNOT = 0x746f6e70U,
    BOX_TYPE_FREE = 0x65657266U,
};

enum isobmff_ftyp_major_brands {
    BRAND_ISOM = 0x6d6f7369U,
    BRAND_MP42 = 0x3234706dU,
    BRAND_MP41 = 0x3134706dU,
    BRAND_3GP4 = 0x34706733U,
    BRAND_3GP5 = 0x35606733U,
};

#define LAST_ISOBMFF_BOX 1
#define READ_NEXT_ISOBMFF_BOX 0
#define EACH_ISOBMFF_BOX(list, element) struct isobmff_box_list *element = list; element != NULL; element = element->next

/* static declarations start */
static int load_media_file(Media_Data data, char *file_path);
static struct isobmff_box_list *new_isobmff_box();
static uint32_t *get_brand_array(struct isobmff_box_list *ftyp_box);
static void delete_isobmff_box_list(struct isobmff_box_list *head);
static void delete_isobmff_box(struct isobmff_box_list *element);
static size_t read_isobmff_box(struct isobmff_box_list *box, uint32_t *buffer);
static char *boxtype2cstr(uint32_t ftyp, char buff[5]);
static void read_meta_data(Media_Data data, struct isobmff_box_list *box);
static int is_valid_box(struct isobmff_box_list *box);

static void dump_box_contents(struct isobmff_box_list *box);
/* static declarations end */

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

    if (data->media.brands)
        free(data->media.brands);

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

    struct isobmff_box_list *current_box = data->media.boxes;
    void *box_address = data->raw.buffer;

    while (LAST_ISOBMFF_BOX !=  read_isobmff_box(current_box, box_address)) {
        box_address += current_box->header.size;
        current_box = current_box->next;
    }

    struct isobmff_box_list *ftyp_box = data->media.boxes;

    /* the ftyp box should *ALWAYS* be first */
    if (ftyp_box->header.type == BOX_TYPE_FTYP) {
        data->media.brands = get_brand_array(ftyp_box);
    } else {
        /* or else we assume it's an mp41 compatible file */
        data->media.brands = malloc(2 * sizeof(uint32_t));
        data->media.brands[MAJOR_BRAND] = BRAND_MP41;
        data->media.brands[MINOR_VERSION] = 0;
    }

    /* now skip the first box */
    struct isobmff_box_list *remaining_boxes = data->media.boxes->next;

    for (EACH_ISOBMFF_BOX(remaining_boxes, box)) {
        {
            char box_str[5] = { 0 };
            printf("box: %s\n", boxtype2cstr(box->header.type, box_str));
        }
        switch (box->header.type) {
            case BOX_TYPE_MOOV: {
            } break;
            case BOX_TYPE_MOOF: {
            } break;
            case BOX_TYPE_PNOT: {
            } break;
            case BOX_TYPE_FREE: {
            } break;
        }
    }
}

void play_media_file(Media_Player player)
{
    IMPL();
}

static uint32_t *get_brand_array(struct isobmff_box_list *ftyp_box)
{
    uint32_t *brand_array = malloc(ftyp_box->header.size);
    memcpy(brand_array, ftyp_box->address + sizeof(uint64_t), ftyp_box->header.size);
    return brand_array;
}

#define ISOBMFF_BOX_TYPE_OFFSET 1

static size_t read_isobmff_box(struct isobmff_box_list *box, uint32_t *buffer)
{
    box->header.size = be32toh(*buffer);
    box->header.type  = *(buffer + ISOBMFF_BOX_TYPE_OFFSET);

    INFO(STR_SYM_FMT_X(box->header.type));
    INFO(STR_SYM_FMT_X(box->header.size));

    if (!is_valid_box(box)) {
        ERRO("corrupted media file!");
        exit(EXIT_FAILURE);
    }

    if (box->header.size == 0 && box->header.type == 0) {
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
    delete_isobmff_box(head);

    delete_isobmff_box_list(next);
}

static int load_media_file(Media_Data data, char *file_path)
{
    ssize_t fd = open(file_path, O_RDONLY);
    if (fd < 0) {
        ERRO("failed to open mediafile: %s: %s", file_path, strerror(errno));
        return FALSE;
    }
    data->raw.size = get_file_size(fd);
    data->raw.buffer = malloc(data->raw.size);
    read(fd, data->raw.buffer, data->raw.size);
    close(fd);
    return TRUE;
}

static char *boxtype2cstr(uint32_t ftyp, char buff[5])
{
    memcpy(buff, &ftyp, 4);
    buff[4] = '\0';
    return buff;
}

static void read_meta_data(Media_Data data, struct isobmff_box_list *box)
{
    IMPL();
}

#define BOX_TYPE_ENDB 0x00000000U

static int is_valid_box(struct isobmff_box_list *box)
{
    TODO("Fix this abomination");
    /* i *REALLY* don't like this */
    if (BOX_TYPE_FTYP == box->header.type
        || BOX_TYPE_MDAT == box->header.type
        || BOX_TYPE_MOOV == box->header.type
        || BOX_TYPE_MOOF == box->header.type
        || BOX_TYPE_PNOT == box->header.type
        || BOX_TYPE_FREE == box->header.type
        || BOX_TYPE_ENDB == box->header.type)
        return TRUE;

    return FALSE;
}

static void dump_box_contents(struct isobmff_box_list *box)
{
    uint32_t *chunk_base = box->address;
    for (size_t i = 0; i < box->header.size; i += sizeof(uint32_t)) {
        uint32_t chunk = *(chunk_base++);
        printf("%");

        if (8%(i))
            fprintf(stderr, "\n");
        }
}
