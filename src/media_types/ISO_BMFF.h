#ifndef MEDIA_TYPES_ISO_BMFF_H
#define MEDIA_TYPES_ISO_BMFF_H

#include "generic.h"

#include <stdlib.h>
#include <stdint.h>

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


typedef struct {
    ISOBMFF_Box box;
} Pnot_Box;

typedef struct {
    ISOBMFF_Box box;
} Free_Box;

#define ISOBMFF_HDR_SIZE sizeof(uint64_t)

struct isobmff_media {
    struct {
        struct isobmff_box_list *boxes;
        uint32_t *brands;
        uint32_t codec_id;
    } media;
    Ftyp_Box ftyp;
    Mdat_Box mdat;
    Moov_Box moov;
    Moof_Box moof;
    Pnot_Box pnot;
    Free_Box free;
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
ssize_t load_isobmff_media(Media_Info **info, void *raw_buffer, ssize_t media_size);

ssize_t play_isobmff_media(Media_Info *info);
ssize_t stop_isobmff_media(Media_Info *info);
ssize_t delete_isobmff_media(Media_Info *info);

#endif /* MEDIA_TYPES_ISO_BMFF_H*/
