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

#include "ISO_BMFF.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <endian.h>

#include "../util.h"

/* static function declarations start */
static struct isobmff_media *new_isobmff_media();
static struct isobmff_box_list *new_isobmff_box();
static uint32_t *get_brand_array(struct isobmff_box_list *ftyp_box);
static void delete_isobmff_box_list(struct isobmff_box_list *head);
static void delete_isobmff_box(struct isobmff_box_list *element);
static size_t read_isobmff_box(struct isobmff_box_list *box, uint32_t *buffer);
static char *boxtype2cstr(uint32_t box, char buff[5]);
static int is_valid_box(struct isobmff_box_list *box);
/* static function declarations end */

/* indeces for the first entries in the 'ftyp' brand array */
enum {
    MAJOR_BRAND = 0,
    MINOR_VERSION,
    COMPAT_BRANDS,
};

ssize_t load_isobmff_media(Media_Info **info, void *raw_buffer, ssize_t media_size)
{
    /* set the global contexts */
    play_media = play_isobmff_media;
    delete_media = delete_isobmff_media;

    struct isobmff_media *isobmff_info = new_isobmff_media();
    struct isobmff_box_list *media_box_list = new_isobmff_box();
    struct isobmff_box_list *current_box = media_box_list;
    void *base_addr = raw_buffer;
    uint64_t off = 0;

    while (READ_NEXT_ISOBMFF_BOX == read_isobmff_box(current_box, base_addr + off)) {
        off += current_box->header.size;
        INFO(STR_SYM_FMTCR_S(boxtype2cstr(current_box->header.type, (char [5]) {0})));
        assert_f(off <= media_size, FATAL,
                "Media file is corrupted!!! More info:"
                LOG_NL"ISO-base-media-file-format box '%s' is pointing out of bounds:"
                LOG_NL"(file size=%zu)"
                LOG_NL"(expected next box at offset %zu)",
                boxtype2cstr(current_box->header.type, (char [5]) {0}), media_size, off);
        current_box = current_box->next;
    }

    /* the first entry is special as older mp4 standards don't come with
     * an 'ftyp' entry.
     * */
    struct isobmff_box_list *ftyp_entry = media_box_list;
    struct isobmff_box_list *remaining_boxes = ftyp_entry;

    if (ftyp_entry->header.type == BOX_TYPE_FTYP) {
        isobmff_info->media.brands = get_brand_array(ftyp_entry);
    } else {
        isobmff_info->media.brands = malloc(2 * sizeof(uint32_t));
        isobmff_info->media.brands[MAJOR_BRAND] = BRAND_MP41;
        isobmff_info->media.brands[MINOR_VERSION] = 0;
    }

    for (EACH_ISOBMFF_BOX(remaining_boxes, box)) {
        switch (box->header.type) {
            case BOX_TYPE_MDAT: {
            } break;
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

    isobmff_info->media.boxes = media_box_list;
    *info = isobmff_info;

    return SUCCESS;
}

ssize_t delete_isobmff_media(Media_Info *info)
{
    struct isobmff_media *isobmff_info = info;

    if (isobmff_info->media.boxes)
        delete_isobmff_box_list(isobmff_info->media.boxes);


    free(isobmff_info);

    return SUCCESS;
}

ssize_t play_isobmff_media(Media_Info *info)
{
    IMPL();

    return SUCCESS;
}

ssize_t stop_isobmff_media(Media_Info *info)
{
    IMPL();

    return SUCCESS;
}

static struct isobmff_media *new_isobmff_media()
{
    struct isobmff_media *isobmff_media = malloc(sizeof(struct isobmff_media));
    explicit_bzero(isobmff_media, sizeof(struct isobmff_media));
    return isobmff_media;
}

static uint32_t *get_brand_array(struct isobmff_box_list *ftyp_box)
{
    uint32_t brand_array_size = ftyp_box->header.size - ISOBMFF_HDR_SIZE;
    uint32_t *brand_array = malloc(brand_array_size);
    memcpy(brand_array, ftyp_box->address + ISOBMFF_HDR_SIZE, brand_array_size);
    return brand_array;
}

#define ISOBMFF_BOX_TYPE_OFFSET 1

static size_t read_isobmff_box(struct isobmff_box_list *box, uint32_t *buffer)
{
    box->header.size = be32toh(*buffer);
    box->header.type  = *(buffer + ISOBMFF_BOX_TYPE_OFFSET);

    INFO(STR_SYM_FMTCR_X(box->header.type));
    INFO(STR_SYM_FMTCR_X(box->header.size));
    assert_f(is_valid_box(box), FATAL, "Media File is  Corrupted!!! More Info:"
            LOG_NL"invalid box (%"PRIx32")", box->header.type);

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

static char *boxtype2cstr(uint32_t box, char buff[5])
{
    memcpy(buff, &box, 4);
    buff[4] = '\0';
    return buff;
}

#define BOX_TYPE_ENDB 0x00000000U

static int is_valid_box(struct isobmff_box_list *box)
{
    switch (box->header.type) {
        case BOX_TYPE_FTYP:
        case BOX_TYPE_MDAT:
        case BOX_TYPE_MOOV:
        case BOX_TYPE_MOOF:
        case BOX_TYPE_PNOT:
        case BOX_TYPE_FREE:
        case BOX_TYPE_ENDB:
            return TRUE;
    }

    return FALSE;
}



