#ifndef TEMCO_PRODUCT_DESC_H
#define TEMCO_PRODUCT_DESC_H

#include <stddef.h>
#include <stdint.h>

/*
 * Shared layout for Temco product identity in a binary image: vendor id (vid),
 * product id (pid), and fixed-size trailing fields. On controllers this blob is
 * placed in read-only data; PC tools locate it in a .bin by matching vid "Temco"
 * and applying TEMCO_PRODUCT_DESC_VID_OFFSET. See the consumer pro_info object.
 */

#define TEMCO_PRODUCT_DESC_VID_OFFSET ((size_t)224)
#define TEMCO_PRODUCT_DESC_SIZE ((size_t)244)

typedef struct {
    uint8_t move_offset_to_512[224];
    char vid[5];
    char pid[10];
    uint8_t ver[2];
    uint8_t reserved[3];
} temco_product_desc_t;

_Static_assert(sizeof(temco_product_desc_t) == TEMCO_PRODUCT_DESC_SIZE, "temco_product_desc_t size");
_Static_assert(offsetof(temco_product_desc_t, vid) == TEMCO_PRODUCT_DESC_VID_OFFSET, "vid offset");

static inline uint16_t temco_product_desc_softrev(const temco_product_desc_t *d)
{
    /* Opaque packing of ver[0]:ver[1]; meaning is defined by the consumer. */
    return (uint16_t)(((uint16_t)d->ver[0] << 8) | (uint16_t)d->ver[1]);
}

#endif /* TEMCO_PRODUCT_DESC_H */
