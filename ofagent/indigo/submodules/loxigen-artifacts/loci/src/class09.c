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
of_action_id_group_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x16); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_group of_action_id_group
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_group_push_wire_values(of_action_id_group_t *obj)
{

    of_action_id_group_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_group object
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
 * \ingroup of_action_id_group
 */

of_action_id_group_t *
of_action_id_group_new(of_version_t version)
{
    of_action_id_group_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_GROUP] + of_object_extra_len[version][OF_ACTION_ID_GROUP];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_group_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_group_init(obj, version, bytes, 0);

    if (of_action_id_group_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_group.
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
of_action_id_group_init(of_action_id_group_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_GROUP] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_GROUP] + of_object_extra_len[version][OF_ACTION_ID_GROUP];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_GROUP;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_group_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

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
of_action_id_nicira_wire_object_id_get(of_object_t *obj, of_object_id_t *id)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3: {
        uint16_t value = U16_NTOH(*(uint16_t *)(buf + 8)); /* subtype */
        switch (value) {
        case 0x12:
            *id = OF_ACTION_ID_NICIRA_DEC_TTL;
            break;
        default:
            *id = OF_ACTION_ID_NICIRA;
            break;
        }
        break;
    }
    default:
        LOCI_ASSERT(0);
    }
}


/**
 * \defgroup of_action_id_nicira of_action_id_nicira
 */

/**
 * Create a new of_action_id_nicira object
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
 * \ingroup of_action_id_nicira
 */

of_action_id_nicira_t *
of_action_id_nicira_new(of_version_t version)
{
    of_action_id_nicira_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_NICIRA] + of_object_extra_len[version][OF_ACTION_ID_NICIRA];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_nicira_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_nicira_init(obj, version, bytes, 0);

    return obj;
}

/**
 * Initialize an object of type of_action_id_nicira.
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
of_action_id_nicira_init(of_action_id_nicira_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_NICIRA] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_NICIRA] + of_object_extra_len[version][OF_ACTION_ID_NICIRA];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_NICIRA;

    /* Set up the object's function pointers */

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_nicira.
 * @param obj Pointer to an object of type of_action_id_nicira.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_nicira_experimenter_get(
    of_action_id_nicira_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_NICIRA);
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
 * Set experimenter in an object of type of_action_id_nicira.
 * @param obj Pointer to an object of type of_action_id_nicira.
 * @param experimenter The value to write into the object
 */
void
of_action_id_nicira_experimenter_set(
    of_action_id_nicira_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_NICIRA);
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
 * Get subtype from an object of type of_action_id_nicira.
 * @param obj Pointer to an object of type of_action_id_nicira.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_nicira_subtype_get(
    of_action_id_nicira_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_NICIRA);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_nicira.
 * @param obj Pointer to an object of type of_action_id_nicira.
 * @param subtype The value to write into the object
 */
void
of_action_id_nicira_subtype_set(
    of_action_id_nicira_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_NICIRA);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_id_nicira_dec_ttl_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x2320); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x12); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_nicira_dec_ttl of_action_id_nicira_dec_ttl
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_nicira_dec_ttl_push_wire_values(of_action_id_nicira_dec_ttl_t *obj)
{

    of_action_id_nicira_dec_ttl_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_nicira_dec_ttl object
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
 * \ingroup of_action_id_nicira_dec_ttl
 */

of_action_id_nicira_dec_ttl_t *
of_action_id_nicira_dec_ttl_new(of_version_t version)
{
    of_action_id_nicira_dec_ttl_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_NICIRA_DEC_TTL] + of_object_extra_len[version][OF_ACTION_ID_NICIRA_DEC_TTL];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_nicira_dec_ttl_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_nicira_dec_ttl_init(obj, version, bytes, 0);

    if (of_action_id_nicira_dec_ttl_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_nicira_dec_ttl.
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
of_action_id_nicira_dec_ttl_init(of_action_id_nicira_dec_ttl_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_NICIRA_DEC_TTL] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_NICIRA_DEC_TTL] + of_object_extra_len[version][OF_ACTION_ID_NICIRA_DEC_TTL];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_NICIRA_DEC_TTL;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_nicira_dec_ttl_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_nicira_dec_ttl.
 * @param obj Pointer to an object of type of_action_id_nicira_dec_ttl.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_nicira_dec_ttl_experimenter_get(
    of_action_id_nicira_dec_ttl_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_NICIRA_DEC_TTL);
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
 * Set experimenter in an object of type of_action_id_nicira_dec_ttl.
 * @param obj Pointer to an object of type of_action_id_nicira_dec_ttl.
 * @param experimenter The value to write into the object
 */
void
of_action_id_nicira_dec_ttl_experimenter_set(
    of_action_id_nicira_dec_ttl_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_NICIRA_DEC_TTL);
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
 * Get subtype from an object of type of_action_id_nicira_dec_ttl.
 * @param obj Pointer to an object of type of_action_id_nicira_dec_ttl.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_nicira_dec_ttl_subtype_get(
    of_action_id_nicira_dec_ttl_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_NICIRA_DEC_TTL);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_nicira_dec_ttl.
 * @param obj Pointer to an object of type of_action_id_nicira_dec_ttl.
 * @param subtype The value to write into the object
 */
void
of_action_id_nicira_dec_ttl_subtype_set(
    of_action_id_nicira_dec_ttl_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_NICIRA_DEC_TTL);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_wire_object_id_get(of_object_t *obj, of_object_id_t *id)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3: {
        uint16_t value = U16_NTOH(*(uint16_t *)(buf + 8)); /* subtype */
        switch (value) {
        case 0x1:
            *id = OF_ACTION_OFDPA_PUSH_L2HDR;
            break;
        case 0x2:
            *id = OF_ACTION_OFDPA_POP_L2HDR;
            break;
        case 0x3:
            *id = OF_ACTION_OFDPA_PUSH_CW;
            break;
        case 0x4:
            *id = OF_ACTION_OFDPA_POP_CW;
            break;
        case 0x5:
            *id = OF_ACTION_OFDPA_COPY_TC_IN;
            break;
        case 0x6:
            *id = OF_ACTION_OFDPA_COPY_TC_OUT;
            break;
        case 0x7:
            *id = OF_ACTION_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE;
            break;
        case 0x9:
            *id = OF_ACTION_OFDPA_SET_PCPDEI_FROM_TABLE;
            break;
        case 0xa:
            *id = OF_ACTION_OFDPA_OAM_LM_TX_COUNT;
            break;
        case 0xb:
            *id = OF_ACTION_OFDPA_OAM_LM_RX_COUNT;
            break;
        case 0xc:
            *id = OF_ACTION_OFDPA_SET_COUNTER_FIELDS;
            break;
        case 0xd:
            *id = OF_ACTION_OFDPA_DEC_TTL_MTU;
            break;
        case 0xe:
            *id = OF_ACTION_OFDPA_CHECK_DROP_STATUS;
            break;
        case 0x10:
            *id = OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE;
            break;
        case 0x11:
            *id = OF_ACTION_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE;
            break;
        case 0x12:
            *id = OF_ACTION_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE;
            break;
        case 0x13:
            *id = OF_ACTION_OFDPA_CLASS_BASED_COUNT;
            break;
        case 0x14:
            *id = OF_ACTION_OFDPA_COLOR_BASED_COUNT;
            break;
        case 0x15:
            *id = OF_ACTION_OFDPA_COLOR_BASED_COUNT_1;
            break;
        case 0x16:
            *id = OF_ACTION_OFDPA_COLOR_BASED_COUNT_2;
            break;
        case 0x17:
            *id = OF_ACTION_OFDPA_COLOR_BASED_COUNT_3;
            break;
        default:
            *id = OF_ACTION_OFDPA;
            break;
        }
        break;
    }
    default:
        LOCI_ASSERT(0);
    }
}


/**
 * \defgroup of_action_ofdpa of_action_ofdpa
 */

/**
 * Create a new of_action_ofdpa object
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
 * \ingroup of_action_ofdpa
 */

of_action_ofdpa_t *
of_action_ofdpa_new(of_version_t version)
{
    of_action_ofdpa_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA] + of_object_extra_len[version][OF_ACTION_OFDPA];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_init(obj, version, bytes, 0);

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa.
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
of_action_ofdpa_init(of_action_ofdpa_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA] + of_object_extra_len[version][OF_ACTION_OFDPA];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA;

    /* Set up the object's function pointers */

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa.
 * @param obj Pointer to an object of type of_action_ofdpa.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_experimenter_get(
    of_action_ofdpa_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA);
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
 * Set experimenter in an object of type of_action_ofdpa.
 * @param obj Pointer to an object of type of_action_ofdpa.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_experimenter_set(
    of_action_ofdpa_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA);
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
 * Get subtype from an object of type of_action_ofdpa.
 * @param obj Pointer to an object of type of_action_ofdpa.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_subtype_get(
    of_action_ofdpa_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa.
 * @param obj Pointer to an object of type of_action_ofdpa.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_subtype_set(
    of_action_ofdpa_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_id_ofdpa_wire_object_id_get(of_object_t *obj, of_object_id_t *id)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3: {
        uint16_t value = U16_NTOH(*(uint16_t *)(buf + 8)); /* subtype */
        switch (value) {
        case 0x1:
            *id = OF_ACTION_ID_OFDPA_PUSH_L2HDR;
            break;
        case 0x2:
            *id = OF_ACTION_ID_OFDPA_POP_L2HDR;
            break;
        case 0x3:
            *id = OF_ACTION_ID_OFDPA_PUSH_CW;
            break;
        case 0x4:
            *id = OF_ACTION_ID_OFDPA_POP_CW;
            break;
        case 0x5:
            *id = OF_ACTION_ID_OFDPA_COPY_TC_IN;
            break;
        case 0x6:
            *id = OF_ACTION_ID_OFDPA_COPY_TC_OUT;
            break;
        case 0x7:
            *id = OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_VPN_TABLE;
            break;
        case 0x9:
            *id = OF_ACTION_ID_OFDPA_SET_PCPDEI_FROM_TABLE;
            break;
        case 0xa:
            *id = OF_ACTION_ID_OFDPA_OAM_LM_TX_COUNT;
            break;
        case 0xb:
            *id = OF_ACTION_ID_OFDPA_OAM_LM_RX_COUNT;
            break;
        case 0xc:
            *id = OF_ACTION_ID_OFDPA_SET_COUNTER_FIELDS;
            break;
        case 0xd:
            *id = OF_ACTION_ID_OFDPA_DEC_TTL_MTU;
            break;
        case 0xe:
            *id = OF_ACTION_ID_OFDPA_CHECK_DROP_STATUS;
            break;
        case 0x10:
            *id = OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TABLE;
            break;
        case 0x11:
            *id = OF_ACTION_ID_OFDPA_SET_MPLS_TC_FROM_TUNNEL_TABLE;
            break;
        case 0x12:
            *id = OF_ACTION_ID_OFDPA_SET_MPLS_PCPDEI_FROM_TUNNEL_TABLE;
            break;
        case 0x13:
            *id = OF_ACTION_ID_OFDPA_CLASS_BASED_COUNT;
            break;
        case 0x14:
            *id = OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT;
            break;
        case 0x15:
            *id = OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_1;
            break;
        case 0x16:
            *id = OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_2;
            break;
        case 0x17:
            *id = OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_3;
            break;
        default:
            *id = OF_ACTION_ID_OFDPA;
            break;
        }
        break;
    }
    default:
        LOCI_ASSERT(0);
    }
}


/**
 * \defgroup of_action_id_ofdpa of_action_id_ofdpa
 */

/**
 * Create a new of_action_id_ofdpa object
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
 * \ingroup of_action_id_ofdpa
 */

of_action_id_ofdpa_t *
of_action_id_ofdpa_new(of_version_t version)
{
    of_action_id_ofdpa_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA] + of_object_extra_len[version][OF_ACTION_ID_OFDPA];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_init(obj, version, bytes, 0);

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa.
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
of_action_id_ofdpa_init(of_action_id_ofdpa_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA] + of_object_extra_len[version][OF_ACTION_ID_OFDPA];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA;

    /* Set up the object's function pointers */

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa.
 * @param obj Pointer to an object of type of_action_id_ofdpa.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_experimenter_get(
    of_action_id_ofdpa_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA);
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
 * Set experimenter in an object of type of_action_id_ofdpa.
 * @param obj Pointer to an object of type of_action_id_ofdpa.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_experimenter_set(
    of_action_id_ofdpa_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA);
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
 * Get subtype from an object of type of_action_id_ofdpa.
 * @param obj Pointer to an object of type of_action_id_ofdpa.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_subtype_get(
    of_action_id_ofdpa_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa.
 * @param obj Pointer to an object of type of_action_id_ofdpa.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_subtype_set(
    of_action_id_ofdpa_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_check_drop_status_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0xe); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_ofdpa_check_drop_status of_action_ofdpa_check_drop_status
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_ofdpa_check_drop_status_push_wire_values(of_action_ofdpa_check_drop_status_t *obj)
{

    of_action_ofdpa_check_drop_status_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_ofdpa_check_drop_status object
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
 * \ingroup of_action_ofdpa_check_drop_status
 */

of_action_ofdpa_check_drop_status_t *
of_action_ofdpa_check_drop_status_new(of_version_t version)
{
    of_action_ofdpa_check_drop_status_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_CHECK_DROP_STATUS] + of_object_extra_len[version][OF_ACTION_OFDPA_CHECK_DROP_STATUS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_check_drop_status_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_check_drop_status_init(obj, version, bytes, 0);

    if (of_action_ofdpa_check_drop_status_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa_check_drop_status.
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
of_action_ofdpa_check_drop_status_init(of_action_ofdpa_check_drop_status_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA_CHECK_DROP_STATUS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_CHECK_DROP_STATUS] + of_object_extra_len[version][OF_ACTION_OFDPA_CHECK_DROP_STATUS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA_CHECK_DROP_STATUS;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_ofdpa_check_drop_status_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa_check_drop_status.
 * @param obj Pointer to an object of type of_action_ofdpa_check_drop_status.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_check_drop_status_experimenter_get(
    of_action_ofdpa_check_drop_status_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_CHECK_DROP_STATUS);
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
 * Set experimenter in an object of type of_action_ofdpa_check_drop_status.
 * @param obj Pointer to an object of type of_action_ofdpa_check_drop_status.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_check_drop_status_experimenter_set(
    of_action_ofdpa_check_drop_status_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_CHECK_DROP_STATUS);
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
 * Get subtype from an object of type of_action_ofdpa_check_drop_status.
 * @param obj Pointer to an object of type of_action_ofdpa_check_drop_status.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_check_drop_status_subtype_get(
    of_action_ofdpa_check_drop_status_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_CHECK_DROP_STATUS);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa_check_drop_status.
 * @param obj Pointer to an object of type of_action_ofdpa_check_drop_status.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_check_drop_status_subtype_set(
    of_action_ofdpa_check_drop_status_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_CHECK_DROP_STATUS);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get drop_type from an object of type of_action_ofdpa_check_drop_status.
 * @param obj Pointer to an object of type of_action_ofdpa_check_drop_status.
 * @param drop_type Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_check_drop_status_drop_type_get(
    of_action_ofdpa_check_drop_status_t *obj,
    uint16_t *drop_type)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_CHECK_DROP_STATUS);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, drop_type);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set drop_type in an object of type of_action_ofdpa_check_drop_status.
 * @param obj Pointer to an object of type of_action_ofdpa_check_drop_status.
 * @param drop_type The value to write into the object
 */
void
of_action_ofdpa_check_drop_status_drop_type_set(
    of_action_ofdpa_check_drop_status_t *obj,
    uint16_t drop_type)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_CHECK_DROP_STATUS);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, drop_type);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get drop_index from an object of type of_action_ofdpa_check_drop_status.
 * @param obj Pointer to an object of type of_action_ofdpa_check_drop_status.
 * @param drop_index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_check_drop_status_drop_index_get(
    of_action_ofdpa_check_drop_status_t *obj,
    uint32_t *drop_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_CHECK_DROP_STATUS);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, drop_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set drop_index in an object of type of_action_ofdpa_check_drop_status.
 * @param obj Pointer to an object of type of_action_ofdpa_check_drop_status.
 * @param drop_index The value to write into the object
 */
void
of_action_ofdpa_check_drop_status_drop_index_set(
    of_action_ofdpa_check_drop_status_t *obj,
    uint32_t drop_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_CHECK_DROP_STATUS);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, drop_index);

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
of_action_id_ofdpa_check_drop_status_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0xe); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_ofdpa_check_drop_status of_action_id_ofdpa_check_drop_status
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_ofdpa_check_drop_status_push_wire_values(of_action_id_ofdpa_check_drop_status_t *obj)
{

    of_action_id_ofdpa_check_drop_status_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_ofdpa_check_drop_status object
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
 * \ingroup of_action_id_ofdpa_check_drop_status
 */

of_action_id_ofdpa_check_drop_status_t *
of_action_id_ofdpa_check_drop_status_new(of_version_t version)
{
    of_action_id_ofdpa_check_drop_status_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_CHECK_DROP_STATUS] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_CHECK_DROP_STATUS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_check_drop_status_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_check_drop_status_init(obj, version, bytes, 0);

    if (of_action_id_ofdpa_check_drop_status_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa_check_drop_status.
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
of_action_id_ofdpa_check_drop_status_init(of_action_id_ofdpa_check_drop_status_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA_CHECK_DROP_STATUS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_CHECK_DROP_STATUS] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_CHECK_DROP_STATUS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA_CHECK_DROP_STATUS;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_ofdpa_check_drop_status_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa_check_drop_status.
 * @param obj Pointer to an object of type of_action_id_ofdpa_check_drop_status.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_check_drop_status_experimenter_get(
    of_action_id_ofdpa_check_drop_status_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_CHECK_DROP_STATUS);
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
 * Set experimenter in an object of type of_action_id_ofdpa_check_drop_status.
 * @param obj Pointer to an object of type of_action_id_ofdpa_check_drop_status.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_check_drop_status_experimenter_set(
    of_action_id_ofdpa_check_drop_status_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_CHECK_DROP_STATUS);
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
 * Get subtype from an object of type of_action_id_ofdpa_check_drop_status.
 * @param obj Pointer to an object of type of_action_id_ofdpa_check_drop_status.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_check_drop_status_subtype_get(
    of_action_id_ofdpa_check_drop_status_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_CHECK_DROP_STATUS);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa_check_drop_status.
 * @param obj Pointer to an object of type of_action_id_ofdpa_check_drop_status.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_check_drop_status_subtype_set(
    of_action_id_ofdpa_check_drop_status_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_CHECK_DROP_STATUS);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_class_based_count_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x13); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_ofdpa_class_based_count of_action_ofdpa_class_based_count
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_ofdpa_class_based_count_push_wire_values(of_action_ofdpa_class_based_count_t *obj)
{

    of_action_ofdpa_class_based_count_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_ofdpa_class_based_count object
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
 * \ingroup of_action_ofdpa_class_based_count
 */

of_action_ofdpa_class_based_count_t *
of_action_ofdpa_class_based_count_new(of_version_t version)
{
    of_action_ofdpa_class_based_count_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_CLASS_BASED_COUNT] + of_object_extra_len[version][OF_ACTION_OFDPA_CLASS_BASED_COUNT];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_class_based_count_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_class_based_count_init(obj, version, bytes, 0);

    if (of_action_ofdpa_class_based_count_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa_class_based_count.
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
of_action_ofdpa_class_based_count_init(of_action_ofdpa_class_based_count_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA_CLASS_BASED_COUNT] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_CLASS_BASED_COUNT] + of_object_extra_len[version][OF_ACTION_OFDPA_CLASS_BASED_COUNT];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA_CLASS_BASED_COUNT;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_ofdpa_class_based_count_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa_class_based_count.
 * @param obj Pointer to an object of type of_action_ofdpa_class_based_count.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_class_based_count_experimenter_get(
    of_action_ofdpa_class_based_count_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_CLASS_BASED_COUNT);
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
 * Set experimenter in an object of type of_action_ofdpa_class_based_count.
 * @param obj Pointer to an object of type of_action_ofdpa_class_based_count.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_class_based_count_experimenter_set(
    of_action_ofdpa_class_based_count_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_CLASS_BASED_COUNT);
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
 * Get subtype from an object of type of_action_ofdpa_class_based_count.
 * @param obj Pointer to an object of type of_action_ofdpa_class_based_count.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_class_based_count_subtype_get(
    of_action_ofdpa_class_based_count_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_CLASS_BASED_COUNT);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa_class_based_count.
 * @param obj Pointer to an object of type of_action_ofdpa_class_based_count.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_class_based_count_subtype_set(
    of_action_ofdpa_class_based_count_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_CLASS_BASED_COUNT);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type of_action_ofdpa_class_based_count.
 * @param obj Pointer to an object of type of_action_ofdpa_class_based_count.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_class_based_count_index_get(
    of_action_ofdpa_class_based_count_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_CLASS_BASED_COUNT);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set index in an object of type of_action_ofdpa_class_based_count.
 * @param obj Pointer to an object of type of_action_ofdpa_class_based_count.
 * @param index The value to write into the object
 */
void
of_action_ofdpa_class_based_count_index_set(
    of_action_ofdpa_class_based_count_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_CLASS_BASED_COUNT);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, index);

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
of_action_id_ofdpa_class_based_count_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x13); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_ofdpa_class_based_count of_action_id_ofdpa_class_based_count
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_ofdpa_class_based_count_push_wire_values(of_action_id_ofdpa_class_based_count_t *obj)
{

    of_action_id_ofdpa_class_based_count_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_ofdpa_class_based_count object
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
 * \ingroup of_action_id_ofdpa_class_based_count
 */

of_action_id_ofdpa_class_based_count_t *
of_action_id_ofdpa_class_based_count_new(of_version_t version)
{
    of_action_id_ofdpa_class_based_count_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_CLASS_BASED_COUNT] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_CLASS_BASED_COUNT];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_class_based_count_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_class_based_count_init(obj, version, bytes, 0);

    if (of_action_id_ofdpa_class_based_count_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa_class_based_count.
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
of_action_id_ofdpa_class_based_count_init(of_action_id_ofdpa_class_based_count_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA_CLASS_BASED_COUNT] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_CLASS_BASED_COUNT] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_CLASS_BASED_COUNT];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA_CLASS_BASED_COUNT;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_ofdpa_class_based_count_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa_class_based_count.
 * @param obj Pointer to an object of type of_action_id_ofdpa_class_based_count.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_class_based_count_experimenter_get(
    of_action_id_ofdpa_class_based_count_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_CLASS_BASED_COUNT);
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
 * Set experimenter in an object of type of_action_id_ofdpa_class_based_count.
 * @param obj Pointer to an object of type of_action_id_ofdpa_class_based_count.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_class_based_count_experimenter_set(
    of_action_id_ofdpa_class_based_count_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_CLASS_BASED_COUNT);
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
 * Get subtype from an object of type of_action_id_ofdpa_class_based_count.
 * @param obj Pointer to an object of type of_action_id_ofdpa_class_based_count.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_class_based_count_subtype_get(
    of_action_id_ofdpa_class_based_count_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_CLASS_BASED_COUNT);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa_class_based_count.
 * @param obj Pointer to an object of type of_action_id_ofdpa_class_based_count.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_class_based_count_subtype_set(
    of_action_id_ofdpa_class_based_count_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_CLASS_BASED_COUNT);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_color_based_count_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x14); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_ofdpa_color_based_count of_action_ofdpa_color_based_count
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_ofdpa_color_based_count_push_wire_values(of_action_ofdpa_color_based_count_t *obj)
{

    of_action_ofdpa_color_based_count_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_ofdpa_color_based_count object
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
 * \ingroup of_action_ofdpa_color_based_count
 */

of_action_ofdpa_color_based_count_t *
of_action_ofdpa_color_based_count_new(of_version_t version)
{
    of_action_ofdpa_color_based_count_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_COLOR_BASED_COUNT] + of_object_extra_len[version][OF_ACTION_OFDPA_COLOR_BASED_COUNT];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_color_based_count_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_color_based_count_init(obj, version, bytes, 0);

    if (of_action_ofdpa_color_based_count_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa_color_based_count.
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
of_action_ofdpa_color_based_count_init(of_action_ofdpa_color_based_count_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA_COLOR_BASED_COUNT] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_COLOR_BASED_COUNT] + of_object_extra_len[version][OF_ACTION_OFDPA_COLOR_BASED_COUNT];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA_COLOR_BASED_COUNT;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_ofdpa_color_based_count_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa_color_based_count.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_color_based_count_experimenter_get(
    of_action_ofdpa_color_based_count_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT);
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
 * Set experimenter in an object of type of_action_ofdpa_color_based_count.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_color_based_count_experimenter_set(
    of_action_ofdpa_color_based_count_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT);
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
 * Get subtype from an object of type of_action_ofdpa_color_based_count.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_color_based_count_subtype_get(
    of_action_ofdpa_color_based_count_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa_color_based_count.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_color_based_count_subtype_set(
    of_action_ofdpa_color_based_count_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type of_action_ofdpa_color_based_count.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_color_based_count_index_get(
    of_action_ofdpa_color_based_count_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set index in an object of type of_action_ofdpa_color_based_count.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count.
 * @param index The value to write into the object
 */
void
of_action_ofdpa_color_based_count_index_set(
    of_action_ofdpa_color_based_count_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, index);

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
of_action_id_ofdpa_color_based_count_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x14); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_ofdpa_color_based_count of_action_id_ofdpa_color_based_count
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_ofdpa_color_based_count_push_wire_values(of_action_id_ofdpa_color_based_count_t *obj)
{

    of_action_id_ofdpa_color_based_count_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_ofdpa_color_based_count object
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
 * \ingroup of_action_id_ofdpa_color_based_count
 */

of_action_id_ofdpa_color_based_count_t *
of_action_id_ofdpa_color_based_count_new(of_version_t version)
{
    of_action_id_ofdpa_color_based_count_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_color_based_count_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_color_based_count_init(obj, version, bytes, 0);

    if (of_action_id_ofdpa_color_based_count_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa_color_based_count.
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
of_action_id_ofdpa_color_based_count_init(of_action_id_ofdpa_color_based_count_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_ofdpa_color_based_count_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa_color_based_count.
 * @param obj Pointer to an object of type of_action_id_ofdpa_color_based_count.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_color_based_count_experimenter_get(
    of_action_id_ofdpa_color_based_count_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT);
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
 * Set experimenter in an object of type of_action_id_ofdpa_color_based_count.
 * @param obj Pointer to an object of type of_action_id_ofdpa_color_based_count.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_color_based_count_experimenter_set(
    of_action_id_ofdpa_color_based_count_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT);
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
 * Get subtype from an object of type of_action_id_ofdpa_color_based_count.
 * @param obj Pointer to an object of type of_action_id_ofdpa_color_based_count.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_color_based_count_subtype_get(
    of_action_id_ofdpa_color_based_count_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa_color_based_count.
 * @param obj Pointer to an object of type of_action_id_ofdpa_color_based_count.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_color_based_count_subtype_set(
    of_action_id_ofdpa_color_based_count_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_color_based_count_1_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x15); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_ofdpa_color_based_count_1 of_action_ofdpa_color_based_count_1
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_ofdpa_color_based_count_1_push_wire_values(of_action_ofdpa_color_based_count_1_t *obj)
{

    of_action_ofdpa_color_based_count_1_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_ofdpa_color_based_count_1 object
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
 * \ingroup of_action_ofdpa_color_based_count_1
 */

of_action_ofdpa_color_based_count_1_t *
of_action_ofdpa_color_based_count_1_new(of_version_t version)
{
    of_action_ofdpa_color_based_count_1_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_COLOR_BASED_COUNT_1] + of_object_extra_len[version][OF_ACTION_OFDPA_COLOR_BASED_COUNT_1];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_color_based_count_1_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_color_based_count_1_init(obj, version, bytes, 0);

    if (of_action_ofdpa_color_based_count_1_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa_color_based_count_1.
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
of_action_ofdpa_color_based_count_1_init(of_action_ofdpa_color_based_count_1_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA_COLOR_BASED_COUNT_1] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_COLOR_BASED_COUNT_1] + of_object_extra_len[version][OF_ACTION_OFDPA_COLOR_BASED_COUNT_1];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA_COLOR_BASED_COUNT_1;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_ofdpa_color_based_count_1_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa_color_based_count_1.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count_1.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_color_based_count_1_experimenter_get(
    of_action_ofdpa_color_based_count_1_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT_1);
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
 * Set experimenter in an object of type of_action_ofdpa_color_based_count_1.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count_1.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_color_based_count_1_experimenter_set(
    of_action_ofdpa_color_based_count_1_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT_1);
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
 * Get subtype from an object of type of_action_ofdpa_color_based_count_1.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count_1.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_color_based_count_1_subtype_get(
    of_action_ofdpa_color_based_count_1_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT_1);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa_color_based_count_1.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count_1.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_color_based_count_1_subtype_set(
    of_action_ofdpa_color_based_count_1_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT_1);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type of_action_ofdpa_color_based_count_1.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count_1.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_color_based_count_1_index_get(
    of_action_ofdpa_color_based_count_1_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT_1);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set index in an object of type of_action_ofdpa_color_based_count_1.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count_1.
 * @param index The value to write into the object
 */
void
of_action_ofdpa_color_based_count_1_index_set(
    of_action_ofdpa_color_based_count_1_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT_1);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, index);

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
of_action_id_ofdpa_color_based_count_1_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x15); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_ofdpa_color_based_count_1 of_action_id_ofdpa_color_based_count_1
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_ofdpa_color_based_count_1_push_wire_values(of_action_id_ofdpa_color_based_count_1_t *obj)
{

    of_action_id_ofdpa_color_based_count_1_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_ofdpa_color_based_count_1 object
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
 * \ingroup of_action_id_ofdpa_color_based_count_1
 */

of_action_id_ofdpa_color_based_count_1_t *
of_action_id_ofdpa_color_based_count_1_new(of_version_t version)
{
    of_action_id_ofdpa_color_based_count_1_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_1] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_1];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_color_based_count_1_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_color_based_count_1_init(obj, version, bytes, 0);

    if (of_action_id_ofdpa_color_based_count_1_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa_color_based_count_1.
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
of_action_id_ofdpa_color_based_count_1_init(of_action_id_ofdpa_color_based_count_1_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_1] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_1] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_1];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_1;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_ofdpa_color_based_count_1_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa_color_based_count_1.
 * @param obj Pointer to an object of type of_action_id_ofdpa_color_based_count_1.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_color_based_count_1_experimenter_get(
    of_action_id_ofdpa_color_based_count_1_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_1);
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
 * Set experimenter in an object of type of_action_id_ofdpa_color_based_count_1.
 * @param obj Pointer to an object of type of_action_id_ofdpa_color_based_count_1.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_color_based_count_1_experimenter_set(
    of_action_id_ofdpa_color_based_count_1_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_1);
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
 * Get subtype from an object of type of_action_id_ofdpa_color_based_count_1.
 * @param obj Pointer to an object of type of_action_id_ofdpa_color_based_count_1.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_color_based_count_1_subtype_get(
    of_action_id_ofdpa_color_based_count_1_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_1);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa_color_based_count_1.
 * @param obj Pointer to an object of type of_action_id_ofdpa_color_based_count_1.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_color_based_count_1_subtype_set(
    of_action_id_ofdpa_color_based_count_1_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_1);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_color_based_count_2_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x16); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_ofdpa_color_based_count_2 of_action_ofdpa_color_based_count_2
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_ofdpa_color_based_count_2_push_wire_values(of_action_ofdpa_color_based_count_2_t *obj)
{

    of_action_ofdpa_color_based_count_2_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_ofdpa_color_based_count_2 object
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
 * \ingroup of_action_ofdpa_color_based_count_2
 */

of_action_ofdpa_color_based_count_2_t *
of_action_ofdpa_color_based_count_2_new(of_version_t version)
{
    of_action_ofdpa_color_based_count_2_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_COLOR_BASED_COUNT_2] + of_object_extra_len[version][OF_ACTION_OFDPA_COLOR_BASED_COUNT_2];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_color_based_count_2_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_color_based_count_2_init(obj, version, bytes, 0);

    if (of_action_ofdpa_color_based_count_2_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa_color_based_count_2.
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
of_action_ofdpa_color_based_count_2_init(of_action_ofdpa_color_based_count_2_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA_COLOR_BASED_COUNT_2] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_COLOR_BASED_COUNT_2] + of_object_extra_len[version][OF_ACTION_OFDPA_COLOR_BASED_COUNT_2];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA_COLOR_BASED_COUNT_2;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_ofdpa_color_based_count_2_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa_color_based_count_2.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count_2.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_color_based_count_2_experimenter_get(
    of_action_ofdpa_color_based_count_2_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT_2);
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
 * Set experimenter in an object of type of_action_ofdpa_color_based_count_2.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count_2.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_color_based_count_2_experimenter_set(
    of_action_ofdpa_color_based_count_2_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT_2);
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
 * Get subtype from an object of type of_action_ofdpa_color_based_count_2.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count_2.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_color_based_count_2_subtype_get(
    of_action_ofdpa_color_based_count_2_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT_2);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa_color_based_count_2.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count_2.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_color_based_count_2_subtype_set(
    of_action_ofdpa_color_based_count_2_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT_2);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type of_action_ofdpa_color_based_count_2.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count_2.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_color_based_count_2_index_get(
    of_action_ofdpa_color_based_count_2_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT_2);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set index in an object of type of_action_ofdpa_color_based_count_2.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count_2.
 * @param index The value to write into the object
 */
void
of_action_ofdpa_color_based_count_2_index_set(
    of_action_ofdpa_color_based_count_2_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT_2);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, index);

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
of_action_id_ofdpa_color_based_count_2_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x16); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_ofdpa_color_based_count_2 of_action_id_ofdpa_color_based_count_2
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_ofdpa_color_based_count_2_push_wire_values(of_action_id_ofdpa_color_based_count_2_t *obj)
{

    of_action_id_ofdpa_color_based_count_2_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_ofdpa_color_based_count_2 object
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
 * \ingroup of_action_id_ofdpa_color_based_count_2
 */

of_action_id_ofdpa_color_based_count_2_t *
of_action_id_ofdpa_color_based_count_2_new(of_version_t version)
{
    of_action_id_ofdpa_color_based_count_2_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_2] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_2];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_color_based_count_2_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_color_based_count_2_init(obj, version, bytes, 0);

    if (of_action_id_ofdpa_color_based_count_2_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa_color_based_count_2.
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
of_action_id_ofdpa_color_based_count_2_init(of_action_id_ofdpa_color_based_count_2_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_2] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_2] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_2];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_2;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_ofdpa_color_based_count_2_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa_color_based_count_2.
 * @param obj Pointer to an object of type of_action_id_ofdpa_color_based_count_2.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_color_based_count_2_experimenter_get(
    of_action_id_ofdpa_color_based_count_2_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_2);
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
 * Set experimenter in an object of type of_action_id_ofdpa_color_based_count_2.
 * @param obj Pointer to an object of type of_action_id_ofdpa_color_based_count_2.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_color_based_count_2_experimenter_set(
    of_action_id_ofdpa_color_based_count_2_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_2);
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
 * Get subtype from an object of type of_action_id_ofdpa_color_based_count_2.
 * @param obj Pointer to an object of type of_action_id_ofdpa_color_based_count_2.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_color_based_count_2_subtype_get(
    of_action_id_ofdpa_color_based_count_2_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_2);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa_color_based_count_2.
 * @param obj Pointer to an object of type of_action_id_ofdpa_color_based_count_2.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_color_based_count_2_subtype_set(
    of_action_id_ofdpa_color_based_count_2_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_2);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_color_based_count_3_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x17); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_ofdpa_color_based_count_3 of_action_ofdpa_color_based_count_3
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_ofdpa_color_based_count_3_push_wire_values(of_action_ofdpa_color_based_count_3_t *obj)
{

    of_action_ofdpa_color_based_count_3_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_ofdpa_color_based_count_3 object
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
 * \ingroup of_action_ofdpa_color_based_count_3
 */

of_action_ofdpa_color_based_count_3_t *
of_action_ofdpa_color_based_count_3_new(of_version_t version)
{
    of_action_ofdpa_color_based_count_3_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_COLOR_BASED_COUNT_3] + of_object_extra_len[version][OF_ACTION_OFDPA_COLOR_BASED_COUNT_3];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_color_based_count_3_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_color_based_count_3_init(obj, version, bytes, 0);

    if (of_action_ofdpa_color_based_count_3_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa_color_based_count_3.
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
of_action_ofdpa_color_based_count_3_init(of_action_ofdpa_color_based_count_3_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA_COLOR_BASED_COUNT_3] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_COLOR_BASED_COUNT_3] + of_object_extra_len[version][OF_ACTION_OFDPA_COLOR_BASED_COUNT_3];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA_COLOR_BASED_COUNT_3;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_ofdpa_color_based_count_3_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa_color_based_count_3.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count_3.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_color_based_count_3_experimenter_get(
    of_action_ofdpa_color_based_count_3_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT_3);
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
 * Set experimenter in an object of type of_action_ofdpa_color_based_count_3.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count_3.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_color_based_count_3_experimenter_set(
    of_action_ofdpa_color_based_count_3_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT_3);
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
 * Get subtype from an object of type of_action_ofdpa_color_based_count_3.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count_3.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_color_based_count_3_subtype_get(
    of_action_ofdpa_color_based_count_3_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT_3);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa_color_based_count_3.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count_3.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_color_based_count_3_subtype_set(
    of_action_ofdpa_color_based_count_3_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT_3);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type of_action_ofdpa_color_based_count_3.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count_3.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_color_based_count_3_index_get(
    of_action_ofdpa_color_based_count_3_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT_3);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set index in an object of type of_action_ofdpa_color_based_count_3.
 * @param obj Pointer to an object of type of_action_ofdpa_color_based_count_3.
 * @param index The value to write into the object
 */
void
of_action_ofdpa_color_based_count_3_index_set(
    of_action_ofdpa_color_based_count_3_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COLOR_BASED_COUNT_3);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, index);

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
of_action_id_ofdpa_color_based_count_3_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x17); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_ofdpa_color_based_count_3 of_action_id_ofdpa_color_based_count_3
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_ofdpa_color_based_count_3_push_wire_values(of_action_id_ofdpa_color_based_count_3_t *obj)
{

    of_action_id_ofdpa_color_based_count_3_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_ofdpa_color_based_count_3 object
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
 * \ingroup of_action_id_ofdpa_color_based_count_3
 */

of_action_id_ofdpa_color_based_count_3_t *
of_action_id_ofdpa_color_based_count_3_new(of_version_t version)
{
    of_action_id_ofdpa_color_based_count_3_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_3] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_3];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_color_based_count_3_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_color_based_count_3_init(obj, version, bytes, 0);

    if (of_action_id_ofdpa_color_based_count_3_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa_color_based_count_3.
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
of_action_id_ofdpa_color_based_count_3_init(of_action_id_ofdpa_color_based_count_3_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_3] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_3] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_3];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_3;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_ofdpa_color_based_count_3_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa_color_based_count_3.
 * @param obj Pointer to an object of type of_action_id_ofdpa_color_based_count_3.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_color_based_count_3_experimenter_get(
    of_action_id_ofdpa_color_based_count_3_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_3);
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
 * Set experimenter in an object of type of_action_id_ofdpa_color_based_count_3.
 * @param obj Pointer to an object of type of_action_id_ofdpa_color_based_count_3.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_color_based_count_3_experimenter_set(
    of_action_id_ofdpa_color_based_count_3_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_3);
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
 * Get subtype from an object of type of_action_id_ofdpa_color_based_count_3.
 * @param obj Pointer to an object of type of_action_id_ofdpa_color_based_count_3.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_color_based_count_3_subtype_get(
    of_action_id_ofdpa_color_based_count_3_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_3);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa_color_based_count_3.
 * @param obj Pointer to an object of type of_action_id_ofdpa_color_based_count_3.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_color_based_count_3_subtype_set(
    of_action_id_ofdpa_color_based_count_3_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COLOR_BASED_COUNT_3);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_copy_tc_in_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x5); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_ofdpa_copy_tc_in of_action_ofdpa_copy_tc_in
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_ofdpa_copy_tc_in_push_wire_values(of_action_ofdpa_copy_tc_in_t *obj)
{

    of_action_ofdpa_copy_tc_in_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_ofdpa_copy_tc_in object
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
 * \ingroup of_action_ofdpa_copy_tc_in
 */

of_action_ofdpa_copy_tc_in_t *
of_action_ofdpa_copy_tc_in_new(of_version_t version)
{
    of_action_ofdpa_copy_tc_in_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_COPY_TC_IN] + of_object_extra_len[version][OF_ACTION_OFDPA_COPY_TC_IN];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_copy_tc_in_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_copy_tc_in_init(obj, version, bytes, 0);

    if (of_action_ofdpa_copy_tc_in_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa_copy_tc_in.
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
of_action_ofdpa_copy_tc_in_init(of_action_ofdpa_copy_tc_in_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA_COPY_TC_IN] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_COPY_TC_IN] + of_object_extra_len[version][OF_ACTION_OFDPA_COPY_TC_IN];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA_COPY_TC_IN;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_ofdpa_copy_tc_in_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa_copy_tc_in.
 * @param obj Pointer to an object of type of_action_ofdpa_copy_tc_in.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_copy_tc_in_experimenter_get(
    of_action_ofdpa_copy_tc_in_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COPY_TC_IN);
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
 * Set experimenter in an object of type of_action_ofdpa_copy_tc_in.
 * @param obj Pointer to an object of type of_action_ofdpa_copy_tc_in.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_copy_tc_in_experimenter_set(
    of_action_ofdpa_copy_tc_in_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COPY_TC_IN);
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
 * Get subtype from an object of type of_action_ofdpa_copy_tc_in.
 * @param obj Pointer to an object of type of_action_ofdpa_copy_tc_in.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_copy_tc_in_subtype_get(
    of_action_ofdpa_copy_tc_in_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COPY_TC_IN);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa_copy_tc_in.
 * @param obj Pointer to an object of type of_action_ofdpa_copy_tc_in.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_copy_tc_in_subtype_set(
    of_action_ofdpa_copy_tc_in_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COPY_TC_IN);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_id_ofdpa_copy_tc_in_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x5); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_ofdpa_copy_tc_in of_action_id_ofdpa_copy_tc_in
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_ofdpa_copy_tc_in_push_wire_values(of_action_id_ofdpa_copy_tc_in_t *obj)
{

    of_action_id_ofdpa_copy_tc_in_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_ofdpa_copy_tc_in object
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
 * \ingroup of_action_id_ofdpa_copy_tc_in
 */

of_action_id_ofdpa_copy_tc_in_t *
of_action_id_ofdpa_copy_tc_in_new(of_version_t version)
{
    of_action_id_ofdpa_copy_tc_in_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_COPY_TC_IN] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_COPY_TC_IN];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_copy_tc_in_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_copy_tc_in_init(obj, version, bytes, 0);

    if (of_action_id_ofdpa_copy_tc_in_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa_copy_tc_in.
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
of_action_id_ofdpa_copy_tc_in_init(of_action_id_ofdpa_copy_tc_in_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA_COPY_TC_IN] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_COPY_TC_IN] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_COPY_TC_IN];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA_COPY_TC_IN;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_ofdpa_copy_tc_in_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa_copy_tc_in.
 * @param obj Pointer to an object of type of_action_id_ofdpa_copy_tc_in.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_copy_tc_in_experimenter_get(
    of_action_id_ofdpa_copy_tc_in_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COPY_TC_IN);
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
 * Set experimenter in an object of type of_action_id_ofdpa_copy_tc_in.
 * @param obj Pointer to an object of type of_action_id_ofdpa_copy_tc_in.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_copy_tc_in_experimenter_set(
    of_action_id_ofdpa_copy_tc_in_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COPY_TC_IN);
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
 * Get subtype from an object of type of_action_id_ofdpa_copy_tc_in.
 * @param obj Pointer to an object of type of_action_id_ofdpa_copy_tc_in.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_copy_tc_in_subtype_get(
    of_action_id_ofdpa_copy_tc_in_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COPY_TC_IN);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa_copy_tc_in.
 * @param obj Pointer to an object of type of_action_id_ofdpa_copy_tc_in.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_copy_tc_in_subtype_set(
    of_action_id_ofdpa_copy_tc_in_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COPY_TC_IN);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_copy_tc_out_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x6); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_ofdpa_copy_tc_out of_action_ofdpa_copy_tc_out
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_ofdpa_copy_tc_out_push_wire_values(of_action_ofdpa_copy_tc_out_t *obj)
{

    of_action_ofdpa_copy_tc_out_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_ofdpa_copy_tc_out object
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
 * \ingroup of_action_ofdpa_copy_tc_out
 */

of_action_ofdpa_copy_tc_out_t *
of_action_ofdpa_copy_tc_out_new(of_version_t version)
{
    of_action_ofdpa_copy_tc_out_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_COPY_TC_OUT] + of_object_extra_len[version][OF_ACTION_OFDPA_COPY_TC_OUT];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_copy_tc_out_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_copy_tc_out_init(obj, version, bytes, 0);

    if (of_action_ofdpa_copy_tc_out_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa_copy_tc_out.
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
of_action_ofdpa_copy_tc_out_init(of_action_ofdpa_copy_tc_out_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA_COPY_TC_OUT] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_COPY_TC_OUT] + of_object_extra_len[version][OF_ACTION_OFDPA_COPY_TC_OUT];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA_COPY_TC_OUT;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_ofdpa_copy_tc_out_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa_copy_tc_out.
 * @param obj Pointer to an object of type of_action_ofdpa_copy_tc_out.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_copy_tc_out_experimenter_get(
    of_action_ofdpa_copy_tc_out_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COPY_TC_OUT);
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
 * Set experimenter in an object of type of_action_ofdpa_copy_tc_out.
 * @param obj Pointer to an object of type of_action_ofdpa_copy_tc_out.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_copy_tc_out_experimenter_set(
    of_action_ofdpa_copy_tc_out_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COPY_TC_OUT);
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
 * Get subtype from an object of type of_action_ofdpa_copy_tc_out.
 * @param obj Pointer to an object of type of_action_ofdpa_copy_tc_out.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_copy_tc_out_subtype_get(
    of_action_ofdpa_copy_tc_out_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COPY_TC_OUT);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa_copy_tc_out.
 * @param obj Pointer to an object of type of_action_ofdpa_copy_tc_out.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_copy_tc_out_subtype_set(
    of_action_ofdpa_copy_tc_out_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_COPY_TC_OUT);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_id_ofdpa_copy_tc_out_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x6); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_ofdpa_copy_tc_out of_action_id_ofdpa_copy_tc_out
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_ofdpa_copy_tc_out_push_wire_values(of_action_id_ofdpa_copy_tc_out_t *obj)
{

    of_action_id_ofdpa_copy_tc_out_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_ofdpa_copy_tc_out object
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
 * \ingroup of_action_id_ofdpa_copy_tc_out
 */

of_action_id_ofdpa_copy_tc_out_t *
of_action_id_ofdpa_copy_tc_out_new(of_version_t version)
{
    of_action_id_ofdpa_copy_tc_out_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_COPY_TC_OUT] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_COPY_TC_OUT];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_copy_tc_out_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_copy_tc_out_init(obj, version, bytes, 0);

    if (of_action_id_ofdpa_copy_tc_out_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa_copy_tc_out.
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
of_action_id_ofdpa_copy_tc_out_init(of_action_id_ofdpa_copy_tc_out_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA_COPY_TC_OUT] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_COPY_TC_OUT] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_COPY_TC_OUT];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA_COPY_TC_OUT;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_ofdpa_copy_tc_out_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa_copy_tc_out.
 * @param obj Pointer to an object of type of_action_id_ofdpa_copy_tc_out.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_copy_tc_out_experimenter_get(
    of_action_id_ofdpa_copy_tc_out_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COPY_TC_OUT);
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
 * Set experimenter in an object of type of_action_id_ofdpa_copy_tc_out.
 * @param obj Pointer to an object of type of_action_id_ofdpa_copy_tc_out.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_copy_tc_out_experimenter_set(
    of_action_id_ofdpa_copy_tc_out_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COPY_TC_OUT);
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
 * Get subtype from an object of type of_action_id_ofdpa_copy_tc_out.
 * @param obj Pointer to an object of type of_action_id_ofdpa_copy_tc_out.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_copy_tc_out_subtype_get(
    of_action_id_ofdpa_copy_tc_out_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COPY_TC_OUT);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa_copy_tc_out.
 * @param obj Pointer to an object of type of_action_id_ofdpa_copy_tc_out.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_copy_tc_out_subtype_set(
    of_action_id_ofdpa_copy_tc_out_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_COPY_TC_OUT);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_dec_ttl_mtu_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0xd); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_ofdpa_dec_ttl_mtu of_action_ofdpa_dec_ttl_mtu
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_ofdpa_dec_ttl_mtu_push_wire_values(of_action_ofdpa_dec_ttl_mtu_t *obj)
{

    of_action_ofdpa_dec_ttl_mtu_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_ofdpa_dec_ttl_mtu object
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
 * \ingroup of_action_ofdpa_dec_ttl_mtu
 */

of_action_ofdpa_dec_ttl_mtu_t *
of_action_ofdpa_dec_ttl_mtu_new(of_version_t version)
{
    of_action_ofdpa_dec_ttl_mtu_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_DEC_TTL_MTU] + of_object_extra_len[version][OF_ACTION_OFDPA_DEC_TTL_MTU];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_dec_ttl_mtu_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_dec_ttl_mtu_init(obj, version, bytes, 0);

    if (of_action_ofdpa_dec_ttl_mtu_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa_dec_ttl_mtu.
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
of_action_ofdpa_dec_ttl_mtu_init(of_action_ofdpa_dec_ttl_mtu_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA_DEC_TTL_MTU] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_DEC_TTL_MTU] + of_object_extra_len[version][OF_ACTION_OFDPA_DEC_TTL_MTU];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA_DEC_TTL_MTU;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_ofdpa_dec_ttl_mtu_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa_dec_ttl_mtu.
 * @param obj Pointer to an object of type of_action_ofdpa_dec_ttl_mtu.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_dec_ttl_mtu_experimenter_get(
    of_action_ofdpa_dec_ttl_mtu_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_DEC_TTL_MTU);
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
 * Set experimenter in an object of type of_action_ofdpa_dec_ttl_mtu.
 * @param obj Pointer to an object of type of_action_ofdpa_dec_ttl_mtu.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_dec_ttl_mtu_experimenter_set(
    of_action_ofdpa_dec_ttl_mtu_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_DEC_TTL_MTU);
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
 * Get subtype from an object of type of_action_ofdpa_dec_ttl_mtu.
 * @param obj Pointer to an object of type of_action_ofdpa_dec_ttl_mtu.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_dec_ttl_mtu_subtype_get(
    of_action_ofdpa_dec_ttl_mtu_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_DEC_TTL_MTU);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa_dec_ttl_mtu.
 * @param obj Pointer to an object of type of_action_ofdpa_dec_ttl_mtu.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_dec_ttl_mtu_subtype_set(
    of_action_ofdpa_dec_ttl_mtu_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_DEC_TTL_MTU);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_id_ofdpa_dec_ttl_mtu_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0xd); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_ofdpa_dec_ttl_mtu of_action_id_ofdpa_dec_ttl_mtu
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_ofdpa_dec_ttl_mtu_push_wire_values(of_action_id_ofdpa_dec_ttl_mtu_t *obj)
{

    of_action_id_ofdpa_dec_ttl_mtu_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_ofdpa_dec_ttl_mtu object
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
 * \ingroup of_action_id_ofdpa_dec_ttl_mtu
 */

of_action_id_ofdpa_dec_ttl_mtu_t *
of_action_id_ofdpa_dec_ttl_mtu_new(of_version_t version)
{
    of_action_id_ofdpa_dec_ttl_mtu_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_DEC_TTL_MTU] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_DEC_TTL_MTU];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_dec_ttl_mtu_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_dec_ttl_mtu_init(obj, version, bytes, 0);

    if (of_action_id_ofdpa_dec_ttl_mtu_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa_dec_ttl_mtu.
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
of_action_id_ofdpa_dec_ttl_mtu_init(of_action_id_ofdpa_dec_ttl_mtu_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA_DEC_TTL_MTU] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_DEC_TTL_MTU] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_DEC_TTL_MTU];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA_DEC_TTL_MTU;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_ofdpa_dec_ttl_mtu_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa_dec_ttl_mtu.
 * @param obj Pointer to an object of type of_action_id_ofdpa_dec_ttl_mtu.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_dec_ttl_mtu_experimenter_get(
    of_action_id_ofdpa_dec_ttl_mtu_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_DEC_TTL_MTU);
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
 * Set experimenter in an object of type of_action_id_ofdpa_dec_ttl_mtu.
 * @param obj Pointer to an object of type of_action_id_ofdpa_dec_ttl_mtu.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_dec_ttl_mtu_experimenter_set(
    of_action_id_ofdpa_dec_ttl_mtu_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_DEC_TTL_MTU);
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
 * Get subtype from an object of type of_action_id_ofdpa_dec_ttl_mtu.
 * @param obj Pointer to an object of type of_action_id_ofdpa_dec_ttl_mtu.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_dec_ttl_mtu_subtype_get(
    of_action_id_ofdpa_dec_ttl_mtu_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_DEC_TTL_MTU);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa_dec_ttl_mtu.
 * @param obj Pointer to an object of type of_action_id_ofdpa_dec_ttl_mtu.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_dec_ttl_mtu_subtype_set(
    of_action_id_ofdpa_dec_ttl_mtu_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_DEC_TTL_MTU);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_oam_lm_rx_count_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0xb); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_ofdpa_oam_lm_rx_count of_action_ofdpa_oam_lm_rx_count
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_ofdpa_oam_lm_rx_count_push_wire_values(of_action_ofdpa_oam_lm_rx_count_t *obj)
{

    of_action_ofdpa_oam_lm_rx_count_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_ofdpa_oam_lm_rx_count object
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
 * \ingroup of_action_ofdpa_oam_lm_rx_count
 */

of_action_ofdpa_oam_lm_rx_count_t *
of_action_ofdpa_oam_lm_rx_count_new(of_version_t version)
{
    of_action_ofdpa_oam_lm_rx_count_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_OAM_LM_RX_COUNT] + of_object_extra_len[version][OF_ACTION_OFDPA_OAM_LM_RX_COUNT];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_oam_lm_rx_count_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_oam_lm_rx_count_init(obj, version, bytes, 0);

    if (of_action_ofdpa_oam_lm_rx_count_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa_oam_lm_rx_count.
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
of_action_ofdpa_oam_lm_rx_count_init(of_action_ofdpa_oam_lm_rx_count_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA_OAM_LM_RX_COUNT] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_OAM_LM_RX_COUNT] + of_object_extra_len[version][OF_ACTION_OFDPA_OAM_LM_RX_COUNT];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA_OAM_LM_RX_COUNT;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_ofdpa_oam_lm_rx_count_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa_oam_lm_rx_count.
 * @param obj Pointer to an object of type of_action_ofdpa_oam_lm_rx_count.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_oam_lm_rx_count_experimenter_get(
    of_action_ofdpa_oam_lm_rx_count_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_OAM_LM_RX_COUNT);
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
 * Set experimenter in an object of type of_action_ofdpa_oam_lm_rx_count.
 * @param obj Pointer to an object of type of_action_ofdpa_oam_lm_rx_count.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_oam_lm_rx_count_experimenter_set(
    of_action_ofdpa_oam_lm_rx_count_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_OAM_LM_RX_COUNT);
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
 * Get subtype from an object of type of_action_ofdpa_oam_lm_rx_count.
 * @param obj Pointer to an object of type of_action_ofdpa_oam_lm_rx_count.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_oam_lm_rx_count_subtype_get(
    of_action_ofdpa_oam_lm_rx_count_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_OAM_LM_RX_COUNT);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa_oam_lm_rx_count.
 * @param obj Pointer to an object of type of_action_ofdpa_oam_lm_rx_count.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_oam_lm_rx_count_subtype_set(
    of_action_ofdpa_oam_lm_rx_count_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_OAM_LM_RX_COUNT);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type of_action_ofdpa_oam_lm_rx_count.
 * @param obj Pointer to an object of type of_action_ofdpa_oam_lm_rx_count.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_oam_lm_rx_count_traffic_class_get(
    of_action_ofdpa_oam_lm_rx_count_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_OAM_LM_RX_COUNT);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set traffic_class in an object of type of_action_ofdpa_oam_lm_rx_count.
 * @param obj Pointer to an object of type of_action_ofdpa_oam_lm_rx_count.
 * @param traffic_class The value to write into the object
 */
void
of_action_ofdpa_oam_lm_rx_count_traffic_class_set(
    of_action_ofdpa_oam_lm_rx_count_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_OAM_LM_RX_COUNT);
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
    of_wire_buffer_u8_set(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get lmep_id from an object of type of_action_ofdpa_oam_lm_rx_count.
 * @param obj Pointer to an object of type of_action_ofdpa_oam_lm_rx_count.
 * @param lmep_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_oam_lm_rx_count_lmep_id_get(
    of_action_ofdpa_oam_lm_rx_count_t *obj,
    uint32_t *lmep_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_OAM_LM_RX_COUNT);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, lmep_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set lmep_id in an object of type of_action_ofdpa_oam_lm_rx_count.
 * @param obj Pointer to an object of type of_action_ofdpa_oam_lm_rx_count.
 * @param lmep_id The value to write into the object
 */
void
of_action_ofdpa_oam_lm_rx_count_lmep_id_set(
    of_action_ofdpa_oam_lm_rx_count_t *obj,
    uint32_t lmep_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_OAM_LM_RX_COUNT);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, lmep_id);

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
of_action_id_ofdpa_oam_lm_rx_count_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0xb); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_ofdpa_oam_lm_rx_count of_action_id_ofdpa_oam_lm_rx_count
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_ofdpa_oam_lm_rx_count_push_wire_values(of_action_id_ofdpa_oam_lm_rx_count_t *obj)
{

    of_action_id_ofdpa_oam_lm_rx_count_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_ofdpa_oam_lm_rx_count object
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
 * \ingroup of_action_id_ofdpa_oam_lm_rx_count
 */

of_action_id_ofdpa_oam_lm_rx_count_t *
of_action_id_ofdpa_oam_lm_rx_count_new(of_version_t version)
{
    of_action_id_ofdpa_oam_lm_rx_count_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_OAM_LM_RX_COUNT] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_OAM_LM_RX_COUNT];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_oam_lm_rx_count_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_oam_lm_rx_count_init(obj, version, bytes, 0);

    if (of_action_id_ofdpa_oam_lm_rx_count_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa_oam_lm_rx_count.
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
of_action_id_ofdpa_oam_lm_rx_count_init(of_action_id_ofdpa_oam_lm_rx_count_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA_OAM_LM_RX_COUNT] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_OAM_LM_RX_COUNT] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_OAM_LM_RX_COUNT];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA_OAM_LM_RX_COUNT;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_ofdpa_oam_lm_rx_count_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa_oam_lm_rx_count.
 * @param obj Pointer to an object of type of_action_id_ofdpa_oam_lm_rx_count.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_oam_lm_rx_count_experimenter_get(
    of_action_id_ofdpa_oam_lm_rx_count_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_OAM_LM_RX_COUNT);
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
 * Set experimenter in an object of type of_action_id_ofdpa_oam_lm_rx_count.
 * @param obj Pointer to an object of type of_action_id_ofdpa_oam_lm_rx_count.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_oam_lm_rx_count_experimenter_set(
    of_action_id_ofdpa_oam_lm_rx_count_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_OAM_LM_RX_COUNT);
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
 * Get subtype from an object of type of_action_id_ofdpa_oam_lm_rx_count.
 * @param obj Pointer to an object of type of_action_id_ofdpa_oam_lm_rx_count.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_oam_lm_rx_count_subtype_get(
    of_action_id_ofdpa_oam_lm_rx_count_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_OAM_LM_RX_COUNT);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa_oam_lm_rx_count.
 * @param obj Pointer to an object of type of_action_id_ofdpa_oam_lm_rx_count.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_oam_lm_rx_count_subtype_set(
    of_action_id_ofdpa_oam_lm_rx_count_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_OAM_LM_RX_COUNT);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_oam_lm_tx_count_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0xa); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_ofdpa_oam_lm_tx_count of_action_ofdpa_oam_lm_tx_count
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_ofdpa_oam_lm_tx_count_push_wire_values(of_action_ofdpa_oam_lm_tx_count_t *obj)
{

    of_action_ofdpa_oam_lm_tx_count_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_ofdpa_oam_lm_tx_count object
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
 * \ingroup of_action_ofdpa_oam_lm_tx_count
 */

of_action_ofdpa_oam_lm_tx_count_t *
of_action_ofdpa_oam_lm_tx_count_new(of_version_t version)
{
    of_action_ofdpa_oam_lm_tx_count_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_OAM_LM_TX_COUNT] + of_object_extra_len[version][OF_ACTION_OFDPA_OAM_LM_TX_COUNT];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_oam_lm_tx_count_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_oam_lm_tx_count_init(obj, version, bytes, 0);

    if (of_action_ofdpa_oam_lm_tx_count_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa_oam_lm_tx_count.
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
of_action_ofdpa_oam_lm_tx_count_init(of_action_ofdpa_oam_lm_tx_count_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA_OAM_LM_TX_COUNT] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_OAM_LM_TX_COUNT] + of_object_extra_len[version][OF_ACTION_OFDPA_OAM_LM_TX_COUNT];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA_OAM_LM_TX_COUNT;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_ofdpa_oam_lm_tx_count_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa_oam_lm_tx_count.
 * @param obj Pointer to an object of type of_action_ofdpa_oam_lm_tx_count.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_oam_lm_tx_count_experimenter_get(
    of_action_ofdpa_oam_lm_tx_count_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_OAM_LM_TX_COUNT);
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
 * Set experimenter in an object of type of_action_ofdpa_oam_lm_tx_count.
 * @param obj Pointer to an object of type of_action_ofdpa_oam_lm_tx_count.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_oam_lm_tx_count_experimenter_set(
    of_action_ofdpa_oam_lm_tx_count_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_OAM_LM_TX_COUNT);
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
 * Get subtype from an object of type of_action_ofdpa_oam_lm_tx_count.
 * @param obj Pointer to an object of type of_action_ofdpa_oam_lm_tx_count.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_oam_lm_tx_count_subtype_get(
    of_action_ofdpa_oam_lm_tx_count_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_OAM_LM_TX_COUNT);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa_oam_lm_tx_count.
 * @param obj Pointer to an object of type of_action_ofdpa_oam_lm_tx_count.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_oam_lm_tx_count_subtype_set(
    of_action_ofdpa_oam_lm_tx_count_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_OAM_LM_TX_COUNT);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type of_action_ofdpa_oam_lm_tx_count.
 * @param obj Pointer to an object of type of_action_ofdpa_oam_lm_tx_count.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_action_ofdpa_oam_lm_tx_count_traffic_class_get(
    of_action_ofdpa_oam_lm_tx_count_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_OAM_LM_TX_COUNT);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set traffic_class in an object of type of_action_ofdpa_oam_lm_tx_count.
 * @param obj Pointer to an object of type of_action_ofdpa_oam_lm_tx_count.
 * @param traffic_class The value to write into the object
 */
void
of_action_ofdpa_oam_lm_tx_count_traffic_class_set(
    of_action_ofdpa_oam_lm_tx_count_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_OAM_LM_TX_COUNT);
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
    of_wire_buffer_u8_set(wbuf, abs_offset, traffic_class);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get lmep_id from an object of type of_action_ofdpa_oam_lm_tx_count.
 * @param obj Pointer to an object of type of_action_ofdpa_oam_lm_tx_count.
 * @param lmep_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_oam_lm_tx_count_lmep_id_get(
    of_action_ofdpa_oam_lm_tx_count_t *obj,
    uint32_t *lmep_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_OAM_LM_TX_COUNT);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, lmep_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set lmep_id in an object of type of_action_ofdpa_oam_lm_tx_count.
 * @param obj Pointer to an object of type of_action_ofdpa_oam_lm_tx_count.
 * @param lmep_id The value to write into the object
 */
void
of_action_ofdpa_oam_lm_tx_count_lmep_id_set(
    of_action_ofdpa_oam_lm_tx_count_t *obj,
    uint32_t lmep_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_OAM_LM_TX_COUNT);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, lmep_id);

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
of_action_id_ofdpa_oam_lm_tx_count_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0xa); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_ofdpa_oam_lm_tx_count of_action_id_ofdpa_oam_lm_tx_count
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_ofdpa_oam_lm_tx_count_push_wire_values(of_action_id_ofdpa_oam_lm_tx_count_t *obj)
{

    of_action_id_ofdpa_oam_lm_tx_count_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_ofdpa_oam_lm_tx_count object
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
 * \ingroup of_action_id_ofdpa_oam_lm_tx_count
 */

of_action_id_ofdpa_oam_lm_tx_count_t *
of_action_id_ofdpa_oam_lm_tx_count_new(of_version_t version)
{
    of_action_id_ofdpa_oam_lm_tx_count_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_OAM_LM_TX_COUNT] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_OAM_LM_TX_COUNT];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_oam_lm_tx_count_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_oam_lm_tx_count_init(obj, version, bytes, 0);

    if (of_action_id_ofdpa_oam_lm_tx_count_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa_oam_lm_tx_count.
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
of_action_id_ofdpa_oam_lm_tx_count_init(of_action_id_ofdpa_oam_lm_tx_count_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA_OAM_LM_TX_COUNT] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_OAM_LM_TX_COUNT] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_OAM_LM_TX_COUNT];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA_OAM_LM_TX_COUNT;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_ofdpa_oam_lm_tx_count_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa_oam_lm_tx_count.
 * @param obj Pointer to an object of type of_action_id_ofdpa_oam_lm_tx_count.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_oam_lm_tx_count_experimenter_get(
    of_action_id_ofdpa_oam_lm_tx_count_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_OAM_LM_TX_COUNT);
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
 * Set experimenter in an object of type of_action_id_ofdpa_oam_lm_tx_count.
 * @param obj Pointer to an object of type of_action_id_ofdpa_oam_lm_tx_count.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_oam_lm_tx_count_experimenter_set(
    of_action_id_ofdpa_oam_lm_tx_count_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_OAM_LM_TX_COUNT);
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
 * Get subtype from an object of type of_action_id_ofdpa_oam_lm_tx_count.
 * @param obj Pointer to an object of type of_action_id_ofdpa_oam_lm_tx_count.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_oam_lm_tx_count_subtype_get(
    of_action_id_ofdpa_oam_lm_tx_count_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_OAM_LM_TX_COUNT);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa_oam_lm_tx_count.
 * @param obj Pointer to an object of type of_action_id_ofdpa_oam_lm_tx_count.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_oam_lm_tx_count_subtype_set(
    of_action_id_ofdpa_oam_lm_tx_count_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_OAM_LM_TX_COUNT);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_pop_cw_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x4); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_ofdpa_pop_cw of_action_ofdpa_pop_cw
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_ofdpa_pop_cw_push_wire_values(of_action_ofdpa_pop_cw_t *obj)
{

    of_action_ofdpa_pop_cw_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_ofdpa_pop_cw object
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
 * \ingroup of_action_ofdpa_pop_cw
 */

of_action_ofdpa_pop_cw_t *
of_action_ofdpa_pop_cw_new(of_version_t version)
{
    of_action_ofdpa_pop_cw_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_POP_CW] + of_object_extra_len[version][OF_ACTION_OFDPA_POP_CW];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_pop_cw_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_pop_cw_init(obj, version, bytes, 0);

    if (of_action_ofdpa_pop_cw_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa_pop_cw.
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
of_action_ofdpa_pop_cw_init(of_action_ofdpa_pop_cw_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA_POP_CW] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_POP_CW] + of_object_extra_len[version][OF_ACTION_OFDPA_POP_CW];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA_POP_CW;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_ofdpa_pop_cw_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa_pop_cw.
 * @param obj Pointer to an object of type of_action_ofdpa_pop_cw.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_pop_cw_experimenter_get(
    of_action_ofdpa_pop_cw_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_POP_CW);
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
 * Set experimenter in an object of type of_action_ofdpa_pop_cw.
 * @param obj Pointer to an object of type of_action_ofdpa_pop_cw.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_pop_cw_experimenter_set(
    of_action_ofdpa_pop_cw_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_POP_CW);
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
 * Get subtype from an object of type of_action_ofdpa_pop_cw.
 * @param obj Pointer to an object of type of_action_ofdpa_pop_cw.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_pop_cw_subtype_get(
    of_action_ofdpa_pop_cw_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_POP_CW);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa_pop_cw.
 * @param obj Pointer to an object of type of_action_ofdpa_pop_cw.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_pop_cw_subtype_set(
    of_action_ofdpa_pop_cw_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_POP_CW);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_id_ofdpa_pop_cw_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x4); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_ofdpa_pop_cw of_action_id_ofdpa_pop_cw
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_ofdpa_pop_cw_push_wire_values(of_action_id_ofdpa_pop_cw_t *obj)
{

    of_action_id_ofdpa_pop_cw_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_ofdpa_pop_cw object
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
 * \ingroup of_action_id_ofdpa_pop_cw
 */

of_action_id_ofdpa_pop_cw_t *
of_action_id_ofdpa_pop_cw_new(of_version_t version)
{
    of_action_id_ofdpa_pop_cw_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_POP_CW] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_POP_CW];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_pop_cw_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_pop_cw_init(obj, version, bytes, 0);

    if (of_action_id_ofdpa_pop_cw_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa_pop_cw.
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
of_action_id_ofdpa_pop_cw_init(of_action_id_ofdpa_pop_cw_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA_POP_CW] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_POP_CW] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_POP_CW];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA_POP_CW;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_ofdpa_pop_cw_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa_pop_cw.
 * @param obj Pointer to an object of type of_action_id_ofdpa_pop_cw.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_pop_cw_experimenter_get(
    of_action_id_ofdpa_pop_cw_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_POP_CW);
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
 * Set experimenter in an object of type of_action_id_ofdpa_pop_cw.
 * @param obj Pointer to an object of type of_action_id_ofdpa_pop_cw.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_pop_cw_experimenter_set(
    of_action_id_ofdpa_pop_cw_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_POP_CW);
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
 * Get subtype from an object of type of_action_id_ofdpa_pop_cw.
 * @param obj Pointer to an object of type of_action_id_ofdpa_pop_cw.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_pop_cw_subtype_get(
    of_action_id_ofdpa_pop_cw_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_POP_CW);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa_pop_cw.
 * @param obj Pointer to an object of type of_action_id_ofdpa_pop_cw.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_pop_cw_subtype_set(
    of_action_id_ofdpa_pop_cw_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_POP_CW);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_pop_l2hdr_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x2); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_ofdpa_pop_l2hdr of_action_ofdpa_pop_l2hdr
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_ofdpa_pop_l2hdr_push_wire_values(of_action_ofdpa_pop_l2hdr_t *obj)
{

    of_action_ofdpa_pop_l2hdr_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_ofdpa_pop_l2hdr object
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
 * \ingroup of_action_ofdpa_pop_l2hdr
 */

of_action_ofdpa_pop_l2hdr_t *
of_action_ofdpa_pop_l2hdr_new(of_version_t version)
{
    of_action_ofdpa_pop_l2hdr_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_POP_L2HDR] + of_object_extra_len[version][OF_ACTION_OFDPA_POP_L2HDR];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_pop_l2hdr_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_pop_l2hdr_init(obj, version, bytes, 0);

    if (of_action_ofdpa_pop_l2hdr_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa_pop_l2hdr.
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
of_action_ofdpa_pop_l2hdr_init(of_action_ofdpa_pop_l2hdr_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA_POP_L2HDR] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_POP_L2HDR] + of_object_extra_len[version][OF_ACTION_OFDPA_POP_L2HDR];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA_POP_L2HDR;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_ofdpa_pop_l2hdr_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa_pop_l2hdr.
 * @param obj Pointer to an object of type of_action_ofdpa_pop_l2hdr.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_pop_l2hdr_experimenter_get(
    of_action_ofdpa_pop_l2hdr_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_POP_L2HDR);
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
 * Set experimenter in an object of type of_action_ofdpa_pop_l2hdr.
 * @param obj Pointer to an object of type of_action_ofdpa_pop_l2hdr.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_pop_l2hdr_experimenter_set(
    of_action_ofdpa_pop_l2hdr_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_POP_L2HDR);
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
 * Get subtype from an object of type of_action_ofdpa_pop_l2hdr.
 * @param obj Pointer to an object of type of_action_ofdpa_pop_l2hdr.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_pop_l2hdr_subtype_get(
    of_action_ofdpa_pop_l2hdr_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_POP_L2HDR);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa_pop_l2hdr.
 * @param obj Pointer to an object of type of_action_ofdpa_pop_l2hdr.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_pop_l2hdr_subtype_set(
    of_action_ofdpa_pop_l2hdr_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_POP_L2HDR);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_id_ofdpa_pop_l2hdr_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x2); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_id_ofdpa_pop_l2hdr of_action_id_ofdpa_pop_l2hdr
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_id_ofdpa_pop_l2hdr_push_wire_values(of_action_id_ofdpa_pop_l2hdr_t *obj)
{

    of_action_id_ofdpa_pop_l2hdr_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_id_ofdpa_pop_l2hdr object
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
 * \ingroup of_action_id_ofdpa_pop_l2hdr
 */

of_action_id_ofdpa_pop_l2hdr_t *
of_action_id_ofdpa_pop_l2hdr_new(of_version_t version)
{
    of_action_id_ofdpa_pop_l2hdr_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_POP_L2HDR] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_POP_L2HDR];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_id_ofdpa_pop_l2hdr_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_id_ofdpa_pop_l2hdr_init(obj, version, bytes, 0);

    if (of_action_id_ofdpa_pop_l2hdr_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_id_ofdpa_pop_l2hdr.
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
of_action_id_ofdpa_pop_l2hdr_init(of_action_id_ofdpa_pop_l2hdr_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_ID_OFDPA_POP_L2HDR] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_ID_OFDPA_POP_L2HDR] + of_object_extra_len[version][OF_ACTION_ID_OFDPA_POP_L2HDR];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_ID_OFDPA_POP_L2HDR;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_id_ofdpa_pop_l2hdr_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_id_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_id_ofdpa_pop_l2hdr.
 * @param obj Pointer to an object of type of_action_id_ofdpa_pop_l2hdr.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_id_ofdpa_pop_l2hdr_experimenter_get(
    of_action_id_ofdpa_pop_l2hdr_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_POP_L2HDR);
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
 * Set experimenter in an object of type of_action_id_ofdpa_pop_l2hdr.
 * @param obj Pointer to an object of type of_action_id_ofdpa_pop_l2hdr.
 * @param experimenter The value to write into the object
 */
void
of_action_id_ofdpa_pop_l2hdr_experimenter_set(
    of_action_id_ofdpa_pop_l2hdr_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_POP_L2HDR);
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
 * Get subtype from an object of type of_action_id_ofdpa_pop_l2hdr.
 * @param obj Pointer to an object of type of_action_id_ofdpa_pop_l2hdr.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_id_ofdpa_pop_l2hdr_subtype_get(
    of_action_id_ofdpa_pop_l2hdr_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_POP_L2HDR);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_id_ofdpa_pop_l2hdr.
 * @param obj Pointer to an object of type of_action_id_ofdpa_pop_l2hdr.
 * @param subtype The value to write into the object
 */
void
of_action_id_ofdpa_pop_l2hdr_subtype_set(
    of_action_id_ofdpa_pop_l2hdr_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_ID_OFDPA_POP_L2HDR);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

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
of_action_ofdpa_push_cw_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        *(uint16_t *)(buf + 8) = U16_HTON(0x3); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_action_ofdpa_push_cw of_action_ofdpa_push_cw
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_action_ofdpa_push_cw_push_wire_values(of_action_ofdpa_push_cw_t *obj)
{

    of_action_ofdpa_push_cw_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_action_ofdpa_push_cw object
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
 * \ingroup of_action_ofdpa_push_cw
 */

of_action_ofdpa_push_cw_t *
of_action_ofdpa_push_cw_new(of_version_t version)
{
    of_action_ofdpa_push_cw_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_PUSH_CW] + of_object_extra_len[version][OF_ACTION_OFDPA_PUSH_CW];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_action_ofdpa_push_cw_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_action_ofdpa_push_cw_init(obj, version, bytes, 0);

    if (of_action_ofdpa_push_cw_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_action_ofdpa_push_cw.
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
of_action_ofdpa_push_cw_init(of_action_ofdpa_push_cw_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ACTION_OFDPA_PUSH_CW] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ACTION_OFDPA_PUSH_CW] + of_object_extra_len[version][OF_ACTION_OFDPA_PUSH_CW];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ACTION_OFDPA_PUSH_CW;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_action_ofdpa_push_cw_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_action_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_action_ofdpa_push_cw.
 * @param obj Pointer to an object of type of_action_ofdpa_push_cw.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_action_ofdpa_push_cw_experimenter_get(
    of_action_ofdpa_push_cw_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_PUSH_CW);
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
 * Set experimenter in an object of type of_action_ofdpa_push_cw.
 * @param obj Pointer to an object of type of_action_ofdpa_push_cw.
 * @param experimenter The value to write into the object
 */
void
of_action_ofdpa_push_cw_experimenter_set(
    of_action_ofdpa_push_cw_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_PUSH_CW);
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
 * Get subtype from an object of type of_action_ofdpa_push_cw.
 * @param obj Pointer to an object of type of_action_ofdpa_push_cw.
 * @param subtype Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_action_ofdpa_push_cw_subtype_get(
    of_action_ofdpa_push_cw_t *obj,
    uint16_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_PUSH_CW);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_action_ofdpa_push_cw.
 * @param obj Pointer to an object of type of_action_ofdpa_push_cw.
 * @param subtype The value to write into the object
 */
void
of_action_ofdpa_push_cw_subtype_set(
    of_action_ofdpa_push_cw_t *obj,
    uint16_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ACTION_OFDPA_PUSH_CW);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
