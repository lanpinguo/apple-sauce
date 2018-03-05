/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"


void
of_instruction_bsn_wire_object_id_get(of_object_t *obj, of_object_id_t *id)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 8)); /* subtype */
        switch (value) {
        case 0x0:
            *id = OF_INSTRUCTION_BSN_DISABLE_SRC_MAC_CHECK;
            break;
        case 0x1:
            *id = OF_INSTRUCTION_BSN_ARP_OFFLOAD;
            break;
        case 0x2:
            *id = OF_INSTRUCTION_BSN_DHCP_OFFLOAD;
            break;
        case 0x3:
            *id = OF_INSTRUCTION_BSN_DISABLE_SPLIT_HORIZON_CHECK;
            break;
        case 0x4:
            *id = OF_INSTRUCTION_BSN_PERMIT;
            break;
        case 0x5:
            *id = OF_INSTRUCTION_BSN_DENY;
            break;
        case 0x6:
            *id = OF_INSTRUCTION_BSN_PACKET_OF_DEATH;
            break;
        default:
            *id = OF_INSTRUCTION_BSN;
            break;
        }
        break;
    }
    default:
        LOCI_ASSERT(0);
    }
}


/**
 * \defgroup of_instruction_bsn of_instruction_bsn
 */

/**
 * Create a new of_instruction_bsn object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_bsn
 */

of_instruction_bsn_t *
of_instruction_bsn_new(of_version_t version)
{
    of_instruction_bsn_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_BSN] + of_object_extra_len[version][OF_INSTRUCTION_BSN];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_bsn_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_bsn_init(obj, version, bytes, 0);

    return obj;
}

/**
 * Initialize an object of type of_instruction_bsn.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_bsn_init(of_instruction_bsn_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_BSN] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_BSN] + of_object_extra_len[version][OF_INSTRUCTION_BSN];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_BSN;

    /* Set up the object's function pointers */

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_instruction_bsn.
 * @param obj Pointer to an object of type of_instruction_bsn.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_bsn_experimenter_get(
    of_instruction_bsn_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_instruction_bsn.
 * @param obj Pointer to an object of type of_instruction_bsn.
 * @param experimenter The value to write into the object
 */
void
of_instruction_bsn_experimenter_set(
    of_instruction_bsn_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_instruction_bsn.
 * @param obj Pointer to an object of type of_instruction_bsn.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_bsn_subtype_get(
    of_instruction_bsn_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_instruction_bsn.
 * @param obj Pointer to an object of type of_instruction_bsn.
 * @param subtype The value to write into the object
 */
void
of_instruction_bsn_subtype_set(
    of_instruction_bsn_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"


void
of_instruction_id_experimenter_wire_object_id_get(of_object_t *obj, of_object_id_t *id)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 4)); /* experimenter */
        switch (value) {
        case 0x5c16c7:
            of_instruction_id_bsn_wire_object_id_get(obj, id);
            break;
        default:
            *id = OF_INSTRUCTION_ID_EXPERIMENTER;
            break;
        }
        break;
    }
    default:
        LOCI_ASSERT(0);
    }
}


/**
 * \defgroup of_instruction_id_experimenter of_instruction_id_experimenter
 */

/**
 * Create a new of_instruction_id_experimenter object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_id_experimenter
 */

of_instruction_id_experimenter_t *
of_instruction_id_experimenter_new(of_version_t version)
{
    of_instruction_id_experimenter_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_EXPERIMENTER] + of_object_extra_len[version][OF_INSTRUCTION_ID_EXPERIMENTER];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_id_experimenter_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_id_experimenter_init(obj, version, bytes, 0);

    return obj;
}

/**
 * Initialize an object of type of_instruction_id_experimenter.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_id_experimenter_init(of_instruction_id_experimenter_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_ID_EXPERIMENTER] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_EXPERIMENTER] + of_object_extra_len[version][OF_INSTRUCTION_ID_EXPERIMENTER];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_ID_EXPERIMENTER;

    /* Set up the object's function pointers */

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_instruction_id_experimenter.
 * @param obj Pointer to an object of type of_instruction_id_experimenter.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_id_experimenter_experimenter_get(
    of_instruction_id_experimenter_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_EXPERIMENTER);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_instruction_id_experimenter.
 * @param obj Pointer to an object of type of_instruction_id_experimenter.
 * @param experimenter The value to write into the object
 */
void
of_instruction_id_experimenter_experimenter_set(
    of_instruction_id_experimenter_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_EXPERIMENTER);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"


void
of_instruction_id_bsn_wire_object_id_get(of_object_t *obj, of_object_id_t *id)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 8)); /* subtype */
        switch (value) {
        case 0x0:
            *id = OF_INSTRUCTION_ID_BSN_DISABLE_SRC_MAC_CHECK;
            break;
        case 0x1:
            *id = OF_INSTRUCTION_ID_BSN_ARP_OFFLOAD;
            break;
        case 0x2:
            *id = OF_INSTRUCTION_ID_BSN_DHCP_OFFLOAD;
            break;
        case 0x3:
            *id = OF_INSTRUCTION_ID_BSN_DISABLE_SPLIT_HORIZON_CHECK;
            break;
        case 0x4:
            *id = OF_INSTRUCTION_ID_BSN_PERMIT;
            break;
        case 0x5:
            *id = OF_INSTRUCTION_ID_BSN_DENY;
            break;
        case 0x6:
            *id = OF_INSTRUCTION_ID_BSN_PACKET_OF_DEATH;
            break;
        default:
            *id = OF_INSTRUCTION_ID_BSN;
            break;
        }
        break;
    }
    default:
        LOCI_ASSERT(0);
    }
}


/**
 * \defgroup of_instruction_id_bsn of_instruction_id_bsn
 */

/**
 * Create a new of_instruction_id_bsn object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_id_bsn
 */

of_instruction_id_bsn_t *
of_instruction_id_bsn_new(of_version_t version)
{
    of_instruction_id_bsn_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN] + of_object_extra_len[version][OF_INSTRUCTION_ID_BSN];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_id_bsn_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_id_bsn_init(obj, version, bytes, 0);

    return obj;
}

/**
 * Initialize an object of type of_instruction_id_bsn.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_id_bsn_init(of_instruction_id_bsn_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN] + of_object_extra_len[version][OF_INSTRUCTION_ID_BSN];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_ID_BSN;

    /* Set up the object's function pointers */

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_instruction_id_bsn.
 * @param obj Pointer to an object of type of_instruction_id_bsn.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_id_bsn_experimenter_get(
    of_instruction_id_bsn_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_instruction_id_bsn.
 * @param obj Pointer to an object of type of_instruction_id_bsn.
 * @param experimenter The value to write into the object
 */
void
of_instruction_id_bsn_experimenter_set(
    of_instruction_id_bsn_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_instruction_id_bsn.
 * @param obj Pointer to an object of type of_instruction_id_bsn.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_id_bsn_subtype_get(
    of_instruction_id_bsn_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_instruction_id_bsn.
 * @param obj Pointer to an object of type of_instruction_id_bsn.
 * @param subtype The value to write into the object
 */
void
of_instruction_id_bsn_subtype_set(
    of_instruction_id_bsn_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_instruction_bsn_arp_offload_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x5c16c7); /* experimenter */
        *(uint32_t *)(buf + 8) = U32_HTON(0x1); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_instruction_bsn_arp_offload of_instruction_bsn_arp_offload
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_instruction_bsn_arp_offload_push_wire_values(of_instruction_bsn_arp_offload_t *obj)
{

    of_instruction_bsn_arp_offload_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_instruction_bsn_arp_offload object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_bsn_arp_offload
 */

of_instruction_bsn_arp_offload_t *
of_instruction_bsn_arp_offload_new(of_version_t version)
{
    of_instruction_bsn_arp_offload_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_BSN_ARP_OFFLOAD] + of_object_extra_len[version][OF_INSTRUCTION_BSN_ARP_OFFLOAD];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_bsn_arp_offload_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_bsn_arp_offload_init(obj, version, bytes, 0);

    if (of_instruction_bsn_arp_offload_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_instruction_bsn_arp_offload.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_bsn_arp_offload_init(of_instruction_bsn_arp_offload_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_BSN_ARP_OFFLOAD] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_BSN_ARP_OFFLOAD] + of_object_extra_len[version][OF_INSTRUCTION_BSN_ARP_OFFLOAD];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_BSN_ARP_OFFLOAD;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_instruction_bsn_arp_offload_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_instruction_bsn_arp_offload.
 * @param obj Pointer to an object of type of_instruction_bsn_arp_offload.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_bsn_arp_offload_experimenter_get(
    of_instruction_bsn_arp_offload_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_ARP_OFFLOAD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_instruction_bsn_arp_offload.
 * @param obj Pointer to an object of type of_instruction_bsn_arp_offload.
 * @param experimenter The value to write into the object
 */
void
of_instruction_bsn_arp_offload_experimenter_set(
    of_instruction_bsn_arp_offload_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_ARP_OFFLOAD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_instruction_bsn_arp_offload.
 * @param obj Pointer to an object of type of_instruction_bsn_arp_offload.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_bsn_arp_offload_subtype_get(
    of_instruction_bsn_arp_offload_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_ARP_OFFLOAD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_instruction_bsn_arp_offload.
 * @param obj Pointer to an object of type of_instruction_bsn_arp_offload.
 * @param subtype The value to write into the object
 */
void
of_instruction_bsn_arp_offload_subtype_set(
    of_instruction_bsn_arp_offload_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_ARP_OFFLOAD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_instruction_id_bsn_arp_offload_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x5c16c7); /* experimenter */
        *(uint32_t *)(buf + 8) = U32_HTON(0x1); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_instruction_id_bsn_arp_offload of_instruction_id_bsn_arp_offload
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_instruction_id_bsn_arp_offload_push_wire_values(of_instruction_id_bsn_arp_offload_t *obj)
{

    of_instruction_id_bsn_arp_offload_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_instruction_id_bsn_arp_offload object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_id_bsn_arp_offload
 */

of_instruction_id_bsn_arp_offload_t *
of_instruction_id_bsn_arp_offload_new(of_version_t version)
{
    of_instruction_id_bsn_arp_offload_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN_ARP_OFFLOAD] + of_object_extra_len[version][OF_INSTRUCTION_ID_BSN_ARP_OFFLOAD];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_id_bsn_arp_offload_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_id_bsn_arp_offload_init(obj, version, bytes, 0);

    if (of_instruction_id_bsn_arp_offload_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_instruction_id_bsn_arp_offload.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_id_bsn_arp_offload_init(of_instruction_id_bsn_arp_offload_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN_ARP_OFFLOAD] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN_ARP_OFFLOAD] + of_object_extra_len[version][OF_INSTRUCTION_ID_BSN_ARP_OFFLOAD];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_ID_BSN_ARP_OFFLOAD;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_instruction_id_bsn_arp_offload_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_instruction_id_bsn_arp_offload.
 * @param obj Pointer to an object of type of_instruction_id_bsn_arp_offload.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_id_bsn_arp_offload_experimenter_get(
    of_instruction_id_bsn_arp_offload_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_ARP_OFFLOAD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_instruction_id_bsn_arp_offload.
 * @param obj Pointer to an object of type of_instruction_id_bsn_arp_offload.
 * @param experimenter The value to write into the object
 */
void
of_instruction_id_bsn_arp_offload_experimenter_set(
    of_instruction_id_bsn_arp_offload_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_ARP_OFFLOAD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_instruction_id_bsn_arp_offload.
 * @param obj Pointer to an object of type of_instruction_id_bsn_arp_offload.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_id_bsn_arp_offload_subtype_get(
    of_instruction_id_bsn_arp_offload_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_ARP_OFFLOAD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_instruction_id_bsn_arp_offload.
 * @param obj Pointer to an object of type of_instruction_id_bsn_arp_offload.
 * @param subtype The value to write into the object
 */
void
of_instruction_id_bsn_arp_offload_subtype_set(
    of_instruction_id_bsn_arp_offload_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_ARP_OFFLOAD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_instruction_bsn_deny_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x5c16c7); /* experimenter */
        *(uint32_t *)(buf + 8) = U32_HTON(0x5); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_instruction_bsn_deny of_instruction_bsn_deny
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_instruction_bsn_deny_push_wire_values(of_instruction_bsn_deny_t *obj)
{

    of_instruction_bsn_deny_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_instruction_bsn_deny object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_bsn_deny
 */

of_instruction_bsn_deny_t *
of_instruction_bsn_deny_new(of_version_t version)
{
    of_instruction_bsn_deny_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_BSN_DENY] + of_object_extra_len[version][OF_INSTRUCTION_BSN_DENY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_bsn_deny_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_bsn_deny_init(obj, version, bytes, 0);

    if (of_instruction_bsn_deny_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_instruction_bsn_deny.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_bsn_deny_init(of_instruction_bsn_deny_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_BSN_DENY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_BSN_DENY] + of_object_extra_len[version][OF_INSTRUCTION_BSN_DENY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_BSN_DENY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_instruction_bsn_deny_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_instruction_bsn_deny.
 * @param obj Pointer to an object of type of_instruction_bsn_deny.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_bsn_deny_experimenter_get(
    of_instruction_bsn_deny_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_DENY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_instruction_bsn_deny.
 * @param obj Pointer to an object of type of_instruction_bsn_deny.
 * @param experimenter The value to write into the object
 */
void
of_instruction_bsn_deny_experimenter_set(
    of_instruction_bsn_deny_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_DENY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_instruction_bsn_deny.
 * @param obj Pointer to an object of type of_instruction_bsn_deny.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_bsn_deny_subtype_get(
    of_instruction_bsn_deny_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_DENY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_instruction_bsn_deny.
 * @param obj Pointer to an object of type of_instruction_bsn_deny.
 * @param subtype The value to write into the object
 */
void
of_instruction_bsn_deny_subtype_set(
    of_instruction_bsn_deny_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_DENY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_instruction_id_bsn_deny_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x5c16c7); /* experimenter */
        *(uint32_t *)(buf + 8) = U32_HTON(0x5); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_instruction_id_bsn_deny of_instruction_id_bsn_deny
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_instruction_id_bsn_deny_push_wire_values(of_instruction_id_bsn_deny_t *obj)
{

    of_instruction_id_bsn_deny_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_instruction_id_bsn_deny object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_id_bsn_deny
 */

of_instruction_id_bsn_deny_t *
of_instruction_id_bsn_deny_new(of_version_t version)
{
    of_instruction_id_bsn_deny_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN_DENY] + of_object_extra_len[version][OF_INSTRUCTION_ID_BSN_DENY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_id_bsn_deny_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_id_bsn_deny_init(obj, version, bytes, 0);

    if (of_instruction_id_bsn_deny_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_instruction_id_bsn_deny.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_id_bsn_deny_init(of_instruction_id_bsn_deny_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN_DENY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN_DENY] + of_object_extra_len[version][OF_INSTRUCTION_ID_BSN_DENY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_ID_BSN_DENY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_instruction_id_bsn_deny_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_instruction_id_bsn_deny.
 * @param obj Pointer to an object of type of_instruction_id_bsn_deny.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_id_bsn_deny_experimenter_get(
    of_instruction_id_bsn_deny_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_DENY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_instruction_id_bsn_deny.
 * @param obj Pointer to an object of type of_instruction_id_bsn_deny.
 * @param experimenter The value to write into the object
 */
void
of_instruction_id_bsn_deny_experimenter_set(
    of_instruction_id_bsn_deny_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_DENY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_instruction_id_bsn_deny.
 * @param obj Pointer to an object of type of_instruction_id_bsn_deny.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_id_bsn_deny_subtype_get(
    of_instruction_id_bsn_deny_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_DENY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_instruction_id_bsn_deny.
 * @param obj Pointer to an object of type of_instruction_id_bsn_deny.
 * @param subtype The value to write into the object
 */
void
of_instruction_id_bsn_deny_subtype_set(
    of_instruction_id_bsn_deny_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_DENY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_instruction_bsn_dhcp_offload_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x5c16c7); /* experimenter */
        *(uint32_t *)(buf + 8) = U32_HTON(0x2); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_instruction_bsn_dhcp_offload of_instruction_bsn_dhcp_offload
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_instruction_bsn_dhcp_offload_push_wire_values(of_instruction_bsn_dhcp_offload_t *obj)
{

    of_instruction_bsn_dhcp_offload_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_instruction_bsn_dhcp_offload object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_bsn_dhcp_offload
 */

of_instruction_bsn_dhcp_offload_t *
of_instruction_bsn_dhcp_offload_new(of_version_t version)
{
    of_instruction_bsn_dhcp_offload_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_BSN_DHCP_OFFLOAD] + of_object_extra_len[version][OF_INSTRUCTION_BSN_DHCP_OFFLOAD];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_bsn_dhcp_offload_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_bsn_dhcp_offload_init(obj, version, bytes, 0);

    if (of_instruction_bsn_dhcp_offload_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_instruction_bsn_dhcp_offload.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_bsn_dhcp_offload_init(of_instruction_bsn_dhcp_offload_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_BSN_DHCP_OFFLOAD] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_BSN_DHCP_OFFLOAD] + of_object_extra_len[version][OF_INSTRUCTION_BSN_DHCP_OFFLOAD];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_BSN_DHCP_OFFLOAD;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_instruction_bsn_dhcp_offload_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_instruction_bsn_dhcp_offload.
 * @param obj Pointer to an object of type of_instruction_bsn_dhcp_offload.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_bsn_dhcp_offload_experimenter_get(
    of_instruction_bsn_dhcp_offload_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_DHCP_OFFLOAD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_instruction_bsn_dhcp_offload.
 * @param obj Pointer to an object of type of_instruction_bsn_dhcp_offload.
 * @param experimenter The value to write into the object
 */
void
of_instruction_bsn_dhcp_offload_experimenter_set(
    of_instruction_bsn_dhcp_offload_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_DHCP_OFFLOAD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_instruction_bsn_dhcp_offload.
 * @param obj Pointer to an object of type of_instruction_bsn_dhcp_offload.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_bsn_dhcp_offload_subtype_get(
    of_instruction_bsn_dhcp_offload_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_DHCP_OFFLOAD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_instruction_bsn_dhcp_offload.
 * @param obj Pointer to an object of type of_instruction_bsn_dhcp_offload.
 * @param subtype The value to write into the object
 */
void
of_instruction_bsn_dhcp_offload_subtype_set(
    of_instruction_bsn_dhcp_offload_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_DHCP_OFFLOAD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_instruction_id_bsn_dhcp_offload_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x5c16c7); /* experimenter */
        *(uint32_t *)(buf + 8) = U32_HTON(0x2); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_instruction_id_bsn_dhcp_offload of_instruction_id_bsn_dhcp_offload
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_instruction_id_bsn_dhcp_offload_push_wire_values(of_instruction_id_bsn_dhcp_offload_t *obj)
{

    of_instruction_id_bsn_dhcp_offload_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_instruction_id_bsn_dhcp_offload object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_id_bsn_dhcp_offload
 */

of_instruction_id_bsn_dhcp_offload_t *
of_instruction_id_bsn_dhcp_offload_new(of_version_t version)
{
    of_instruction_id_bsn_dhcp_offload_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN_DHCP_OFFLOAD] + of_object_extra_len[version][OF_INSTRUCTION_ID_BSN_DHCP_OFFLOAD];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_id_bsn_dhcp_offload_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_id_bsn_dhcp_offload_init(obj, version, bytes, 0);

    if (of_instruction_id_bsn_dhcp_offload_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_instruction_id_bsn_dhcp_offload.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_id_bsn_dhcp_offload_init(of_instruction_id_bsn_dhcp_offload_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN_DHCP_OFFLOAD] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN_DHCP_OFFLOAD] + of_object_extra_len[version][OF_INSTRUCTION_ID_BSN_DHCP_OFFLOAD];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_ID_BSN_DHCP_OFFLOAD;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_instruction_id_bsn_dhcp_offload_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_instruction_id_bsn_dhcp_offload.
 * @param obj Pointer to an object of type of_instruction_id_bsn_dhcp_offload.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_id_bsn_dhcp_offload_experimenter_get(
    of_instruction_id_bsn_dhcp_offload_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_DHCP_OFFLOAD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_instruction_id_bsn_dhcp_offload.
 * @param obj Pointer to an object of type of_instruction_id_bsn_dhcp_offload.
 * @param experimenter The value to write into the object
 */
void
of_instruction_id_bsn_dhcp_offload_experimenter_set(
    of_instruction_id_bsn_dhcp_offload_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_DHCP_OFFLOAD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_instruction_id_bsn_dhcp_offload.
 * @param obj Pointer to an object of type of_instruction_id_bsn_dhcp_offload.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_id_bsn_dhcp_offload_subtype_get(
    of_instruction_id_bsn_dhcp_offload_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_DHCP_OFFLOAD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_instruction_id_bsn_dhcp_offload.
 * @param obj Pointer to an object of type of_instruction_id_bsn_dhcp_offload.
 * @param subtype The value to write into the object
 */
void
of_instruction_id_bsn_dhcp_offload_subtype_set(
    of_instruction_id_bsn_dhcp_offload_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_DHCP_OFFLOAD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_instruction_bsn_disable_split_horizon_check_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x5c16c7); /* experimenter */
        *(uint32_t *)(buf + 8) = U32_HTON(0x3); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_instruction_bsn_disable_split_horizon_check of_instruction_bsn_disable_split_horizon_check
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_instruction_bsn_disable_split_horizon_check_push_wire_values(of_instruction_bsn_disable_split_horizon_check_t *obj)
{

    of_instruction_bsn_disable_split_horizon_check_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_instruction_bsn_disable_split_horizon_check object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_bsn_disable_split_horizon_check
 */

of_instruction_bsn_disable_split_horizon_check_t *
of_instruction_bsn_disable_split_horizon_check_new(of_version_t version)
{
    of_instruction_bsn_disable_split_horizon_check_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_BSN_DISABLE_SPLIT_HORIZON_CHECK] + of_object_extra_len[version][OF_INSTRUCTION_BSN_DISABLE_SPLIT_HORIZON_CHECK];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_bsn_disable_split_horizon_check_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_bsn_disable_split_horizon_check_init(obj, version, bytes, 0);

    if (of_instruction_bsn_disable_split_horizon_check_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_instruction_bsn_disable_split_horizon_check.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_bsn_disable_split_horizon_check_init(of_instruction_bsn_disable_split_horizon_check_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_BSN_DISABLE_SPLIT_HORIZON_CHECK] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_BSN_DISABLE_SPLIT_HORIZON_CHECK] + of_object_extra_len[version][OF_INSTRUCTION_BSN_DISABLE_SPLIT_HORIZON_CHECK];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_BSN_DISABLE_SPLIT_HORIZON_CHECK;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_instruction_bsn_disable_split_horizon_check_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_instruction_bsn_disable_split_horizon_check.
 * @param obj Pointer to an object of type of_instruction_bsn_disable_split_horizon_check.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_bsn_disable_split_horizon_check_experimenter_get(
    of_instruction_bsn_disable_split_horizon_check_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_DISABLE_SPLIT_HORIZON_CHECK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_instruction_bsn_disable_split_horizon_check.
 * @param obj Pointer to an object of type of_instruction_bsn_disable_split_horizon_check.
 * @param experimenter The value to write into the object
 */
void
of_instruction_bsn_disable_split_horizon_check_experimenter_set(
    of_instruction_bsn_disable_split_horizon_check_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_DISABLE_SPLIT_HORIZON_CHECK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_instruction_bsn_disable_split_horizon_check.
 * @param obj Pointer to an object of type of_instruction_bsn_disable_split_horizon_check.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_bsn_disable_split_horizon_check_subtype_get(
    of_instruction_bsn_disable_split_horizon_check_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_DISABLE_SPLIT_HORIZON_CHECK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_instruction_bsn_disable_split_horizon_check.
 * @param obj Pointer to an object of type of_instruction_bsn_disable_split_horizon_check.
 * @param subtype The value to write into the object
 */
void
of_instruction_bsn_disable_split_horizon_check_subtype_set(
    of_instruction_bsn_disable_split_horizon_check_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_DISABLE_SPLIT_HORIZON_CHECK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_instruction_id_bsn_disable_split_horizon_check_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x5c16c7); /* experimenter */
        *(uint32_t *)(buf + 8) = U32_HTON(0x3); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_instruction_id_bsn_disable_split_horizon_check of_instruction_id_bsn_disable_split_horizon_check
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_instruction_id_bsn_disable_split_horizon_check_push_wire_values(of_instruction_id_bsn_disable_split_horizon_check_t *obj)
{

    of_instruction_id_bsn_disable_split_horizon_check_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_instruction_id_bsn_disable_split_horizon_check object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_id_bsn_disable_split_horizon_check
 */

of_instruction_id_bsn_disable_split_horizon_check_t *
of_instruction_id_bsn_disable_split_horizon_check_new(of_version_t version)
{
    of_instruction_id_bsn_disable_split_horizon_check_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN_DISABLE_SPLIT_HORIZON_CHECK] + of_object_extra_len[version][OF_INSTRUCTION_ID_BSN_DISABLE_SPLIT_HORIZON_CHECK];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_id_bsn_disable_split_horizon_check_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_id_bsn_disable_split_horizon_check_init(obj, version, bytes, 0);

    if (of_instruction_id_bsn_disable_split_horizon_check_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_instruction_id_bsn_disable_split_horizon_check.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_id_bsn_disable_split_horizon_check_init(of_instruction_id_bsn_disable_split_horizon_check_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN_DISABLE_SPLIT_HORIZON_CHECK] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN_DISABLE_SPLIT_HORIZON_CHECK] + of_object_extra_len[version][OF_INSTRUCTION_ID_BSN_DISABLE_SPLIT_HORIZON_CHECK];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_ID_BSN_DISABLE_SPLIT_HORIZON_CHECK;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_instruction_id_bsn_disable_split_horizon_check_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_instruction_id_bsn_disable_split_horizon_check.
 * @param obj Pointer to an object of type of_instruction_id_bsn_disable_split_horizon_check.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_id_bsn_disable_split_horizon_check_experimenter_get(
    of_instruction_id_bsn_disable_split_horizon_check_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_DISABLE_SPLIT_HORIZON_CHECK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_instruction_id_bsn_disable_split_horizon_check.
 * @param obj Pointer to an object of type of_instruction_id_bsn_disable_split_horizon_check.
 * @param experimenter The value to write into the object
 */
void
of_instruction_id_bsn_disable_split_horizon_check_experimenter_set(
    of_instruction_id_bsn_disable_split_horizon_check_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_DISABLE_SPLIT_HORIZON_CHECK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_instruction_id_bsn_disable_split_horizon_check.
 * @param obj Pointer to an object of type of_instruction_id_bsn_disable_split_horizon_check.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_id_bsn_disable_split_horizon_check_subtype_get(
    of_instruction_id_bsn_disable_split_horizon_check_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_DISABLE_SPLIT_HORIZON_CHECK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_instruction_id_bsn_disable_split_horizon_check.
 * @param obj Pointer to an object of type of_instruction_id_bsn_disable_split_horizon_check.
 * @param subtype The value to write into the object
 */
void
of_instruction_id_bsn_disable_split_horizon_check_subtype_set(
    of_instruction_id_bsn_disable_split_horizon_check_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_DISABLE_SPLIT_HORIZON_CHECK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_instruction_bsn_disable_src_mac_check_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x5c16c7); /* experimenter */
        *(uint32_t *)(buf + 8) = U32_HTON(0x0); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_instruction_bsn_disable_src_mac_check of_instruction_bsn_disable_src_mac_check
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_instruction_bsn_disable_src_mac_check_push_wire_values(of_instruction_bsn_disable_src_mac_check_t *obj)
{

    of_instruction_bsn_disable_src_mac_check_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_instruction_bsn_disable_src_mac_check object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_bsn_disable_src_mac_check
 */

of_instruction_bsn_disable_src_mac_check_t *
of_instruction_bsn_disable_src_mac_check_new(of_version_t version)
{
    of_instruction_bsn_disable_src_mac_check_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_BSN_DISABLE_SRC_MAC_CHECK] + of_object_extra_len[version][OF_INSTRUCTION_BSN_DISABLE_SRC_MAC_CHECK];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_bsn_disable_src_mac_check_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_bsn_disable_src_mac_check_init(obj, version, bytes, 0);

    if (of_instruction_bsn_disable_src_mac_check_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_instruction_bsn_disable_src_mac_check.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_bsn_disable_src_mac_check_init(of_instruction_bsn_disable_src_mac_check_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_BSN_DISABLE_SRC_MAC_CHECK] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_BSN_DISABLE_SRC_MAC_CHECK] + of_object_extra_len[version][OF_INSTRUCTION_BSN_DISABLE_SRC_MAC_CHECK];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_BSN_DISABLE_SRC_MAC_CHECK;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_instruction_bsn_disable_src_mac_check_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_instruction_bsn_disable_src_mac_check.
 * @param obj Pointer to an object of type of_instruction_bsn_disable_src_mac_check.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_bsn_disable_src_mac_check_experimenter_get(
    of_instruction_bsn_disable_src_mac_check_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_DISABLE_SRC_MAC_CHECK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_instruction_bsn_disable_src_mac_check.
 * @param obj Pointer to an object of type of_instruction_bsn_disable_src_mac_check.
 * @param experimenter The value to write into the object
 */
void
of_instruction_bsn_disable_src_mac_check_experimenter_set(
    of_instruction_bsn_disable_src_mac_check_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_DISABLE_SRC_MAC_CHECK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_instruction_bsn_disable_src_mac_check.
 * @param obj Pointer to an object of type of_instruction_bsn_disable_src_mac_check.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_bsn_disable_src_mac_check_subtype_get(
    of_instruction_bsn_disable_src_mac_check_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_DISABLE_SRC_MAC_CHECK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_instruction_bsn_disable_src_mac_check.
 * @param obj Pointer to an object of type of_instruction_bsn_disable_src_mac_check.
 * @param subtype The value to write into the object
 */
void
of_instruction_bsn_disable_src_mac_check_subtype_set(
    of_instruction_bsn_disable_src_mac_check_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_DISABLE_SRC_MAC_CHECK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_instruction_id_bsn_disable_src_mac_check_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x5c16c7); /* experimenter */
        *(uint32_t *)(buf + 8) = U32_HTON(0x0); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_instruction_id_bsn_disable_src_mac_check of_instruction_id_bsn_disable_src_mac_check
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_instruction_id_bsn_disable_src_mac_check_push_wire_values(of_instruction_id_bsn_disable_src_mac_check_t *obj)
{

    of_instruction_id_bsn_disable_src_mac_check_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_instruction_id_bsn_disable_src_mac_check object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_id_bsn_disable_src_mac_check
 */

of_instruction_id_bsn_disable_src_mac_check_t *
of_instruction_id_bsn_disable_src_mac_check_new(of_version_t version)
{
    of_instruction_id_bsn_disable_src_mac_check_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN_DISABLE_SRC_MAC_CHECK] + of_object_extra_len[version][OF_INSTRUCTION_ID_BSN_DISABLE_SRC_MAC_CHECK];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_id_bsn_disable_src_mac_check_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_id_bsn_disable_src_mac_check_init(obj, version, bytes, 0);

    if (of_instruction_id_bsn_disable_src_mac_check_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_instruction_id_bsn_disable_src_mac_check.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_id_bsn_disable_src_mac_check_init(of_instruction_id_bsn_disable_src_mac_check_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN_DISABLE_SRC_MAC_CHECK] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN_DISABLE_SRC_MAC_CHECK] + of_object_extra_len[version][OF_INSTRUCTION_ID_BSN_DISABLE_SRC_MAC_CHECK];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_ID_BSN_DISABLE_SRC_MAC_CHECK;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_instruction_id_bsn_disable_src_mac_check_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_instruction_id_bsn_disable_src_mac_check.
 * @param obj Pointer to an object of type of_instruction_id_bsn_disable_src_mac_check.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_id_bsn_disable_src_mac_check_experimenter_get(
    of_instruction_id_bsn_disable_src_mac_check_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_DISABLE_SRC_MAC_CHECK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_instruction_id_bsn_disable_src_mac_check.
 * @param obj Pointer to an object of type of_instruction_id_bsn_disable_src_mac_check.
 * @param experimenter The value to write into the object
 */
void
of_instruction_id_bsn_disable_src_mac_check_experimenter_set(
    of_instruction_id_bsn_disable_src_mac_check_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_DISABLE_SRC_MAC_CHECK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_instruction_id_bsn_disable_src_mac_check.
 * @param obj Pointer to an object of type of_instruction_id_bsn_disable_src_mac_check.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_id_bsn_disable_src_mac_check_subtype_get(
    of_instruction_id_bsn_disable_src_mac_check_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_DISABLE_SRC_MAC_CHECK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_instruction_id_bsn_disable_src_mac_check.
 * @param obj Pointer to an object of type of_instruction_id_bsn_disable_src_mac_check.
 * @param subtype The value to write into the object
 */
void
of_instruction_id_bsn_disable_src_mac_check_subtype_set(
    of_instruction_id_bsn_disable_src_mac_check_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_DISABLE_SRC_MAC_CHECK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_instruction_bsn_packet_of_death_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x5c16c7); /* experimenter */
        *(uint32_t *)(buf + 8) = U32_HTON(0x6); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_instruction_bsn_packet_of_death of_instruction_bsn_packet_of_death
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_instruction_bsn_packet_of_death_push_wire_values(of_instruction_bsn_packet_of_death_t *obj)
{

    of_instruction_bsn_packet_of_death_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_instruction_bsn_packet_of_death object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_bsn_packet_of_death
 */

of_instruction_bsn_packet_of_death_t *
of_instruction_bsn_packet_of_death_new(of_version_t version)
{
    of_instruction_bsn_packet_of_death_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_BSN_PACKET_OF_DEATH] + of_object_extra_len[version][OF_INSTRUCTION_BSN_PACKET_OF_DEATH];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_bsn_packet_of_death_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_bsn_packet_of_death_init(obj, version, bytes, 0);

    if (of_instruction_bsn_packet_of_death_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_instruction_bsn_packet_of_death.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_bsn_packet_of_death_init(of_instruction_bsn_packet_of_death_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_BSN_PACKET_OF_DEATH] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_BSN_PACKET_OF_DEATH] + of_object_extra_len[version][OF_INSTRUCTION_BSN_PACKET_OF_DEATH];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_BSN_PACKET_OF_DEATH;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_instruction_bsn_packet_of_death_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_instruction_bsn_packet_of_death.
 * @param obj Pointer to an object of type of_instruction_bsn_packet_of_death.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_bsn_packet_of_death_experimenter_get(
    of_instruction_bsn_packet_of_death_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_PACKET_OF_DEATH);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_instruction_bsn_packet_of_death.
 * @param obj Pointer to an object of type of_instruction_bsn_packet_of_death.
 * @param experimenter The value to write into the object
 */
void
of_instruction_bsn_packet_of_death_experimenter_set(
    of_instruction_bsn_packet_of_death_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_PACKET_OF_DEATH);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_instruction_bsn_packet_of_death.
 * @param obj Pointer to an object of type of_instruction_bsn_packet_of_death.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_bsn_packet_of_death_subtype_get(
    of_instruction_bsn_packet_of_death_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_PACKET_OF_DEATH);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_instruction_bsn_packet_of_death.
 * @param obj Pointer to an object of type of_instruction_bsn_packet_of_death.
 * @param subtype The value to write into the object
 */
void
of_instruction_bsn_packet_of_death_subtype_set(
    of_instruction_bsn_packet_of_death_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_PACKET_OF_DEATH);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_instruction_id_bsn_packet_of_death_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x5c16c7); /* experimenter */
        *(uint32_t *)(buf + 8) = U32_HTON(0x6); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_instruction_id_bsn_packet_of_death of_instruction_id_bsn_packet_of_death
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_instruction_id_bsn_packet_of_death_push_wire_values(of_instruction_id_bsn_packet_of_death_t *obj)
{

    of_instruction_id_bsn_packet_of_death_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_instruction_id_bsn_packet_of_death object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_id_bsn_packet_of_death
 */

of_instruction_id_bsn_packet_of_death_t *
of_instruction_id_bsn_packet_of_death_new(of_version_t version)
{
    of_instruction_id_bsn_packet_of_death_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN_PACKET_OF_DEATH] + of_object_extra_len[version][OF_INSTRUCTION_ID_BSN_PACKET_OF_DEATH];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_id_bsn_packet_of_death_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_id_bsn_packet_of_death_init(obj, version, bytes, 0);

    if (of_instruction_id_bsn_packet_of_death_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_instruction_id_bsn_packet_of_death.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_id_bsn_packet_of_death_init(of_instruction_id_bsn_packet_of_death_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN_PACKET_OF_DEATH] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN_PACKET_OF_DEATH] + of_object_extra_len[version][OF_INSTRUCTION_ID_BSN_PACKET_OF_DEATH];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_ID_BSN_PACKET_OF_DEATH;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_instruction_id_bsn_packet_of_death_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_instruction_id_bsn_packet_of_death.
 * @param obj Pointer to an object of type of_instruction_id_bsn_packet_of_death.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_id_bsn_packet_of_death_experimenter_get(
    of_instruction_id_bsn_packet_of_death_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_PACKET_OF_DEATH);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_instruction_id_bsn_packet_of_death.
 * @param obj Pointer to an object of type of_instruction_id_bsn_packet_of_death.
 * @param experimenter The value to write into the object
 */
void
of_instruction_id_bsn_packet_of_death_experimenter_set(
    of_instruction_id_bsn_packet_of_death_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_PACKET_OF_DEATH);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_instruction_id_bsn_packet_of_death.
 * @param obj Pointer to an object of type of_instruction_id_bsn_packet_of_death.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_id_bsn_packet_of_death_subtype_get(
    of_instruction_id_bsn_packet_of_death_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_PACKET_OF_DEATH);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_instruction_id_bsn_packet_of_death.
 * @param obj Pointer to an object of type of_instruction_id_bsn_packet_of_death.
 * @param subtype The value to write into the object
 */
void
of_instruction_id_bsn_packet_of_death_subtype_set(
    of_instruction_id_bsn_packet_of_death_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_PACKET_OF_DEATH);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_instruction_bsn_permit_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x5c16c7); /* experimenter */
        *(uint32_t *)(buf + 8) = U32_HTON(0x4); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_instruction_bsn_permit of_instruction_bsn_permit
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_instruction_bsn_permit_push_wire_values(of_instruction_bsn_permit_t *obj)
{

    of_instruction_bsn_permit_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_instruction_bsn_permit object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_bsn_permit
 */

of_instruction_bsn_permit_t *
of_instruction_bsn_permit_new(of_version_t version)
{
    of_instruction_bsn_permit_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_BSN_PERMIT] + of_object_extra_len[version][OF_INSTRUCTION_BSN_PERMIT];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_bsn_permit_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_bsn_permit_init(obj, version, bytes, 0);

    if (of_instruction_bsn_permit_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_instruction_bsn_permit.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_bsn_permit_init(of_instruction_bsn_permit_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_BSN_PERMIT] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_BSN_PERMIT] + of_object_extra_len[version][OF_INSTRUCTION_BSN_PERMIT];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_BSN_PERMIT;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_instruction_bsn_permit_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_instruction_bsn_permit.
 * @param obj Pointer to an object of type of_instruction_bsn_permit.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_bsn_permit_experimenter_get(
    of_instruction_bsn_permit_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_PERMIT);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_instruction_bsn_permit.
 * @param obj Pointer to an object of type of_instruction_bsn_permit.
 * @param experimenter The value to write into the object
 */
void
of_instruction_bsn_permit_experimenter_set(
    of_instruction_bsn_permit_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_PERMIT);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_instruction_bsn_permit.
 * @param obj Pointer to an object of type of_instruction_bsn_permit.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_bsn_permit_subtype_get(
    of_instruction_bsn_permit_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_PERMIT);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_instruction_bsn_permit.
 * @param obj Pointer to an object of type of_instruction_bsn_permit.
 * @param subtype The value to write into the object
 */
void
of_instruction_bsn_permit_subtype_set(
    of_instruction_bsn_permit_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_BSN_PERMIT);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_instruction_id_bsn_permit_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x5c16c7); /* experimenter */
        *(uint32_t *)(buf + 8) = U32_HTON(0x4); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_instruction_id_bsn_permit of_instruction_id_bsn_permit
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_instruction_id_bsn_permit_push_wire_values(of_instruction_id_bsn_permit_t *obj)
{

    of_instruction_id_bsn_permit_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_instruction_id_bsn_permit object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_id_bsn_permit
 */

of_instruction_id_bsn_permit_t *
of_instruction_id_bsn_permit_new(of_version_t version)
{
    of_instruction_id_bsn_permit_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN_PERMIT] + of_object_extra_len[version][OF_INSTRUCTION_ID_BSN_PERMIT];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_id_bsn_permit_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_id_bsn_permit_init(obj, version, bytes, 0);

    if (of_instruction_id_bsn_permit_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_instruction_id_bsn_permit.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_id_bsn_permit_init(of_instruction_id_bsn_permit_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN_PERMIT] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_BSN_PERMIT] + of_object_extra_len[version][OF_INSTRUCTION_ID_BSN_PERMIT];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_ID_BSN_PERMIT;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_instruction_id_bsn_permit_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_instruction_id_bsn_permit.
 * @param obj Pointer to an object of type of_instruction_id_bsn_permit.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_id_bsn_permit_experimenter_get(
    of_instruction_id_bsn_permit_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_PERMIT);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_instruction_id_bsn_permit.
 * @param obj Pointer to an object of type of_instruction_id_bsn_permit.
 * @param experimenter The value to write into the object
 */
void
of_instruction_id_bsn_permit_experimenter_set(
    of_instruction_id_bsn_permit_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_PERMIT);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_instruction_id_bsn_permit.
 * @param obj Pointer to an object of type of_instruction_id_bsn_permit.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_id_bsn_permit_subtype_get(
    of_instruction_id_bsn_permit_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_PERMIT);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_instruction_id_bsn_permit.
 * @param obj Pointer to an object of type of_instruction_id_bsn_permit.
 * @param subtype The value to write into the object
 */
void
of_instruction_id_bsn_permit_subtype_set(
    of_instruction_id_bsn_permit_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_ID_BSN_PERMIT);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_instruction_id_clear_actions_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x5); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_instruction_id_clear_actions of_instruction_id_clear_actions
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_instruction_id_clear_actions_push_wire_values(of_instruction_id_clear_actions_t *obj)
{

    of_instruction_id_clear_actions_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_instruction_id_clear_actions object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_id_clear_actions
 */

of_instruction_id_clear_actions_t *
of_instruction_id_clear_actions_new(of_version_t version)
{
    of_instruction_id_clear_actions_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_CLEAR_ACTIONS] + of_object_extra_len[version][OF_INSTRUCTION_ID_CLEAR_ACTIONS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_id_clear_actions_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_id_clear_actions_init(obj, version, bytes, 0);

    if (of_instruction_id_clear_actions_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_instruction_id_clear_actions.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_id_clear_actions_init(of_instruction_id_clear_actions_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_ID_CLEAR_ACTIONS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_CLEAR_ACTIONS] + of_object_extra_len[version][OF_INSTRUCTION_ID_CLEAR_ACTIONS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_ID_CLEAR_ACTIONS;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_instruction_id_clear_actions_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}

/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_instruction_id_goto_table_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x1); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_instruction_id_goto_table of_instruction_id_goto_table
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_instruction_id_goto_table_push_wire_values(of_instruction_id_goto_table_t *obj)
{

    of_instruction_id_goto_table_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_instruction_id_goto_table object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_id_goto_table
 */

of_instruction_id_goto_table_t *
of_instruction_id_goto_table_new(of_version_t version)
{
    of_instruction_id_goto_table_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_GOTO_TABLE] + of_object_extra_len[version][OF_INSTRUCTION_ID_GOTO_TABLE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_id_goto_table_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_id_goto_table_init(obj, version, bytes, 0);

    if (of_instruction_id_goto_table_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_instruction_id_goto_table.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_id_goto_table_init(of_instruction_id_goto_table_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_ID_GOTO_TABLE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_GOTO_TABLE] + of_object_extra_len[version][OF_INSTRUCTION_ID_GOTO_TABLE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_ID_GOTO_TABLE;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_instruction_id_goto_table_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}

/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_instruction_meter_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x6); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_instruction_meter of_instruction_meter
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_instruction_meter_push_wire_values(of_instruction_meter_t *obj)
{

    of_instruction_meter_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_instruction_meter object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_meter
 */

of_instruction_meter_t *
of_instruction_meter_new(of_version_t version)
{
    of_instruction_meter_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_METER] + of_object_extra_len[version][OF_INSTRUCTION_METER];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_meter_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_meter_init(obj, version, bytes, 0);

    if (of_instruction_meter_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_instruction_meter.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_meter_init(of_instruction_meter_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_METER] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_METER] + of_object_extra_len[version][OF_INSTRUCTION_METER];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_METER;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_instruction_meter_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get meter_id from an object of type of_instruction_meter.
 * @param obj Pointer to an object of type of_instruction_meter.
 * @param meter_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_instruction_meter_meter_id_get(
    of_instruction_meter_t *obj,
    uint32_t *meter_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_METER);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, meter_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set meter_id in an object of type of_instruction_meter.
 * @param obj Pointer to an object of type of_instruction_meter.
 * @param meter_id The value to write into the object
 */
void
of_instruction_meter_meter_id_set(
    of_instruction_meter_t *obj,
    uint32_t meter_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_INSTRUCTION_METER);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, meter_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_instruction_id_meter_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x6); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_instruction_id_meter of_instruction_id_meter
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_instruction_id_meter_push_wire_values(of_instruction_id_meter_t *obj)
{

    of_instruction_id_meter_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_instruction_id_meter object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_id_meter
 */

of_instruction_id_meter_t *
of_instruction_id_meter_new(of_version_t version)
{
    of_instruction_id_meter_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_METER] + of_object_extra_len[version][OF_INSTRUCTION_ID_METER];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_id_meter_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_id_meter_init(obj, version, bytes, 0);

    if (of_instruction_id_meter_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_instruction_id_meter.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_id_meter_init(of_instruction_id_meter_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_ID_METER] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_METER] + of_object_extra_len[version][OF_INSTRUCTION_ID_METER];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_ID_METER;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_instruction_id_meter_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}

/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_instruction_id_write_actions_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x3); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_instruction_id_write_actions of_instruction_id_write_actions
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_instruction_id_write_actions_push_wire_values(of_instruction_id_write_actions_t *obj)
{

    of_instruction_id_write_actions_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_instruction_id_write_actions object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_id_write_actions
 */

of_instruction_id_write_actions_t *
of_instruction_id_write_actions_new(of_version_t version)
{
    of_instruction_id_write_actions_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_WRITE_ACTIONS] + of_object_extra_len[version][OF_INSTRUCTION_ID_WRITE_ACTIONS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_id_write_actions_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_id_write_actions_init(obj, version, bytes, 0);

    if (of_instruction_id_write_actions_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_instruction_id_write_actions.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_id_write_actions_init(of_instruction_id_write_actions_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_ID_WRITE_ACTIONS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_WRITE_ACTIONS] + of_object_extra_len[version][OF_INSTRUCTION_ID_WRITE_ACTIONS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_ID_WRITE_ACTIONS;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_instruction_id_write_actions_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}

/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_instruction_id_write_metadata_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x2); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_instruction_id_write_metadata of_instruction_id_write_metadata
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_instruction_id_write_metadata_push_wire_values(of_instruction_id_write_metadata_t *obj)
{

    of_instruction_id_write_metadata_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_instruction_id_write_metadata object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_instruction_id_write_metadata
 */

of_instruction_id_write_metadata_t *
of_instruction_id_write_metadata_new(of_version_t version)
{
    of_instruction_id_write_metadata_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_WRITE_METADATA] + of_object_extra_len[version][OF_INSTRUCTION_ID_WRITE_METADATA];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_instruction_id_write_metadata_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_instruction_id_write_metadata_init(obj, version, bytes, 0);

    if (of_instruction_id_write_metadata_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_instruction_id_write_metadata.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_instruction_id_write_metadata_init(of_instruction_id_write_metadata_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_INSTRUCTION_ID_WRITE_METADATA] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_INSTRUCTION_ID_WRITE_METADATA] + of_object_extra_len[version][OF_INSTRUCTION_ID_WRITE_METADATA];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_INSTRUCTION_ID_WRITE_METADATA;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_instruction_id_write_metadata_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_instruction_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}

/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"


void
of_meter_mod_wire_object_id_get(of_object_t *obj, of_object_id_t *id)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3: {
        uint16_t value = U16_NTOH(*(uint16_t *)(buf + 8)); /* command */
        switch (value) {
        case 0x0:
            *id = OF_METER_ADD;
            break;
        case 0x1:
            *id = OF_METER_MODIFY;
            break;
        case 0x2:
            *id = OF_METER_DELETE;
            break;
        default:
            *id = OF_METER_MOD;
            break;
        }
        break;
    }
    default:
        LOCI_ASSERT(0);
    }
}


/**
 * \defgroup of_meter_mod of_meter_mod
 */

/**
 * Create a new of_meter_mod object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_meter_mod
 */

of_meter_mod_t *
of_meter_mod_new(of_version_t version)
{
    of_meter_mod_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_METER_MOD] + of_object_extra_len[version][OF_METER_MOD];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_meter_mod_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_meter_mod_init(obj, version, bytes, 0);

    return obj;
}

/**
 * Initialize an object of type of_meter_mod.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_meter_mod_init(of_meter_mod_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_METER_MOD] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_METER_MOD] + of_object_extra_len[version][OF_METER_MOD];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_METER_MOD;

    /* Set up the object's function pointers */

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new of_meter_mod object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_meter_mod
 */

of_meter_mod_t *
of_meter_mod_new_from_message(of_message_t msg)
{
    of_meter_mod_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_meter_mod_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_meter_mod_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type of_meter_mod.
 * @param obj Pointer to an object of type of_meter_mod.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_mod_xid_get(
    of_meter_mod_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type of_meter_mod.
 * @param obj Pointer to an object of type of_meter_mod.
 * @param xid The value to write into the object
 */
void
of_meter_mod_xid_set(
    of_meter_mod_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type of_meter_mod.
 * @param obj Pointer to an object of type of_meter_mod.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_meter_mod_flags_get(
    of_meter_mod_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type of_meter_mod.
 * @param obj Pointer to an object of type of_meter_mod.
 * @param flags The value to write into the object
 */
void
of_meter_mod_flags_set(
    of_meter_mod_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get meter_id from an object of type of_meter_mod.
 * @param obj Pointer to an object of type of_meter_mod.
 * @param meter_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_mod_meter_id_get(
    of_meter_mod_t *obj,
    uint32_t *meter_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, meter_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set meter_id in an object of type of_meter_mod.
 * @param obj Pointer to an object of type of_meter_mod.
 * @param meter_id The value to write into the object
 */
void
of_meter_mod_meter_id_set(
    of_meter_mod_t *obj,
    uint32_t meter_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, meter_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Bind an object of type of_list_meter_band_t to the parent of type of_meter_mod for
 * member meters
 * @param obj Pointer to an object of type of_meter_mod.
 * @param meters Pointer to the child object of type
 * of_list_meter_band_t to be filled out.
 * \ingroup of_meter_mod
 *
 * The parameter meters is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_meter_mod_meters_bind(
    of_meter_mod_t *obj,
    of_list_meter_band_t *meters)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_METER_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_meter_band_init(meters, obj->version, 0, 1);
    /* Attach to parent */
    meters->parent = (of_object_t *)obj;
    meters->wire_object.wbuf = obj->wire_object.wbuf;
    meters->wire_object.obj_offset = abs_offset;
    meters->wire_object.owned = 0;
    meters->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of meters into a new variable of type of_list_meter_band_t from
 * a of_meter_mod instance.
 *
 * @param obj Pointer to the source of type of_meter_mod_t
 * @returns A pointer to a new instance of type of_list_meter_band_t whose contents
 * match that of meters from source
 * @returns NULL if an error occurs
 */
of_list_meter_band_t *
of_meter_mod_meters_get(of_meter_mod_t *obj) {
    of_list_meter_band_t _meters;
    of_list_meter_band_t *_meters_ptr;

    of_meter_mod_meters_bind(obj, &_meters);
    _meters_ptr = (of_list_meter_band_t *)of_object_dup(&_meters);
    return _meters_ptr;
}

/**
 * Set meters in an object of type of_meter_mod.
 * @param obj Pointer to an object of type of_meter_mod.
 * @param meters Pointer to the child of type of_list_meter_band_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_meter_mod_meters_set(
    of_meter_mod_t *obj,
    of_list_meter_band_t *meters)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_METER_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = meters->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == meters->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(meters, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(meters, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (meters->wire_length_set != NULL) {
        meters->wire_length_set((of_object_t *)meters, meters->length);
    }

    /* Not scalar, update lengths if needed */
    delta = new_len - cur_len;
    if (delta != 0) {
        /* Update parent(s) */
        of_object_parent_length_update((of_object_t *)obj, delta);
    }

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_meter_add_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x1d; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0x0); /* command */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_meter_add of_meter_add
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_meter_add_push_wire_values(of_meter_add_t *obj)
{

    of_meter_add_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_meter_add object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_meter_add
 */

of_meter_add_t *
of_meter_add_new(of_version_t version)
{
    of_meter_add_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_METER_ADD] + of_object_extra_len[version][OF_METER_ADD];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_meter_add_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_meter_add_init(obj, version, bytes, 0);

    if (of_meter_add_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_meter_add.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_meter_add_init(of_meter_add_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_METER_ADD] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_METER_ADD] + of_object_extra_len[version][OF_METER_ADD];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_METER_ADD;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_meter_add_push_wire_types;

    obj->wire_length_get = of_object_message_wire_length_get;
    obj->wire_length_set = of_object_message_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Create a new of_meter_add object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_meter_add
 */

of_meter_add_t *
of_meter_add_new_from_message(of_message_t msg)
{
    of_meter_add_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_meter_add_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_meter_add_init(obj, version, 0, 0);

    if ((of_object_buffer_bind((of_object_t *)obj, OF_MESSAGE_TO_BUFFER(msg),
                               length, OF_MESSAGE_FREE_FUNCTION)) < 0) {
       FREE(obj);
       return NULL;
    }
    obj->length = length;
    obj->version = version;

    return obj;
}

/**
 * Get xid from an object of type of_meter_add.
 * @param obj Pointer to an object of type of_meter_add.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_add_xid_get(
    of_meter_add_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_ADD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set xid in an object of type of_meter_add.
 * @param obj Pointer to an object of type of_meter_add.
 * @param xid The value to write into the object
 */
void
of_meter_add_xid_set(
    of_meter_add_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_ADD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, xid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type of_meter_add.
 * @param obj Pointer to an object of type of_meter_add.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_meter_add_flags_get(
    of_meter_add_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_ADD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type of_meter_add.
 * @param obj Pointer to an object of type of_meter_add.
 * @param flags The value to write into the object
 */
void
of_meter_add_flags_set(
    of_meter_add_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_ADD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get meter_id from an object of type of_meter_add.
 * @param obj Pointer to an object of type of_meter_add.
 * @param meter_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_add_meter_id_get(
    of_meter_add_t *obj,
    uint32_t *meter_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_ADD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, meter_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set meter_id in an object of type of_meter_add.
 * @param obj Pointer to an object of type of_meter_add.
 * @param meter_id The value to write into the object
 */
void
of_meter_add_meter_id_set(
    of_meter_add_t *obj,
    uint32_t meter_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_ADD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, meter_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Bind an object of type of_list_meter_band_t to the parent of type of_meter_add for
 * member meters
 * @param obj Pointer to an object of type of_meter_add.
 * @param meters Pointer to the child object of type
 * of_list_meter_band_t to be filled out.
 * \ingroup of_meter_add
 *
 * The parameter meters is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_meter_add_meters_bind(
    of_meter_add_t *obj,
    of_list_meter_band_t *meters)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_METER_ADD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_meter_band_init(meters, obj->version, 0, 1);
    /* Attach to parent */
    meters->parent = (of_object_t *)obj;
    meters->wire_object.wbuf = obj->wire_object.wbuf;
    meters->wire_object.obj_offset = abs_offset;
    meters->wire_object.owned = 0;
    meters->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of meters into a new variable of type of_list_meter_band_t from
 * a of_meter_add instance.
 *
 * @param obj Pointer to the source of type of_meter_add_t
 * @returns A pointer to a new instance of type of_list_meter_band_t whose contents
 * match that of meters from source
 * @returns NULL if an error occurs
 */
of_list_meter_band_t *
of_meter_add_meters_get(of_meter_add_t *obj) {
    of_list_meter_band_t _meters;
    of_list_meter_band_t *_meters_ptr;

    of_meter_add_meters_bind(obj, &_meters);
    _meters_ptr = (of_list_meter_band_t *)of_object_dup(&_meters);
    return _meters_ptr;
}

/**
 * Set meters in an object of type of_meter_add.
 * @param obj Pointer to an object of type of_meter_add.
 * @param meters Pointer to the child of type of_list_meter_band_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_meter_add_meters_set(
    of_meter_add_t *obj,
    of_list_meter_band_t *meters)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_METER_ADD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = meters->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == meters->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(meters, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(meters, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (meters->wire_length_set != NULL) {
        meters->wire_length_set((of_object_t *)meters, meters->length);
    }

    /* Not scalar, update lengths if needed */
    delta = new_len - cur_len;
    if (delta != 0) {
        /* Update parent(s) */
        of_object_parent_length_update((of_object_t *)obj, delta);
    }

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"


void
of_meter_band_wire_object_id_get(of_object_t *obj, of_object_id_t *id)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3: {
        uint16_t value = U16_NTOH(*(uint16_t *)(buf + 0)); /* type */
        switch (value) {
        case 0x1:
            *id = OF_METER_BAND_DROP;
            break;
        case 0x2:
            *id = OF_METER_BAND_DSCP_REMARK;
            break;
        case 0xffff:
            of_meter_band_experimenter_wire_object_id_get(obj, id);
            break;
        default:
            *id = OF_METER_BAND;
            break;
        }
        break;
    }
    default:
        LOCI_ASSERT(0);
    }
}


/**
 * \defgroup of_meter_band of_meter_band
 */

/**
 * Create a new of_meter_band object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_meter_band
 */

of_meter_band_t *
of_meter_band_new(of_version_t version)
{
    of_meter_band_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_METER_BAND] + of_object_extra_len[version][OF_METER_BAND];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_meter_band_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_meter_band_init(obj, version, bytes, 0);

    return obj;
}

/**
 * Initialize an object of type of_meter_band.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_meter_band_init(of_meter_band_t *obj_p,
    of_version_t version, int bytes, int clean_wire)
{
    of_meter_band_header_t *obj;

    obj = &obj_p->header;  /* Need instantiable subclass */

    LOCI_ASSERT(of_object_fixed_len[version][OF_METER_BAND] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_METER_BAND] + of_object_extra_len[version][OF_METER_BAND];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_METER_BAND;

    /* Set up the object's function pointers */

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_meter_band_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}

/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_meter_band_drop_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x1); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_meter_band_drop of_meter_band_drop
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_meter_band_drop_push_wire_values(of_meter_band_drop_t *obj)
{

    of_meter_band_drop_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_meter_band_drop object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_meter_band_drop
 */

of_meter_band_drop_t *
of_meter_band_drop_new(of_version_t version)
{
    of_meter_band_drop_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_METER_BAND_DROP] + of_object_extra_len[version][OF_METER_BAND_DROP];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_meter_band_drop_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_meter_band_drop_init(obj, version, bytes, 0);

    if (of_meter_band_drop_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_meter_band_drop.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_meter_band_drop_init(of_meter_band_drop_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_METER_BAND_DROP] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_METER_BAND_DROP] + of_object_extra_len[version][OF_METER_BAND_DROP];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_METER_BAND_DROP;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_meter_band_drop_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_meter_band_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get rate from an object of type of_meter_band_drop.
 * @param obj Pointer to an object of type of_meter_band_drop.
 * @param rate Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_band_drop_rate_get(
    of_meter_band_drop_t *obj,
    uint32_t *rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_DROP);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set rate in an object of type of_meter_band_drop.
 * @param obj Pointer to an object of type of_meter_band_drop.
 * @param rate The value to write into the object
 */
void
of_meter_band_drop_rate_set(
    of_meter_band_drop_t *obj,
    uint32_t rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_DROP);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get burst_size from an object of type of_meter_band_drop.
 * @param obj Pointer to an object of type of_meter_band_drop.
 * @param burst_size Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_band_drop_burst_size_get(
    of_meter_band_drop_t *obj,
    uint32_t *burst_size)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_DROP);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, burst_size);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set burst_size in an object of type of_meter_band_drop.
 * @param obj Pointer to an object of type of_meter_band_drop.
 * @param burst_size The value to write into the object
 */
void
of_meter_band_drop_burst_size_set(
    of_meter_band_drop_t *obj,
    uint32_t burst_size)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_DROP);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, burst_size);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_meter_band_dscp_remark_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x2); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_meter_band_dscp_remark of_meter_band_dscp_remark
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_meter_band_dscp_remark_push_wire_values(of_meter_band_dscp_remark_t *obj)
{

    of_meter_band_dscp_remark_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_meter_band_dscp_remark object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_meter_band_dscp_remark
 */

of_meter_band_dscp_remark_t *
of_meter_band_dscp_remark_new(of_version_t version)
{
    of_meter_band_dscp_remark_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_METER_BAND_DSCP_REMARK] + of_object_extra_len[version][OF_METER_BAND_DSCP_REMARK];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_meter_band_dscp_remark_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_meter_band_dscp_remark_init(obj, version, bytes, 0);

    if (of_meter_band_dscp_remark_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_meter_band_dscp_remark.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_meter_band_dscp_remark_init(of_meter_band_dscp_remark_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_METER_BAND_DSCP_REMARK] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_METER_BAND_DSCP_REMARK] + of_object_extra_len[version][OF_METER_BAND_DSCP_REMARK];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_METER_BAND_DSCP_REMARK;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_meter_band_dscp_remark_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_meter_band_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get rate from an object of type of_meter_band_dscp_remark.
 * @param obj Pointer to an object of type of_meter_band_dscp_remark.
 * @param rate Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_band_dscp_remark_rate_get(
    of_meter_band_dscp_remark_t *obj,
    uint32_t *rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_DSCP_REMARK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set rate in an object of type of_meter_band_dscp_remark.
 * @param obj Pointer to an object of type of_meter_band_dscp_remark.
 * @param rate The value to write into the object
 */
void
of_meter_band_dscp_remark_rate_set(
    of_meter_band_dscp_remark_t *obj,
    uint32_t rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_DSCP_REMARK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get burst_size from an object of type of_meter_band_dscp_remark.
 * @param obj Pointer to an object of type of_meter_band_dscp_remark.
 * @param burst_size Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_band_dscp_remark_burst_size_get(
    of_meter_band_dscp_remark_t *obj,
    uint32_t *burst_size)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_DSCP_REMARK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, burst_size);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set burst_size in an object of type of_meter_band_dscp_remark.
 * @param obj Pointer to an object of type of_meter_band_dscp_remark.
 * @param burst_size The value to write into the object
 */
void
of_meter_band_dscp_remark_burst_size_set(
    of_meter_band_dscp_remark_t *obj,
    uint32_t burst_size)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_DSCP_REMARK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, burst_size);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get prec_level from an object of type of_meter_band_dscp_remark.
 * @param obj Pointer to an object of type of_meter_band_dscp_remark.
 * @param prec_level Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_meter_band_dscp_remark_prec_level_get(
    of_meter_band_dscp_remark_t *obj,
    uint8_t *prec_level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_DSCP_REMARK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, prec_level);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set prec_level in an object of type of_meter_band_dscp_remark.
 * @param obj Pointer to an object of type of_meter_band_dscp_remark.
 * @param prec_level The value to write into the object
 */
void
of_meter_band_dscp_remark_prec_level_set(
    of_meter_band_dscp_remark_t *obj,
    uint8_t prec_level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_DSCP_REMARK);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, prec_level);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"


void
of_meter_band_experimenter_wire_object_id_get(of_object_t *obj, of_object_id_t *id)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 12)); /* experimenter */
        switch (value) {
        case 0x1018:
            *id = OF_METER_BAND_OFDPA_COLOR_SET;
            break;
        default:
            *id = OF_METER_BAND_EXPERIMENTER;
            break;
        }
        break;
    }
    default:
        LOCI_ASSERT(0);
    }
}


/**
 * \defgroup of_meter_band_experimenter of_meter_band_experimenter
 */

/**
 * Create a new of_meter_band_experimenter object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_meter_band_experimenter
 */

of_meter_band_experimenter_t *
of_meter_band_experimenter_new(of_version_t version)
{
    of_meter_band_experimenter_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_METER_BAND_EXPERIMENTER] + of_object_extra_len[version][OF_METER_BAND_EXPERIMENTER];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_meter_band_experimenter_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_meter_band_experimenter_init(obj, version, bytes, 0);

    return obj;
}

/**
 * Initialize an object of type of_meter_band_experimenter.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_meter_band_experimenter_init(of_meter_band_experimenter_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_METER_BAND_EXPERIMENTER] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_METER_BAND_EXPERIMENTER] + of_object_extra_len[version][OF_METER_BAND_EXPERIMENTER];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_METER_BAND_EXPERIMENTER;

    /* Set up the object's function pointers */

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_meter_band_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get rate from an object of type of_meter_band_experimenter.
 * @param obj Pointer to an object of type of_meter_band_experimenter.
 * @param rate Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_band_experimenter_rate_get(
    of_meter_band_experimenter_t *obj,
    uint32_t *rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_EXPERIMENTER);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set rate in an object of type of_meter_band_experimenter.
 * @param obj Pointer to an object of type of_meter_band_experimenter.
 * @param rate The value to write into the object
 */
void
of_meter_band_experimenter_rate_set(
    of_meter_band_experimenter_t *obj,
    uint32_t rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_EXPERIMENTER);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get burst_size from an object of type of_meter_band_experimenter.
 * @param obj Pointer to an object of type of_meter_band_experimenter.
 * @param burst_size Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_band_experimenter_burst_size_get(
    of_meter_band_experimenter_t *obj,
    uint32_t *burst_size)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_EXPERIMENTER);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, burst_size);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set burst_size in an object of type of_meter_band_experimenter.
 * @param obj Pointer to an object of type of_meter_band_experimenter.
 * @param burst_size The value to write into the object
 */
void
of_meter_band_experimenter_burst_size_set(
    of_meter_band_experimenter_t *obj,
    uint32_t burst_size)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_EXPERIMENTER);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, burst_size);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type of_meter_band_experimenter.
 * @param obj Pointer to an object of type of_meter_band_experimenter.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_band_experimenter_experimenter_get(
    of_meter_band_experimenter_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_EXPERIMENTER);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_meter_band_experimenter.
 * @param obj Pointer to an object of type of_meter_band_experimenter.
 * @param experimenter The value to write into the object
 */
void
of_meter_band_experimenter_experimenter_set(
    of_meter_band_experimenter_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_EXPERIMENTER);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get data from an object of type of_meter_band_experimenter.
 * @param obj Pointer to an object of type of_meter_band_experimenter.
 * @param data Pointer to the child object of type
 * of_octets_t to be filled out.
 *
 */
void
of_meter_band_experimenter_data_get(
    of_meter_band_experimenter_t *obj,
    of_octets_t *data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_EXPERIMENTER);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);
    LOCI_ASSERT(cur_len + abs_offset <= WBUF_CURRENT_BYTES(wbuf));
    data->bytes = cur_len;
    data->data = OF_WIRE_BUFFER_INDEX(wbuf, abs_offset);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set data in an object of type of_meter_band_experimenter.
 * @param obj Pointer to an object of type of_meter_band_experimenter.
 * @param data The value to write into the object
 */
int WARN_UNUSED_RESULT
of_meter_band_experimenter_data_set(
    of_meter_band_experimenter_t *obj,
    of_octets_t *data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_EXPERIMENTER);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);
    new_len = data->bytes;
    of_wire_buffer_grow(wbuf, abs_offset + (new_len - cur_len));
    of_wire_buffer_octets_data_set(wbuf, abs_offset, data, cur_len);

    /* Not scalar, update lengths if needed */
    delta = new_len - cur_len;
    if (delta != 0) {
        /* Update parent(s) */
        of_object_parent_length_update((of_object_t *)obj, delta);
    }

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"

static void
of_meter_band_ofdpa_color_set_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 12) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 16) = U16_HTON(0x3); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_meter_band_ofdpa_color_set of_meter_band_ofdpa_color_set
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_meter_band_ofdpa_color_set_push_wire_values(of_meter_band_ofdpa_color_set_t *obj)
{

    of_meter_band_ofdpa_color_set_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_meter_band_ofdpa_color_set object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_meter_band_ofdpa_color_set
 */

of_meter_band_ofdpa_color_set_t *
of_meter_band_ofdpa_color_set_new(of_version_t version)
{
    of_meter_band_ofdpa_color_set_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_METER_BAND_OFDPA_COLOR_SET] + of_object_extra_len[version][OF_METER_BAND_OFDPA_COLOR_SET];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_meter_band_ofdpa_color_set_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_meter_band_ofdpa_color_set_init(obj, version, bytes, 0);

    if (of_meter_band_ofdpa_color_set_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_meter_band_ofdpa_color_set.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_meter_band_ofdpa_color_set_init(of_meter_band_ofdpa_color_set_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_METER_BAND_OFDPA_COLOR_SET] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_METER_BAND_OFDPA_COLOR_SET] + of_object_extra_len[version][OF_METER_BAND_OFDPA_COLOR_SET];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_METER_BAND_OFDPA_COLOR_SET;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_meter_band_ofdpa_color_set_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_meter_band_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get rate from an object of type of_meter_band_ofdpa_color_set.
 * @param obj Pointer to an object of type of_meter_band_ofdpa_color_set.
 * @param rate Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_band_ofdpa_color_set_rate_get(
    of_meter_band_ofdpa_color_set_t *obj,
    uint32_t *rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_OFDPA_COLOR_SET);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set rate in an object of type of_meter_band_ofdpa_color_set.
 * @param obj Pointer to an object of type of_meter_band_ofdpa_color_set.
 * @param rate The value to write into the object
 */
void
of_meter_band_ofdpa_color_set_rate_set(
    of_meter_band_ofdpa_color_set_t *obj,
    uint32_t rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_OFDPA_COLOR_SET);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get burst_size from an object of type of_meter_band_ofdpa_color_set.
 * @param obj Pointer to an object of type of_meter_band_ofdpa_color_set.
 * @param burst_size Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_band_ofdpa_color_set_burst_size_get(
    of_meter_band_ofdpa_color_set_t *obj,
    uint32_t *burst_size)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_OFDPA_COLOR_SET);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, burst_size);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set burst_size in an object of type of_meter_band_ofdpa_color_set.
 * @param obj Pointer to an object of type of_meter_band_ofdpa_color_set.
 * @param burst_size The value to write into the object
 */
void
of_meter_band_ofdpa_color_set_burst_size_set(
    of_meter_band_ofdpa_color_set_t *obj,
    uint32_t burst_size)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_OFDPA_COLOR_SET);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, burst_size);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type of_meter_band_ofdpa_color_set.
 * @param obj Pointer to an object of type of_meter_band_ofdpa_color_set.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_band_ofdpa_color_set_experimenter_get(
    of_meter_band_ofdpa_color_set_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_OFDPA_COLOR_SET);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_meter_band_ofdpa_color_set.
 * @param obj Pointer to an object of type of_meter_band_ofdpa_color_set.
 * @param experimenter The value to write into the object
 */
void
of_meter_band_ofdpa_color_set_experimenter_set(
    of_meter_band_ofdpa_color_set_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_OFDPA_COLOR_SET);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_meter_band_ofdpa_color_set.
 * @param obj Pointer to an object of type of_meter_band_ofdpa_color_set.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_meter_band_ofdpa_color_set_subtype_get(
    of_meter_band_ofdpa_color_set_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_OFDPA_COLOR_SET);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_meter_band_ofdpa_color_set.
 * @param obj Pointer to an object of type of_meter_band_ofdpa_color_set.
 * @param subtype The value to write into the object
 */
void
of_meter_band_ofdpa_color_set_subtype_set(
    of_meter_band_ofdpa_color_set_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_OFDPA_COLOR_SET);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mode from an object of type of_meter_band_ofdpa_color_set.
 * @param obj Pointer to an object of type of_meter_band_ofdpa_color_set.
 * @param mode Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_meter_band_ofdpa_color_set_mode_get(
    of_meter_band_ofdpa_color_set_t *obj,
    uint8_t *mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_OFDPA_COLOR_SET);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 18;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mode in an object of type of_meter_band_ofdpa_color_set.
 * @param obj Pointer to an object of type of_meter_band_ofdpa_color_set.
 * @param mode The value to write into the object
 */
void
of_meter_band_ofdpa_color_set_mode_set(
    of_meter_band_ofdpa_color_set_t *obj,
    uint8_t mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_OFDPA_COLOR_SET);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 18;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get color_aware from an object of type of_meter_band_ofdpa_color_set.
 * @param obj Pointer to an object of type of_meter_band_ofdpa_color_set.
 * @param color_aware Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_meter_band_ofdpa_color_set_color_aware_get(
    of_meter_band_ofdpa_color_set_t *obj,
    uint8_t *color_aware)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_OFDPA_COLOR_SET);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 19;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, color_aware);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set color_aware in an object of type of_meter_band_ofdpa_color_set.
 * @param obj Pointer to an object of type of_meter_band_ofdpa_color_set.
 * @param color_aware The value to write into the object
 */
void
of_meter_band_ofdpa_color_set_color_aware_set(
    of_meter_band_ofdpa_color_set_t *obj,
    uint8_t color_aware)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_OFDPA_COLOR_SET);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 19;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, color_aware);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get color from an object of type of_meter_band_ofdpa_color_set.
 * @param obj Pointer to an object of type of_meter_band_ofdpa_color_set.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_meter_band_ofdpa_color_set_color_get(
    of_meter_band_ofdpa_color_set_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_OFDPA_COLOR_SET);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set color in an object of type of_meter_band_ofdpa_color_set.
 * @param obj Pointer to an object of type of_meter_band_ofdpa_color_set.
 * @param color The value to write into the object
 */
void
of_meter_band_ofdpa_color_set_color_set(
    of_meter_band_ofdpa_color_set_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_OFDPA_COLOR_SET);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"



/**
 * \defgroup of_meter_band_stats of_meter_band_stats
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_meter_band_stats_push_wire_values(of_meter_band_stats_t *obj)
{

    return OF_ERROR_NONE;
}

/**
 * Create a new of_meter_band_stats object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_meter_band_stats
 */

of_meter_band_stats_t *
of_meter_band_stats_new(of_version_t version)
{
    of_meter_band_stats_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_METER_BAND_STATS] + of_object_extra_len[version][OF_METER_BAND_STATS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_meter_band_stats_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_meter_band_stats_init(obj, version, bytes, 0);

    if (of_meter_band_stats_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_meter_band_stats.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_meter_band_stats_init(of_meter_band_stats_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_METER_BAND_STATS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_METER_BAND_STATS] + of_object_extra_len[version][OF_METER_BAND_STATS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_METER_BAND_STATS;

    /* Set up the object's function pointers */

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get packet_band_count from an object of type of_meter_band_stats.
 * @param obj Pointer to an object of type of_meter_band_stats.
 * @param packet_band_count Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_meter_band_stats_packet_band_count_get(
    of_meter_band_stats_t *obj,
    uint64_t *packet_band_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_STATS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 0;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, packet_band_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set packet_band_count in an object of type of_meter_band_stats.
 * @param obj Pointer to an object of type of_meter_band_stats.
 * @param packet_band_count The value to write into the object
 */
void
of_meter_band_stats_packet_band_count_set(
    of_meter_band_stats_t *obj,
    uint64_t packet_band_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_STATS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 0;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, packet_band_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get byte_band_count from an object of type of_meter_band_stats.
 * @param obj Pointer to an object of type of_meter_band_stats.
 * @param byte_band_count Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_meter_band_stats_byte_band_count_get(
    of_meter_band_stats_t *obj,
    uint64_t *byte_band_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_STATS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, byte_band_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set byte_band_count in an object of type of_meter_band_stats.
 * @param obj Pointer to an object of type of_meter_band_stats.
 * @param byte_band_count The value to write into the object
 */
void
of_meter_band_stats_byte_band_count_set(
    of_meter_band_stats_t *obj,
    uint64_t byte_band_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_BAND_STATS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, byte_band_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */
#ifdef __GNUC__

#ifdef __linux__
/* glibc */
#include <features.h>
#else
/* NetBSD etc */
#include <sys/cdefs.h>
#ifdef __GNUC_PREREQ__
#define __GNUC_PREREQ __GNUC_PREREQ__
#endif
#endif

#ifndef __GNUC_PREREQ
/* fallback */
#define __GNUC_PREREQ(maj, min) 0
#endif

#if __GNUC_PREREQ(4,6)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

#endif

#include "loci_log.h"
#include "loci_int.h"



/**
 * \defgroup of_meter_config of_meter_config
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_meter_config_push_wire_values(of_meter_config_t *obj)
{

    return OF_ERROR_NONE;
}

/**
 * Create a new of_meter_config object
 *
 * @param version The wire version to use for the object
 * @return Pointer to the newly create object or NULL on error
 *
 * Initializes the new object with it's default fixed length associating
 * a new underlying wire buffer.
 *
 * Use new_from_message to bind an existing message to a message object,
 * or a _get function for non-message objects.
 *
 * \ingroup of_meter_config
 */

of_meter_config_t *
of_meter_config_new(of_version_t version)
{
    of_meter_config_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_METER_CONFIG] + of_object_extra_len[version][OF_METER_CONFIG];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_meter_config_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_meter_config_init(obj, version, bytes, 0);

    if (of_meter_config_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_meter_config.
 *
 * @param obj Pointer to the object to initialize
 * @param version The wire version to use for the object
 * @param bytes How many bytes in the object
 * @param clean_wire Boolean: If true, clear the wire object control struct
 *
 * If bytes < 0, then the default fixed length is used for the object
 *
 * This is a "coerce" function that sets up the pointers for the
 * accessors properly.
 *
 * If anything other than 0 is passed in for the buffer size, the underlying
 * wire buffer will have 'grow' called.
 */

void
of_meter_config_init(of_meter_config_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_METER_CONFIG] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_METER_CONFIG] + of_object_extra_len[version][OF_METER_CONFIG];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_METER_CONFIG;

    /* Set up the object's function pointers */

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get flags from an object of type of_meter_config.
 * @param obj Pointer to an object of type of_meter_config.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_meter_config_flags_get(
    of_meter_config_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_CONFIG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 2;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type of_meter_config.
 * @param obj Pointer to an object of type of_meter_config.
 * @param flags The value to write into the object
 */
void
of_meter_config_flags_set(
    of_meter_config_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_CONFIG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 2;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get meter_id from an object of type of_meter_config.
 * @param obj Pointer to an object of type of_meter_config.
 * @param meter_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_config_meter_id_get(
    of_meter_config_t *obj,
    uint32_t *meter_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_CONFIG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, meter_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set meter_id in an object of type of_meter_config.
 * @param obj Pointer to an object of type of_meter_config.
 * @param meter_id The value to write into the object
 */
void
of_meter_config_meter_id_set(
    of_meter_config_t *obj,
    uint32_t meter_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_CONFIG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, meter_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Bind an object of type of_list_meter_band_t to the parent of type of_meter_config for
 * member entries
 * @param obj Pointer to an object of type of_meter_config.
 * @param entries Pointer to the child object of type
 * of_list_meter_band_t to be filled out.
 * \ingroup of_meter_config
 *
 * The parameter entries is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_meter_config_entries_bind(
    of_meter_config_t *obj,
    of_list_meter_band_t *entries)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_METER_CONFIG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_meter_band_init(entries, obj->version, 0, 1);
    /* Attach to parent */
    entries->parent = (of_object_t *)obj;
    entries->wire_object.wbuf = obj->wire_object.wbuf;
    entries->wire_object.obj_offset = abs_offset;
    entries->wire_object.owned = 0;
    entries->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of entries into a new variable of type of_list_meter_band_t from
 * a of_meter_config instance.
 *
 * @param obj Pointer to the source of type of_meter_config_t
 * @returns A pointer to a new instance of type of_list_meter_band_t whose contents
 * match that of entries from source
 * @returns NULL if an error occurs
 */
of_list_meter_band_t *
of_meter_config_entries_get(of_meter_config_t *obj) {
    of_list_meter_band_t _entries;
    of_list_meter_band_t *_entries_ptr;

    of_meter_config_entries_bind(obj, &_entries);
    _entries_ptr = (of_list_meter_band_t *)of_object_dup(&_entries);
    return _entries_ptr;
}

/**
 * Set entries in an object of type of_meter_config.
 * @param obj Pointer to an object of type of_meter_config.
 * @param entries Pointer to the child of type of_list_meter_band_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_meter_config_entries_set(
    of_meter_config_t *obj,
    of_list_meter_band_t *entries)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_METER_CONFIG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 8;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = entries->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == entries->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(entries, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(entries, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (entries->wire_length_set != NULL) {
        entries->wire_length_set((of_object_t *)entries, entries->length);
    }

    /* Not scalar, update lengths if needed */
    delta = new_len - cur_len;
    if (delta != 0) {
        /* Update parent(s) */
        of_object_parent_length_update((of_object_t *)obj, delta);
    }

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}
