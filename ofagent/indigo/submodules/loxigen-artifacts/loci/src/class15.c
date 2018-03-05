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
of_meter_config_stats_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xa); /* stats_type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_meter_config_stats_reply of_meter_config_stats_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_meter_config_stats_reply_push_wire_values(of_meter_config_stats_reply_t *obj)
{

    of_meter_config_stats_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_meter_config_stats_reply object
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
 * \ingroup of_meter_config_stats_reply
 */

of_meter_config_stats_reply_t *
of_meter_config_stats_reply_new(of_version_t version)
{
    of_meter_config_stats_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_METER_CONFIG_STATS_REPLY] + of_object_extra_len[version][OF_METER_CONFIG_STATS_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_meter_config_stats_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_meter_config_stats_reply_init(obj, version, bytes, 0);

    if (of_meter_config_stats_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_meter_config_stats_reply.
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
of_meter_config_stats_reply_init(of_meter_config_stats_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_METER_CONFIG_STATS_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_METER_CONFIG_STATS_REPLY] + of_object_extra_len[version][OF_METER_CONFIG_STATS_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_METER_CONFIG_STATS_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_meter_config_stats_reply_push_wire_types;

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
 * Create a new of_meter_config_stats_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_meter_config_stats_reply
 */

of_meter_config_stats_reply_t *
of_meter_config_stats_reply_new_from_message(of_message_t msg)
{
    of_meter_config_stats_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_meter_config_stats_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_meter_config_stats_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_meter_config_stats_reply.
 * @param obj Pointer to an object of type of_meter_config_stats_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_config_stats_reply_xid_get(
    of_meter_config_stats_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_CONFIG_STATS_REPLY);
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
 * Set xid in an object of type of_meter_config_stats_reply.
 * @param obj Pointer to an object of type of_meter_config_stats_reply.
 * @param xid The value to write into the object
 */
void
of_meter_config_stats_reply_xid_set(
    of_meter_config_stats_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_CONFIG_STATS_REPLY);
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
 * Get flags from an object of type of_meter_config_stats_reply.
 * @param obj Pointer to an object of type of_meter_config_stats_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_meter_config_stats_reply_flags_get(
    of_meter_config_stats_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_CONFIG_STATS_REPLY);
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
 * Set flags in an object of type of_meter_config_stats_reply.
 * @param obj Pointer to an object of type of_meter_config_stats_reply.
 * @param flags The value to write into the object
 */
void
of_meter_config_stats_reply_flags_set(
    of_meter_config_stats_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_CONFIG_STATS_REPLY);
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
 * Bind an object of type of_list_meter_band_t to the parent of type of_meter_config_stats_reply for
 * member entries
 * @param obj Pointer to an object of type of_meter_config_stats_reply.
 * @param entries Pointer to the child object of type
 * of_list_meter_band_t to be filled out.
 * \ingroup of_meter_config_stats_reply
 *
 * The parameter entries is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_meter_config_stats_reply_entries_bind(
    of_meter_config_stats_reply_t *obj,
    of_list_meter_band_t *entries)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_METER_CONFIG_STATS_REPLY);
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
 * a of_meter_config_stats_reply instance.
 *
 * @param obj Pointer to the source of type of_meter_config_stats_reply_t
 * @returns A pointer to a new instance of type of_list_meter_band_t whose contents
 * match that of entries from source
 * @returns NULL if an error occurs
 */
of_list_meter_band_t *
of_meter_config_stats_reply_entries_get(of_meter_config_stats_reply_t *obj) {
    of_list_meter_band_t _entries;
    of_list_meter_band_t *_entries_ptr;

    of_meter_config_stats_reply_entries_bind(obj, &_entries);
    _entries_ptr = (of_list_meter_band_t *)of_object_dup(&_entries);
    return _entries_ptr;
}

/**
 * Set entries in an object of type of_meter_config_stats_reply.
 * @param obj Pointer to an object of type of_meter_config_stats_reply.
 * @param entries Pointer to the child of type of_list_meter_band_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_meter_config_stats_reply_entries_set(
    of_meter_config_stats_reply_t *obj,
    of_list_meter_band_t *entries)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_METER_CONFIG_STATS_REPLY);
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
of_meter_config_stats_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xa); /* stats_type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_meter_config_stats_request of_meter_config_stats_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_meter_config_stats_request_push_wire_values(of_meter_config_stats_request_t *obj)
{

    of_meter_config_stats_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_meter_config_stats_request object
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
 * \ingroup of_meter_config_stats_request
 */

of_meter_config_stats_request_t *
of_meter_config_stats_request_new(of_version_t version)
{
    of_meter_config_stats_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_METER_CONFIG_STATS_REQUEST] + of_object_extra_len[version][OF_METER_CONFIG_STATS_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_meter_config_stats_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_meter_config_stats_request_init(obj, version, bytes, 0);

    if (of_meter_config_stats_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_meter_config_stats_request.
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
of_meter_config_stats_request_init(of_meter_config_stats_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_METER_CONFIG_STATS_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_METER_CONFIG_STATS_REQUEST] + of_object_extra_len[version][OF_METER_CONFIG_STATS_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_METER_CONFIG_STATS_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_meter_config_stats_request_push_wire_types;

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
 * Create a new of_meter_config_stats_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_meter_config_stats_request
 */

of_meter_config_stats_request_t *
of_meter_config_stats_request_new_from_message(of_message_t msg)
{
    of_meter_config_stats_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_meter_config_stats_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_meter_config_stats_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_meter_config_stats_request.
 * @param obj Pointer to an object of type of_meter_config_stats_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_config_stats_request_xid_get(
    of_meter_config_stats_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_CONFIG_STATS_REQUEST);
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
 * Set xid in an object of type of_meter_config_stats_request.
 * @param obj Pointer to an object of type of_meter_config_stats_request.
 * @param xid The value to write into the object
 */
void
of_meter_config_stats_request_xid_set(
    of_meter_config_stats_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_CONFIG_STATS_REQUEST);
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
 * Get flags from an object of type of_meter_config_stats_request.
 * @param obj Pointer to an object of type of_meter_config_stats_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_meter_config_stats_request_flags_get(
    of_meter_config_stats_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_CONFIG_STATS_REQUEST);
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
 * Set flags in an object of type of_meter_config_stats_request.
 * @param obj Pointer to an object of type of_meter_config_stats_request.
 * @param flags The value to write into the object
 */
void
of_meter_config_stats_request_flags_set(
    of_meter_config_stats_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_CONFIG_STATS_REQUEST);
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
 * Get meter_id from an object of type of_meter_config_stats_request.
 * @param obj Pointer to an object of type of_meter_config_stats_request.
 * @param meter_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_config_stats_request_meter_id_get(
    of_meter_config_stats_request_t *obj,
    uint32_t *meter_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_CONFIG_STATS_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, meter_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set meter_id in an object of type of_meter_config_stats_request.
 * @param obj Pointer to an object of type of_meter_config_stats_request.
 * @param meter_id The value to write into the object
 */
void
of_meter_config_stats_request_meter_id_set(
    of_meter_config_stats_request_t *obj,
    uint32_t meter_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_CONFIG_STATS_REQUEST);
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
of_meter_delete_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x1d; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0x2); /* command */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_meter_delete of_meter_delete
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_meter_delete_push_wire_values(of_meter_delete_t *obj)
{

    of_meter_delete_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_meter_delete object
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
 * \ingroup of_meter_delete
 */

of_meter_delete_t *
of_meter_delete_new(of_version_t version)
{
    of_meter_delete_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_METER_DELETE] + of_object_extra_len[version][OF_METER_DELETE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_meter_delete_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_meter_delete_init(obj, version, bytes, 0);

    if (of_meter_delete_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_meter_delete.
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
of_meter_delete_init(of_meter_delete_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_METER_DELETE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_METER_DELETE] + of_object_extra_len[version][OF_METER_DELETE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_METER_DELETE;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_meter_delete_push_wire_types;

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
 * Create a new of_meter_delete object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_meter_delete
 */

of_meter_delete_t *
of_meter_delete_new_from_message(of_message_t msg)
{
    of_meter_delete_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_meter_delete_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_meter_delete_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_meter_delete.
 * @param obj Pointer to an object of type of_meter_delete.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_delete_xid_get(
    of_meter_delete_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_DELETE);
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
 * Set xid in an object of type of_meter_delete.
 * @param obj Pointer to an object of type of_meter_delete.
 * @param xid The value to write into the object
 */
void
of_meter_delete_xid_set(
    of_meter_delete_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_DELETE);
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
 * Get flags from an object of type of_meter_delete.
 * @param obj Pointer to an object of type of_meter_delete.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_meter_delete_flags_get(
    of_meter_delete_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_DELETE);
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
 * Set flags in an object of type of_meter_delete.
 * @param obj Pointer to an object of type of_meter_delete.
 * @param flags The value to write into the object
 */
void
of_meter_delete_flags_set(
    of_meter_delete_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_DELETE);
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
 * Get meter_id from an object of type of_meter_delete.
 * @param obj Pointer to an object of type of_meter_delete.
 * @param meter_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_delete_meter_id_get(
    of_meter_delete_t *obj,
    uint32_t *meter_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_DELETE);
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
 * Set meter_id in an object of type of_meter_delete.
 * @param obj Pointer to an object of type of_meter_delete.
 * @param meter_id The value to write into the object
 */
void
of_meter_delete_meter_id_set(
    of_meter_delete_t *obj,
    uint32_t meter_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_DELETE);
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
 * Bind an object of type of_list_meter_band_t to the parent of type of_meter_delete for
 * member meters
 * @param obj Pointer to an object of type of_meter_delete.
 * @param meters Pointer to the child object of type
 * of_list_meter_band_t to be filled out.
 * \ingroup of_meter_delete
 *
 * The parameter meters is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_meter_delete_meters_bind(
    of_meter_delete_t *obj,
    of_list_meter_band_t *meters)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_METER_DELETE);
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
 * a of_meter_delete instance.
 *
 * @param obj Pointer to the source of type of_meter_delete_t
 * @returns A pointer to a new instance of type of_list_meter_band_t whose contents
 * match that of meters from source
 * @returns NULL if an error occurs
 */
of_list_meter_band_t *
of_meter_delete_meters_get(of_meter_delete_t *obj) {
    of_list_meter_band_t _meters;
    of_list_meter_band_t *_meters_ptr;

    of_meter_delete_meters_bind(obj, &_meters);
    _meters_ptr = (of_list_meter_band_t *)of_object_dup(&_meters);
    return _meters_ptr;
}

/**
 * Set meters in an object of type of_meter_delete.
 * @param obj Pointer to an object of type of_meter_delete.
 * @param meters Pointer to the child of type of_list_meter_band_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_meter_delete_meters_set(
    of_meter_delete_t *obj,
    of_list_meter_band_t *meters)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_METER_DELETE);
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



/**
 * \defgroup of_meter_features of_meter_features
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_meter_features_push_wire_values(of_meter_features_t *obj)
{

    return OF_ERROR_NONE;
}

/**
 * Create a new of_meter_features object
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
 * \ingroup of_meter_features
 */

of_meter_features_t *
of_meter_features_new(of_version_t version)
{
    of_meter_features_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_METER_FEATURES] + of_object_extra_len[version][OF_METER_FEATURES];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_meter_features_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_meter_features_init(obj, version, bytes, 0);

    if (of_meter_features_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_meter_features.
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
of_meter_features_init(of_meter_features_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_METER_FEATURES] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_METER_FEATURES] + of_object_extra_len[version][OF_METER_FEATURES];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_METER_FEATURES;

    /* Set up the object's function pointers */

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get max_meter from an object of type of_meter_features.
 * @param obj Pointer to an object of type of_meter_features.
 * @param max_meter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_features_max_meter_get(
    of_meter_features_t *obj,
    uint32_t *max_meter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_FEATURES);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, max_meter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set max_meter in an object of type of_meter_features.
 * @param obj Pointer to an object of type of_meter_features.
 * @param max_meter The value to write into the object
 */
void
of_meter_features_max_meter_set(
    of_meter_features_t *obj,
    uint32_t max_meter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_FEATURES);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, max_meter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get band_types from an object of type of_meter_features.
 * @param obj Pointer to an object of type of_meter_features.
 * @param band_types Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_features_band_types_get(
    of_meter_features_t *obj,
    uint32_t *band_types)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_FEATURES);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, band_types);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set band_types in an object of type of_meter_features.
 * @param obj Pointer to an object of type of_meter_features.
 * @param band_types The value to write into the object
 */
void
of_meter_features_band_types_set(
    of_meter_features_t *obj,
    uint32_t band_types)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_FEATURES);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, band_types);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get capabilities from an object of type of_meter_features.
 * @param obj Pointer to an object of type of_meter_features.
 * @param capabilities Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_features_capabilities_get(
    of_meter_features_t *obj,
    uint32_t *capabilities)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_FEATURES);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, capabilities);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set capabilities in an object of type of_meter_features.
 * @param obj Pointer to an object of type of_meter_features.
 * @param capabilities The value to write into the object
 */
void
of_meter_features_capabilities_set(
    of_meter_features_t *obj,
    uint32_t capabilities)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_FEATURES);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, capabilities);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get max_bands from an object of type of_meter_features.
 * @param obj Pointer to an object of type of_meter_features.
 * @param max_bands Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_meter_features_max_bands_get(
    of_meter_features_t *obj,
    uint8_t *max_bands)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_FEATURES);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, max_bands);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set max_bands in an object of type of_meter_features.
 * @param obj Pointer to an object of type of_meter_features.
 * @param max_bands The value to write into the object
 */
void
of_meter_features_max_bands_set(
    of_meter_features_t *obj,
    uint8_t max_bands)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_FEATURES);
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
    of_wire_buffer_u8_set(wbuf, abs_offset, max_bands);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get max_color from an object of type of_meter_features.
 * @param obj Pointer to an object of type of_meter_features.
 * @param max_color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_meter_features_max_color_get(
    of_meter_features_t *obj,
    uint8_t *max_color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_FEATURES);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 13;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, max_color);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set max_color in an object of type of_meter_features.
 * @param obj Pointer to an object of type of_meter_features.
 * @param max_color The value to write into the object
 */
void
of_meter_features_max_color_set(
    of_meter_features_t *obj,
    uint8_t max_color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_FEATURES);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 13;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, max_color);

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
of_meter_features_stats_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xb); /* stats_type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_meter_features_stats_reply of_meter_features_stats_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_meter_features_stats_reply_push_wire_values(of_meter_features_stats_reply_t *obj)
{

    of_meter_features_stats_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_meter_features_stats_reply object
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
 * \ingroup of_meter_features_stats_reply
 */

of_meter_features_stats_reply_t *
of_meter_features_stats_reply_new(of_version_t version)
{
    of_meter_features_stats_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_METER_FEATURES_STATS_REPLY] + of_object_extra_len[version][OF_METER_FEATURES_STATS_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_meter_features_stats_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_meter_features_stats_reply_init(obj, version, bytes, 0);

    if (of_meter_features_stats_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_meter_features_stats_reply.
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
of_meter_features_stats_reply_init(of_meter_features_stats_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_METER_FEATURES_STATS_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_METER_FEATURES_STATS_REPLY] + of_object_extra_len[version][OF_METER_FEATURES_STATS_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_METER_FEATURES_STATS_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_meter_features_stats_reply_push_wire_types;

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
 * Create a new of_meter_features_stats_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_meter_features_stats_reply
 */

of_meter_features_stats_reply_t *
of_meter_features_stats_reply_new_from_message(of_message_t msg)
{
    of_meter_features_stats_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_meter_features_stats_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_meter_features_stats_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_meter_features_stats_reply.
 * @param obj Pointer to an object of type of_meter_features_stats_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_features_stats_reply_xid_get(
    of_meter_features_stats_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_FEATURES_STATS_REPLY);
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
 * Set xid in an object of type of_meter_features_stats_reply.
 * @param obj Pointer to an object of type of_meter_features_stats_reply.
 * @param xid The value to write into the object
 */
void
of_meter_features_stats_reply_xid_set(
    of_meter_features_stats_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_FEATURES_STATS_REPLY);
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
 * Get flags from an object of type of_meter_features_stats_reply.
 * @param obj Pointer to an object of type of_meter_features_stats_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_meter_features_stats_reply_flags_get(
    of_meter_features_stats_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_FEATURES_STATS_REPLY);
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
 * Set flags in an object of type of_meter_features_stats_reply.
 * @param obj Pointer to an object of type of_meter_features_stats_reply.
 * @param flags The value to write into the object
 */
void
of_meter_features_stats_reply_flags_set(
    of_meter_features_stats_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_FEATURES_STATS_REPLY);
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
 * Bind an object of type of_meter_features_t to the parent of type of_meter_features_stats_reply for
 * member features
 * @param obj Pointer to an object of type of_meter_features_stats_reply.
 * @param features Pointer to the child object of type
 * of_meter_features_t to be filled out.
 * \ingroup of_meter_features_stats_reply
 *
 * The parameter features is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_meter_features_stats_reply_features_bind(
    of_meter_features_stats_reply_t *obj,
    of_meter_features_t *features)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_METER_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        cur_len = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_meter_features_init(features, obj->version, 0, 1);
    /* Attach to parent */
    features->parent = (of_object_t *)obj;
    features->wire_object.wbuf = obj->wire_object.wbuf;
    features->wire_object.obj_offset = abs_offset;
    features->wire_object.owned = 0;
    features->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of features into a new variable of type of_meter_features_t from
 * a of_meter_features_stats_reply instance.
 *
 * @param obj Pointer to the source of type of_meter_features_stats_reply_t
 * @returns A pointer to a new instance of type of_meter_features_t whose contents
 * match that of features from source
 * @returns NULL if an error occurs
 */
of_meter_features_t *
of_meter_features_stats_reply_features_get(of_meter_features_stats_reply_t *obj) {
    of_meter_features_t _features;
    of_meter_features_t *_features_ptr;

    of_meter_features_stats_reply_features_bind(obj, &_features);
    _features_ptr = (of_meter_features_t *)of_object_dup(&_features);
    return _features_ptr;
}

/**
 * Set features in an object of type of_meter_features_stats_reply.
 * @param obj Pointer to an object of type of_meter_features_stats_reply.
 * @param features Pointer to the child of type of_meter_features_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_meter_features_stats_reply_features_set(
    of_meter_features_stats_reply_t *obj,
    of_meter_features_t *features)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_METER_FEATURES_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 16;
        cur_len = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = features->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == features->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(features, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(features, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (features->wire_length_set != NULL) {
        features->wire_length_set((of_object_t *)features, features->length);
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
of_meter_features_stats_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xb); /* stats_type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_meter_features_stats_request of_meter_features_stats_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_meter_features_stats_request_push_wire_values(of_meter_features_stats_request_t *obj)
{

    of_meter_features_stats_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_meter_features_stats_request object
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
 * \ingroup of_meter_features_stats_request
 */

of_meter_features_stats_request_t *
of_meter_features_stats_request_new(of_version_t version)
{
    of_meter_features_stats_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_METER_FEATURES_STATS_REQUEST] + of_object_extra_len[version][OF_METER_FEATURES_STATS_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_meter_features_stats_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_meter_features_stats_request_init(obj, version, bytes, 0);

    if (of_meter_features_stats_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_meter_features_stats_request.
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
of_meter_features_stats_request_init(of_meter_features_stats_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_METER_FEATURES_STATS_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_METER_FEATURES_STATS_REQUEST] + of_object_extra_len[version][OF_METER_FEATURES_STATS_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_METER_FEATURES_STATS_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_meter_features_stats_request_push_wire_types;

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
 * Create a new of_meter_features_stats_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_meter_features_stats_request
 */

of_meter_features_stats_request_t *
of_meter_features_stats_request_new_from_message(of_message_t msg)
{
    of_meter_features_stats_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_meter_features_stats_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_meter_features_stats_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_meter_features_stats_request.
 * @param obj Pointer to an object of type of_meter_features_stats_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_features_stats_request_xid_get(
    of_meter_features_stats_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_FEATURES_STATS_REQUEST);
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
 * Set xid in an object of type of_meter_features_stats_request.
 * @param obj Pointer to an object of type of_meter_features_stats_request.
 * @param xid The value to write into the object
 */
void
of_meter_features_stats_request_xid_set(
    of_meter_features_stats_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_FEATURES_STATS_REQUEST);
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
 * Get flags from an object of type of_meter_features_stats_request.
 * @param obj Pointer to an object of type of_meter_features_stats_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_meter_features_stats_request_flags_get(
    of_meter_features_stats_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_FEATURES_STATS_REQUEST);
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
 * Set flags in an object of type of_meter_features_stats_request.
 * @param obj Pointer to an object of type of_meter_features_stats_request.
 * @param flags The value to write into the object
 */
void
of_meter_features_stats_request_flags_set(
    of_meter_features_stats_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_FEATURES_STATS_REQUEST);
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
of_meter_mod_failed_error_msg_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x1; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xc); /* err_type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_meter_mod_failed_error_msg of_meter_mod_failed_error_msg
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_meter_mod_failed_error_msg_push_wire_values(of_meter_mod_failed_error_msg_t *obj)
{

    of_meter_mod_failed_error_msg_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_meter_mod_failed_error_msg object
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
 * \ingroup of_meter_mod_failed_error_msg
 */

of_meter_mod_failed_error_msg_t *
of_meter_mod_failed_error_msg_new(of_version_t version)
{
    of_meter_mod_failed_error_msg_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_METER_MOD_FAILED_ERROR_MSG] + of_object_extra_len[version][OF_METER_MOD_FAILED_ERROR_MSG];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_meter_mod_failed_error_msg_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_meter_mod_failed_error_msg_init(obj, version, bytes, 0);

    if (of_meter_mod_failed_error_msg_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_meter_mod_failed_error_msg.
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
of_meter_mod_failed_error_msg_init(of_meter_mod_failed_error_msg_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_METER_MOD_FAILED_ERROR_MSG] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_METER_MOD_FAILED_ERROR_MSG] + of_object_extra_len[version][OF_METER_MOD_FAILED_ERROR_MSG];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_METER_MOD_FAILED_ERROR_MSG;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_meter_mod_failed_error_msg_push_wire_types;

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
 * Create a new of_meter_mod_failed_error_msg object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_meter_mod_failed_error_msg
 */

of_meter_mod_failed_error_msg_t *
of_meter_mod_failed_error_msg_new_from_message(of_message_t msg)
{
    of_meter_mod_failed_error_msg_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_meter_mod_failed_error_msg_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_meter_mod_failed_error_msg_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_meter_mod_failed_error_msg.
 * @param obj Pointer to an object of type of_meter_mod_failed_error_msg.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_mod_failed_error_msg_xid_get(
    of_meter_mod_failed_error_msg_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_MOD_FAILED_ERROR_MSG);
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
 * Set xid in an object of type of_meter_mod_failed_error_msg.
 * @param obj Pointer to an object of type of_meter_mod_failed_error_msg.
 * @param xid The value to write into the object
 */
void
of_meter_mod_failed_error_msg_xid_set(
    of_meter_mod_failed_error_msg_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_MOD_FAILED_ERROR_MSG);
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
 * Get code from an object of type of_meter_mod_failed_error_msg.
 * @param obj Pointer to an object of type of_meter_mod_failed_error_msg.
 * @param code Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_meter_mod_failed_error_msg_code_get(
    of_meter_mod_failed_error_msg_t *obj,
    uint16_t *code)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_MOD_FAILED_ERROR_MSG);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, code);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set code in an object of type of_meter_mod_failed_error_msg.
 * @param obj Pointer to an object of type of_meter_mod_failed_error_msg.
 * @param code The value to write into the object
 */
void
of_meter_mod_failed_error_msg_code_set(
    of_meter_mod_failed_error_msg_t *obj,
    uint16_t code)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_MOD_FAILED_ERROR_MSG);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, code);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get data from an object of type of_meter_mod_failed_error_msg.
 * @param obj Pointer to an object of type of_meter_mod_failed_error_msg.
 * @param data Pointer to the child object of type
 * of_octets_t to be filled out.
 *
 */
void
of_meter_mod_failed_error_msg_data_get(
    of_meter_mod_failed_error_msg_t *obj,
    of_octets_t *data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_METER_MOD_FAILED_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
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
 * Set data in an object of type of_meter_mod_failed_error_msg.
 * @param obj Pointer to an object of type of_meter_mod_failed_error_msg.
 * @param data The value to write into the object
 */
int WARN_UNUSED_RESULT
of_meter_mod_failed_error_msg_data_set(
    of_meter_mod_failed_error_msg_t *obj,
    of_octets_t *data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_METER_MOD_FAILED_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 12;
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
of_meter_modify_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x1d; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0x1); /* command */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_meter_modify of_meter_modify
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_meter_modify_push_wire_values(of_meter_modify_t *obj)
{

    of_meter_modify_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_meter_modify object
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
 * \ingroup of_meter_modify
 */

of_meter_modify_t *
of_meter_modify_new(of_version_t version)
{
    of_meter_modify_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_METER_MODIFY] + of_object_extra_len[version][OF_METER_MODIFY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_meter_modify_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_meter_modify_init(obj, version, bytes, 0);

    if (of_meter_modify_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_meter_modify.
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
of_meter_modify_init(of_meter_modify_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_METER_MODIFY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_METER_MODIFY] + of_object_extra_len[version][OF_METER_MODIFY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_METER_MODIFY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_meter_modify_push_wire_types;

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
 * Create a new of_meter_modify object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_meter_modify
 */

of_meter_modify_t *
of_meter_modify_new_from_message(of_message_t msg)
{
    of_meter_modify_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_meter_modify_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_meter_modify_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_meter_modify.
 * @param obj Pointer to an object of type of_meter_modify.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_modify_xid_get(
    of_meter_modify_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_MODIFY);
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
 * Set xid in an object of type of_meter_modify.
 * @param obj Pointer to an object of type of_meter_modify.
 * @param xid The value to write into the object
 */
void
of_meter_modify_xid_set(
    of_meter_modify_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_MODIFY);
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
 * Get flags from an object of type of_meter_modify.
 * @param obj Pointer to an object of type of_meter_modify.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_meter_modify_flags_get(
    of_meter_modify_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_MODIFY);
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
 * Set flags in an object of type of_meter_modify.
 * @param obj Pointer to an object of type of_meter_modify.
 * @param flags The value to write into the object
 */
void
of_meter_modify_flags_set(
    of_meter_modify_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_MODIFY);
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
 * Get meter_id from an object of type of_meter_modify.
 * @param obj Pointer to an object of type of_meter_modify.
 * @param meter_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_modify_meter_id_get(
    of_meter_modify_t *obj,
    uint32_t *meter_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_MODIFY);
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
 * Set meter_id in an object of type of_meter_modify.
 * @param obj Pointer to an object of type of_meter_modify.
 * @param meter_id The value to write into the object
 */
void
of_meter_modify_meter_id_set(
    of_meter_modify_t *obj,
    uint32_t meter_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_MODIFY);
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
 * Bind an object of type of_list_meter_band_t to the parent of type of_meter_modify for
 * member meters
 * @param obj Pointer to an object of type of_meter_modify.
 * @param meters Pointer to the child object of type
 * of_list_meter_band_t to be filled out.
 * \ingroup of_meter_modify
 *
 * The parameter meters is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_meter_modify_meters_bind(
    of_meter_modify_t *obj,
    of_list_meter_band_t *meters)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_METER_MODIFY);
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
 * a of_meter_modify instance.
 *
 * @param obj Pointer to the source of type of_meter_modify_t
 * @returns A pointer to a new instance of type of_list_meter_band_t whose contents
 * match that of meters from source
 * @returns NULL if an error occurs
 */
of_list_meter_band_t *
of_meter_modify_meters_get(of_meter_modify_t *obj) {
    of_list_meter_band_t _meters;
    of_list_meter_band_t *_meters_ptr;

    of_meter_modify_meters_bind(obj, &_meters);
    _meters_ptr = (of_list_meter_band_t *)of_object_dup(&_meters);
    return _meters_ptr;
}

/**
 * Set meters in an object of type of_meter_modify.
 * @param obj Pointer to an object of type of_meter_modify.
 * @param meters Pointer to the child of type of_list_meter_band_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_meter_modify_meters_set(
    of_meter_modify_t *obj,
    of_list_meter_band_t *meters)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_METER_MODIFY);
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



/**
 * \defgroup of_meter_stats of_meter_stats
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_meter_stats_push_wire_values(of_meter_stats_t *obj)
{

    of_meter_stats_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_meter_stats object
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
 * \ingroup of_meter_stats
 */

of_meter_stats_t *
of_meter_stats_new(of_version_t version)
{
    of_meter_stats_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_METER_STATS] + of_object_extra_len[version][OF_METER_STATS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_meter_stats_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_meter_stats_init(obj, version, bytes, 0);

    if (of_meter_stats_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_meter_stats.
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
of_meter_stats_init(of_meter_stats_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_METER_STATS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_METER_STATS] + of_object_extra_len[version][OF_METER_STATS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_METER_STATS;

    /* Set up the object's function pointers */

    obj->wire_length_get = of_meter_stats_wire_length_get;
    obj->wire_length_set = of_meter_stats_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get meter_id from an object of type of_meter_stats.
 * @param obj Pointer to an object of type of_meter_stats.
 * @param meter_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_stats_meter_id_get(
    of_meter_stats_t *obj,
    uint32_t *meter_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, meter_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set meter_id in an object of type of_meter_stats.
 * @param obj Pointer to an object of type of_meter_stats.
 * @param meter_id The value to write into the object
 */
void
of_meter_stats_meter_id_set(
    of_meter_stats_t *obj,
    uint32_t meter_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, meter_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flow_count from an object of type of_meter_stats.
 * @param obj Pointer to an object of type of_meter_stats.
 * @param flow_count Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_stats_flow_count_get(
    of_meter_stats_t *obj,
    uint32_t *flow_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, flow_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flow_count in an object of type of_meter_stats.
 * @param obj Pointer to an object of type of_meter_stats.
 * @param flow_count The value to write into the object
 */
void
of_meter_stats_flow_count_set(
    of_meter_stats_t *obj,
    uint32_t flow_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, flow_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get packet_in_count from an object of type of_meter_stats.
 * @param obj Pointer to an object of type of_meter_stats.
 * @param packet_in_count Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_meter_stats_packet_in_count_get(
    of_meter_stats_t *obj,
    uint64_t *packet_in_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS);
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
    of_wire_buffer_u64_get(wbuf, abs_offset, packet_in_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set packet_in_count in an object of type of_meter_stats.
 * @param obj Pointer to an object of type of_meter_stats.
 * @param packet_in_count The value to write into the object
 */
void
of_meter_stats_packet_in_count_set(
    of_meter_stats_t *obj,
    uint64_t packet_in_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS);
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
    of_wire_buffer_u64_set(wbuf, abs_offset, packet_in_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get byte_in_count from an object of type of_meter_stats.
 * @param obj Pointer to an object of type of_meter_stats.
 * @param byte_in_count Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_meter_stats_byte_in_count_get(
    of_meter_stats_t *obj,
    uint64_t *byte_in_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, byte_in_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set byte_in_count in an object of type of_meter_stats.
 * @param obj Pointer to an object of type of_meter_stats.
 * @param byte_in_count The value to write into the object
 */
void
of_meter_stats_byte_in_count_set(
    of_meter_stats_t *obj,
    uint64_t byte_in_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, byte_in_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get duration_sec from an object of type of_meter_stats.
 * @param obj Pointer to an object of type of_meter_stats.
 * @param duration_sec Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_stats_duration_sec_get(
    of_meter_stats_t *obj,
    uint32_t *duration_sec)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, duration_sec);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set duration_sec in an object of type of_meter_stats.
 * @param obj Pointer to an object of type of_meter_stats.
 * @param duration_sec The value to write into the object
 */
void
of_meter_stats_duration_sec_set(
    of_meter_stats_t *obj,
    uint32_t duration_sec)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, duration_sec);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get duration_nsec from an object of type of_meter_stats.
 * @param obj Pointer to an object of type of_meter_stats.
 * @param duration_nsec Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_stats_duration_nsec_get(
    of_meter_stats_t *obj,
    uint32_t *duration_nsec)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, duration_nsec);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set duration_nsec in an object of type of_meter_stats.
 * @param obj Pointer to an object of type of_meter_stats.
 * @param duration_nsec The value to write into the object
 */
void
of_meter_stats_duration_nsec_set(
    of_meter_stats_t *obj,
    uint32_t duration_nsec)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, duration_nsec);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Bind an object of type of_list_meter_band_stats_t to the parent of type of_meter_stats for
 * member band_stats
 * @param obj Pointer to an object of type of_meter_stats.
 * @param band_stats Pointer to the child object of type
 * of_list_meter_band_stats_t to be filled out.
 * \ingroup of_meter_stats
 *
 * The parameter band_stats is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_meter_stats_band_stats_bind(
    of_meter_stats_t *obj,
    of_list_meter_band_stats_t *band_stats)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_METER_STATS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 40;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_meter_band_stats_init(band_stats, obj->version, 0, 1);
    /* Attach to parent */
    band_stats->parent = (of_object_t *)obj;
    band_stats->wire_object.wbuf = obj->wire_object.wbuf;
    band_stats->wire_object.obj_offset = abs_offset;
    band_stats->wire_object.owned = 0;
    band_stats->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of band_stats into a new variable of type of_list_meter_band_stats_t from
 * a of_meter_stats instance.
 *
 * @param obj Pointer to the source of type of_meter_stats_t
 * @returns A pointer to a new instance of type of_list_meter_band_stats_t whose contents
 * match that of band_stats from source
 * @returns NULL if an error occurs
 */
of_list_meter_band_stats_t *
of_meter_stats_band_stats_get(of_meter_stats_t *obj) {
    of_list_meter_band_stats_t _band_stats;
    of_list_meter_band_stats_t *_band_stats_ptr;

    of_meter_stats_band_stats_bind(obj, &_band_stats);
    _band_stats_ptr = (of_list_meter_band_stats_t *)of_object_dup(&_band_stats);
    return _band_stats_ptr;
}

/**
 * Set band_stats in an object of type of_meter_stats.
 * @param obj Pointer to an object of type of_meter_stats.
 * @param band_stats Pointer to the child of type of_list_meter_band_stats_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_meter_stats_band_stats_set(
    of_meter_stats_t *obj,
    of_list_meter_band_stats_t *band_stats)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_METER_STATS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 40;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = band_stats->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == band_stats->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(band_stats, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(band_stats, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (band_stats->wire_length_set != NULL) {
        band_stats->wire_length_set((of_object_t *)band_stats, band_stats->length);
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
of_meter_stats_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0x9); /* stats_type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_meter_stats_reply of_meter_stats_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_meter_stats_reply_push_wire_values(of_meter_stats_reply_t *obj)
{

    of_meter_stats_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_meter_stats_reply object
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
 * \ingroup of_meter_stats_reply
 */

of_meter_stats_reply_t *
of_meter_stats_reply_new(of_version_t version)
{
    of_meter_stats_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_METER_STATS_REPLY] + of_object_extra_len[version][OF_METER_STATS_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_meter_stats_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_meter_stats_reply_init(obj, version, bytes, 0);

    if (of_meter_stats_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_meter_stats_reply.
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
of_meter_stats_reply_init(of_meter_stats_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_METER_STATS_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_METER_STATS_REPLY] + of_object_extra_len[version][OF_METER_STATS_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_METER_STATS_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_meter_stats_reply_push_wire_types;

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
 * Create a new of_meter_stats_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_meter_stats_reply
 */

of_meter_stats_reply_t *
of_meter_stats_reply_new_from_message(of_message_t msg)
{
    of_meter_stats_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_meter_stats_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_meter_stats_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_meter_stats_reply.
 * @param obj Pointer to an object of type of_meter_stats_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_stats_reply_xid_get(
    of_meter_stats_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS_REPLY);
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
 * Set xid in an object of type of_meter_stats_reply.
 * @param obj Pointer to an object of type of_meter_stats_reply.
 * @param xid The value to write into the object
 */
void
of_meter_stats_reply_xid_set(
    of_meter_stats_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS_REPLY);
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
 * Get flags from an object of type of_meter_stats_reply.
 * @param obj Pointer to an object of type of_meter_stats_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_meter_stats_reply_flags_get(
    of_meter_stats_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS_REPLY);
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
 * Set flags in an object of type of_meter_stats_reply.
 * @param obj Pointer to an object of type of_meter_stats_reply.
 * @param flags The value to write into the object
 */
void
of_meter_stats_reply_flags_set(
    of_meter_stats_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS_REPLY);
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
 * Bind an object of type of_list_meter_stats_t to the parent of type of_meter_stats_reply for
 * member entries
 * @param obj Pointer to an object of type of_meter_stats_reply.
 * @param entries Pointer to the child object of type
 * of_list_meter_stats_t to be filled out.
 * \ingroup of_meter_stats_reply
 *
 * The parameter entries is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_meter_stats_reply_entries_bind(
    of_meter_stats_reply_t *obj,
    of_list_meter_stats_t *entries)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_METER_STATS_REPLY);
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
    of_list_meter_stats_init(entries, obj->version, 0, 1);
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
 * Create a copy of entries into a new variable of type of_list_meter_stats_t from
 * a of_meter_stats_reply instance.
 *
 * @param obj Pointer to the source of type of_meter_stats_reply_t
 * @returns A pointer to a new instance of type of_list_meter_stats_t whose contents
 * match that of entries from source
 * @returns NULL if an error occurs
 */
of_list_meter_stats_t *
of_meter_stats_reply_entries_get(of_meter_stats_reply_t *obj) {
    of_list_meter_stats_t _entries;
    of_list_meter_stats_t *_entries_ptr;

    of_meter_stats_reply_entries_bind(obj, &_entries);
    _entries_ptr = (of_list_meter_stats_t *)of_object_dup(&_entries);
    return _entries_ptr;
}

/**
 * Set entries in an object of type of_meter_stats_reply.
 * @param obj Pointer to an object of type of_meter_stats_reply.
 * @param entries Pointer to the child of type of_list_meter_stats_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_meter_stats_reply_entries_set(
    of_meter_stats_reply_t *obj,
    of_list_meter_stats_t *entries)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_METER_STATS_REPLY);
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
of_meter_stats_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0x9); /* stats_type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_meter_stats_request of_meter_stats_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_meter_stats_request_push_wire_values(of_meter_stats_request_t *obj)
{

    of_meter_stats_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_meter_stats_request object
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
 * \ingroup of_meter_stats_request
 */

of_meter_stats_request_t *
of_meter_stats_request_new(of_version_t version)
{
    of_meter_stats_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_METER_STATS_REQUEST] + of_object_extra_len[version][OF_METER_STATS_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_meter_stats_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_meter_stats_request_init(obj, version, bytes, 0);

    if (of_meter_stats_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_meter_stats_request.
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
of_meter_stats_request_init(of_meter_stats_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_METER_STATS_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_METER_STATS_REQUEST] + of_object_extra_len[version][OF_METER_STATS_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_METER_STATS_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_meter_stats_request_push_wire_types;

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
 * Create a new of_meter_stats_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_meter_stats_request
 */

of_meter_stats_request_t *
of_meter_stats_request_new_from_message(of_message_t msg)
{
    of_meter_stats_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_meter_stats_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_meter_stats_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_meter_stats_request.
 * @param obj Pointer to an object of type of_meter_stats_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_stats_request_xid_get(
    of_meter_stats_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS_REQUEST);
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
 * Set xid in an object of type of_meter_stats_request.
 * @param obj Pointer to an object of type of_meter_stats_request.
 * @param xid The value to write into the object
 */
void
of_meter_stats_request_xid_set(
    of_meter_stats_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS_REQUEST);
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
 * Get flags from an object of type of_meter_stats_request.
 * @param obj Pointer to an object of type of_meter_stats_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_meter_stats_request_flags_get(
    of_meter_stats_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS_REQUEST);
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
 * Set flags in an object of type of_meter_stats_request.
 * @param obj Pointer to an object of type of_meter_stats_request.
 * @param flags The value to write into the object
 */
void
of_meter_stats_request_flags_set(
    of_meter_stats_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS_REQUEST);
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
 * Get meter_id from an object of type of_meter_stats_request.
 * @param obj Pointer to an object of type of_meter_stats_request.
 * @param meter_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_meter_stats_request_meter_id_get(
    of_meter_stats_request_t *obj,
    uint32_t *meter_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, meter_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set meter_id in an object of type of_meter_stats_request.
 * @param obj Pointer to an object of type of_meter_stats_request.
 * @param meter_id The value to write into the object
 */
void
of_meter_stats_request_meter_id_set(
    of_meter_stats_request_t *obj,
    uint32_t meter_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_METER_STATS_REQUEST);
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
of_ofdpa_class_based_ctr_mod_msg_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0xc); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_class_based_ctr_mod_msg of_ofdpa_class_based_ctr_mod_msg
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_class_based_ctr_mod_msg_push_wire_values(of_ofdpa_class_based_ctr_mod_msg_t *obj)
{

    of_ofdpa_class_based_ctr_mod_msg_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_class_based_ctr_mod_msg object
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
 * \ingroup of_ofdpa_class_based_ctr_mod_msg
 */

of_ofdpa_class_based_ctr_mod_msg_t *
of_ofdpa_class_based_ctr_mod_msg_new(of_version_t version)
{
    of_ofdpa_class_based_ctr_mod_msg_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_CLASS_BASED_CTR_MOD_MSG] + of_object_extra_len[version][OF_OFDPA_CLASS_BASED_CTR_MOD_MSG];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_class_based_ctr_mod_msg_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_class_based_ctr_mod_msg_init(obj, version, bytes, 0);

    if (of_ofdpa_class_based_ctr_mod_msg_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_class_based_ctr_mod_msg.
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
of_ofdpa_class_based_ctr_mod_msg_init(of_ofdpa_class_based_ctr_mod_msg_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_CLASS_BASED_CTR_MOD_MSG] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_CLASS_BASED_CTR_MOD_MSG] + of_object_extra_len[version][OF_OFDPA_CLASS_BASED_CTR_MOD_MSG];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_CLASS_BASED_CTR_MOD_MSG;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_class_based_ctr_mod_msg_push_wire_types;

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
 * Create a new of_ofdpa_class_based_ctr_mod_msg object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_class_based_ctr_mod_msg
 */

of_ofdpa_class_based_ctr_mod_msg_t *
of_ofdpa_class_based_ctr_mod_msg_new_from_message(of_message_t msg)
{
    of_ofdpa_class_based_ctr_mod_msg_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_class_based_ctr_mod_msg_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_class_based_ctr_mod_msg_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_mod_msg_xid_get(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MOD_MSG);
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
 * Set xid in an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param xid The value to write into the object
 */
void
of_ofdpa_class_based_ctr_mod_msg_xid_set(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MOD_MSG);
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
 * Get experimenter from an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_mod_msg_experimenter_get(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_class_based_ctr_mod_msg_experimenter_set(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_mod_msg_subtype_get(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_class_based_ctr_mod_msg_subtype_set(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mod_command from an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param mod_command Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_mod_msg_mod_command_get(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint32_t *mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mod_command in an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param mod_command The value to write into the object
 */
void
of_ofdpa_class_based_ctr_mod_msg_mod_command_set(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint32_t mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get block_index from an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param block_index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_mod_msg_block_index_get(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint32_t *block_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, block_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set block_index in an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param block_index The value to write into the object
 */
void
of_ofdpa_class_based_ctr_mod_msg_block_index_set(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint32_t block_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, block_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get packets from an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param packets Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_mod_msg_packets_get(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint64_t *packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set packets in an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param packets The value to write into the object
 */
void
of_ofdpa_class_based_ctr_mod_msg_packets_set(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint64_t packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get bytes from an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param bytes Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_mod_msg_bytes_get(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint64_t *bytes)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, bytes);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set bytes in an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param bytes The value to write into the object
 */
void
of_ofdpa_class_based_ctr_mod_msg_bytes_set(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint64_t bytes)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, bytes);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_mod_msg_traffic_class_get(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 40;
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
 * Set traffic_class in an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_mod_msg.
 * @param traffic_class The value to write into the object
 */
void
of_ofdpa_class_based_ctr_mod_msg_traffic_class_set(
    of_ofdpa_class_based_ctr_mod_msg_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 40;
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
of_ofdpa_class_based_ctr_multipart_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0xd); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_class_based_ctr_multipart_reply of_ofdpa_class_based_ctr_multipart_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_class_based_ctr_multipart_reply_push_wire_values(of_ofdpa_class_based_ctr_multipart_reply_t *obj)
{

    of_ofdpa_class_based_ctr_multipart_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_class_based_ctr_multipart_reply object
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
 * \ingroup of_ofdpa_class_based_ctr_multipart_reply
 */

of_ofdpa_class_based_ctr_multipart_reply_t *
of_ofdpa_class_based_ctr_multipart_reply_new(of_version_t version)
{
    of_ofdpa_class_based_ctr_multipart_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY] + of_object_extra_len[version][OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_class_based_ctr_multipart_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_class_based_ctr_multipart_reply_init(obj, version, bytes, 0);

    if (of_ofdpa_class_based_ctr_multipart_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_class_based_ctr_multipart_reply.
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
of_ofdpa_class_based_ctr_multipart_reply_init(of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY] + of_object_extra_len[version][OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_class_based_ctr_multipart_reply_push_wire_types;

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
 * Create a new of_ofdpa_class_based_ctr_multipart_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_class_based_ctr_multipart_reply
 */

of_ofdpa_class_based_ctr_multipart_reply_t *
of_ofdpa_class_based_ctr_multipart_reply_new_from_message(of_message_t msg)
{
    of_ofdpa_class_based_ctr_multipart_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_class_based_ctr_multipart_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_class_based_ctr_multipart_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_multipart_reply_xid_get(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY);
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
 * Set xid in an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param xid The value to write into the object
 */
void
of_ofdpa_class_based_ctr_multipart_reply_xid_set(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY);
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
 * Get flags from an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_multipart_reply_flags_get(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY);
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
 * Set flags in an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param flags The value to write into the object
 */
void
of_ofdpa_class_based_ctr_multipart_reply_flags_set(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY);
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
 * Get experimenter from an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_multipart_reply_experimenter_get(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_class_based_ctr_multipart_reply_experimenter_set(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_multipart_reply_subtype_get(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_class_based_ctr_multipart_reply_subtype_set(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get block_index from an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param block_index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_multipart_reply_block_index_get(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint32_t *block_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, block_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set block_index in an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param block_index The value to write into the object
 */
void
of_ofdpa_class_based_ctr_multipart_reply_block_index_set(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint32_t block_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, block_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get packets from an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param packets Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_multipart_reply_packets_get(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint64_t *packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set packets in an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param packets The value to write into the object
 */
void
of_ofdpa_class_based_ctr_multipart_reply_packets_set(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint64_t packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get bytes from an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param bytes Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_multipart_reply_bytes_get(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint64_t *bytes)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, bytes);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set bytes in an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param bytes The value to write into the object
 */
void
of_ofdpa_class_based_ctr_multipart_reply_bytes_set(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint64_t bytes)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, bytes);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get reference_count from an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param reference_count Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_multipart_reply_reference_count_get(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint64_t *reference_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 48;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, reference_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set reference_count in an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param reference_count The value to write into the object
 */
void
of_ofdpa_class_based_ctr_multipart_reply_reference_count_set(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint64_t reference_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 48;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, reference_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_multipart_reply_traffic_class_get(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 56;
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
 * Set traffic_class in an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_reply.
 * @param traffic_class The value to write into the object
 */
void
of_ofdpa_class_based_ctr_multipart_reply_traffic_class_set(
    of_ofdpa_class_based_ctr_multipart_reply_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 56;
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
of_ofdpa_class_based_ctr_multipart_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0xd); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_class_based_ctr_multipart_request of_ofdpa_class_based_ctr_multipart_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_class_based_ctr_multipart_request_push_wire_values(of_ofdpa_class_based_ctr_multipart_request_t *obj)
{

    of_ofdpa_class_based_ctr_multipart_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_class_based_ctr_multipart_request object
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
 * \ingroup of_ofdpa_class_based_ctr_multipart_request
 */

of_ofdpa_class_based_ctr_multipart_request_t *
of_ofdpa_class_based_ctr_multipart_request_new(of_version_t version)
{
    of_ofdpa_class_based_ctr_multipart_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REQUEST] + of_object_extra_len[version][OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_class_based_ctr_multipart_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_class_based_ctr_multipart_request_init(obj, version, bytes, 0);

    if (of_ofdpa_class_based_ctr_multipart_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_class_based_ctr_multipart_request.
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
of_ofdpa_class_based_ctr_multipart_request_init(of_ofdpa_class_based_ctr_multipart_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REQUEST] + of_object_extra_len[version][OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_class_based_ctr_multipart_request_push_wire_types;

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
 * Create a new of_ofdpa_class_based_ctr_multipart_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_class_based_ctr_multipart_request
 */

of_ofdpa_class_based_ctr_multipart_request_t *
of_ofdpa_class_based_ctr_multipart_request_new_from_message(of_message_t msg)
{
    of_ofdpa_class_based_ctr_multipart_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_class_based_ctr_multipart_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_class_based_ctr_multipart_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_multipart_request_xid_get(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REQUEST);
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
 * Set xid in an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param xid The value to write into the object
 */
void
of_ofdpa_class_based_ctr_multipart_request_xid_set(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REQUEST);
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
 * Get flags from an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_multipart_request_flags_get(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REQUEST);
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
 * Set flags in an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param flags The value to write into the object
 */
void
of_ofdpa_class_based_ctr_multipart_request_flags_set(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REQUEST);
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
 * Get experimenter from an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_multipart_request_experimenter_get(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_class_based_ctr_multipart_request_experimenter_set(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_multipart_request_subtype_get(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_class_based_ctr_multipart_request_subtype_set(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get block_index from an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param block_index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_multipart_request_block_index_get(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint32_t *block_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, block_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set block_index in an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param block_index The value to write into the object
 */
void
of_ofdpa_class_based_ctr_multipart_request_block_index_set(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint32_t block_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, block_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_class_based_ctr_multipart_request_traffic_class_get(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
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
 * Set traffic_class in an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_class_based_ctr_multipart_request.
 * @param traffic_class The value to write into the object
 */
void
of_ofdpa_class_based_ctr_multipart_request_traffic_class_set(
    of_ofdpa_class_based_ctr_multipart_request_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_CLASS_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
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
of_ofdpa_color_based_ctr_mod_msg_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0xe); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_color_based_ctr_mod_msg of_ofdpa_color_based_ctr_mod_msg
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_color_based_ctr_mod_msg_push_wire_values(of_ofdpa_color_based_ctr_mod_msg_t *obj)
{

    of_ofdpa_color_based_ctr_mod_msg_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_color_based_ctr_mod_msg object
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
 * \ingroup of_ofdpa_color_based_ctr_mod_msg
 */

of_ofdpa_color_based_ctr_mod_msg_t *
of_ofdpa_color_based_ctr_mod_msg_new(of_version_t version)
{
    of_ofdpa_color_based_ctr_mod_msg_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_COLOR_BASED_CTR_MOD_MSG] + of_object_extra_len[version][OF_OFDPA_COLOR_BASED_CTR_MOD_MSG];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_color_based_ctr_mod_msg_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_color_based_ctr_mod_msg_init(obj, version, bytes, 0);

    if (of_ofdpa_color_based_ctr_mod_msg_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_color_based_ctr_mod_msg.
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
of_ofdpa_color_based_ctr_mod_msg_init(of_ofdpa_color_based_ctr_mod_msg_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_COLOR_BASED_CTR_MOD_MSG] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_COLOR_BASED_CTR_MOD_MSG] + of_object_extra_len[version][OF_OFDPA_COLOR_BASED_CTR_MOD_MSG];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_COLOR_BASED_CTR_MOD_MSG;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_color_based_ctr_mod_msg_push_wire_types;

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
 * Create a new of_ofdpa_color_based_ctr_mod_msg object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_color_based_ctr_mod_msg
 */

of_ofdpa_color_based_ctr_mod_msg_t *
of_ofdpa_color_based_ctr_mod_msg_new_from_message(of_message_t msg)
{
    of_ofdpa_color_based_ctr_mod_msg_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_color_based_ctr_mod_msg_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_color_based_ctr_mod_msg_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_mod_msg_xid_get(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MOD_MSG);
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
 * Set xid in an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param xid The value to write into the object
 */
void
of_ofdpa_color_based_ctr_mod_msg_xid_set(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MOD_MSG);
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
 * Get experimenter from an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_mod_msg_experimenter_get(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_color_based_ctr_mod_msg_experimenter_set(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_mod_msg_subtype_get(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_color_based_ctr_mod_msg_subtype_set(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mod_command from an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param mod_command Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_mod_msg_mod_command_get(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint32_t *mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mod_command in an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param mod_command The value to write into the object
 */
void
of_ofdpa_color_based_ctr_mod_msg_mod_command_set(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint32_t mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get block_index from an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param block_index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_mod_msg_block_index_get(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint32_t *block_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, block_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set block_index in an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param block_index The value to write into the object
 */
void
of_ofdpa_color_based_ctr_mod_msg_block_index_set(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint32_t block_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, block_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get packets from an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param packets Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_mod_msg_packets_get(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint64_t *packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set packets in an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param packets The value to write into the object
 */
void
of_ofdpa_color_based_ctr_mod_msg_packets_set(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint64_t packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get bytes from an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param bytes Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_mod_msg_bytes_get(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint64_t *bytes)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, bytes);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set bytes in an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param bytes The value to write into the object
 */
void
of_ofdpa_color_based_ctr_mod_msg_bytes_set(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint64_t bytes)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, bytes);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get color from an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_mod_msg_color_get(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 40;
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
 * Set color in an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_mod_msg.
 * @param color The value to write into the object
 */
void
of_ofdpa_color_based_ctr_mod_msg_color_set(
    of_ofdpa_color_based_ctr_mod_msg_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 40;
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

static void
of_ofdpa_color_based_ctr_multipart_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0xf); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_color_based_ctr_multipart_reply of_ofdpa_color_based_ctr_multipart_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_color_based_ctr_multipart_reply_push_wire_values(of_ofdpa_color_based_ctr_multipart_reply_t *obj)
{

    of_ofdpa_color_based_ctr_multipart_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_color_based_ctr_multipart_reply object
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
 * \ingroup of_ofdpa_color_based_ctr_multipart_reply
 */

of_ofdpa_color_based_ctr_multipart_reply_t *
of_ofdpa_color_based_ctr_multipart_reply_new(of_version_t version)
{
    of_ofdpa_color_based_ctr_multipart_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY] + of_object_extra_len[version][OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_color_based_ctr_multipart_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_color_based_ctr_multipart_reply_init(obj, version, bytes, 0);

    if (of_ofdpa_color_based_ctr_multipart_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_color_based_ctr_multipart_reply.
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
of_ofdpa_color_based_ctr_multipart_reply_init(of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY] + of_object_extra_len[version][OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_color_based_ctr_multipart_reply_push_wire_types;

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
 * Create a new of_ofdpa_color_based_ctr_multipart_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_color_based_ctr_multipart_reply
 */

of_ofdpa_color_based_ctr_multipart_reply_t *
of_ofdpa_color_based_ctr_multipart_reply_new_from_message(of_message_t msg)
{
    of_ofdpa_color_based_ctr_multipart_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_color_based_ctr_multipart_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_color_based_ctr_multipart_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_multipart_reply_xid_get(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY);
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
 * Set xid in an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param xid The value to write into the object
 */
void
of_ofdpa_color_based_ctr_multipart_reply_xid_set(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY);
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
 * Get flags from an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_multipart_reply_flags_get(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY);
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
 * Set flags in an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param flags The value to write into the object
 */
void
of_ofdpa_color_based_ctr_multipart_reply_flags_set(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY);
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
 * Get experimenter from an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_multipart_reply_experimenter_get(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_color_based_ctr_multipart_reply_experimenter_set(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_multipart_reply_subtype_get(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_color_based_ctr_multipart_reply_subtype_set(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get block_index from an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param block_index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_multipart_reply_block_index_get(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint32_t *block_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, block_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set block_index in an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param block_index The value to write into the object
 */
void
of_ofdpa_color_based_ctr_multipart_reply_block_index_set(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint32_t block_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, block_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get packets from an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param packets Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_multipart_reply_packets_get(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint64_t *packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set packets in an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param packets The value to write into the object
 */
void
of_ofdpa_color_based_ctr_multipart_reply_packets_set(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint64_t packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get bytes from an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param bytes Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_multipart_reply_bytes_get(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint64_t *bytes)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, bytes);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set bytes in an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param bytes The value to write into the object
 */
void
of_ofdpa_color_based_ctr_multipart_reply_bytes_set(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint64_t bytes)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, bytes);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get reference_count from an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param reference_count Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_multipart_reply_reference_count_get(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint64_t *reference_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 48;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, reference_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set reference_count in an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param reference_count The value to write into the object
 */
void
of_ofdpa_color_based_ctr_multipart_reply_reference_count_set(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint64_t reference_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 48;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, reference_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get color from an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_multipart_reply_color_get(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 56;
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
 * Set color in an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_reply.
 * @param color The value to write into the object
 */
void
of_ofdpa_color_based_ctr_multipart_reply_color_set(
    of_ofdpa_color_based_ctr_multipart_reply_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 56;
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

static void
of_ofdpa_color_based_ctr_multipart_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0xf); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_color_based_ctr_multipart_request of_ofdpa_color_based_ctr_multipart_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_color_based_ctr_multipart_request_push_wire_values(of_ofdpa_color_based_ctr_multipart_request_t *obj)
{

    of_ofdpa_color_based_ctr_multipart_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_color_based_ctr_multipart_request object
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
 * \ingroup of_ofdpa_color_based_ctr_multipart_request
 */

of_ofdpa_color_based_ctr_multipart_request_t *
of_ofdpa_color_based_ctr_multipart_request_new(of_version_t version)
{
    of_ofdpa_color_based_ctr_multipart_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST] + of_object_extra_len[version][OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_color_based_ctr_multipart_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_color_based_ctr_multipart_request_init(obj, version, bytes, 0);

    if (of_ofdpa_color_based_ctr_multipart_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_color_based_ctr_multipart_request.
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
of_ofdpa_color_based_ctr_multipart_request_init(of_ofdpa_color_based_ctr_multipart_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST] + of_object_extra_len[version][OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_color_based_ctr_multipart_request_push_wire_types;

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
 * Create a new of_ofdpa_color_based_ctr_multipart_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_color_based_ctr_multipart_request
 */

of_ofdpa_color_based_ctr_multipart_request_t *
of_ofdpa_color_based_ctr_multipart_request_new_from_message(of_message_t msg)
{
    of_ofdpa_color_based_ctr_multipart_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_color_based_ctr_multipart_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_color_based_ctr_multipart_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_multipart_request_xid_get(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
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
 * Set xid in an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param xid The value to write into the object
 */
void
of_ofdpa_color_based_ctr_multipart_request_xid_set(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
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
 * Get flags from an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_multipart_request_flags_get(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
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
 * Set flags in an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param flags The value to write into the object
 */
void
of_ofdpa_color_based_ctr_multipart_request_flags_set(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
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
 * Get experimenter from an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_multipart_request_experimenter_get(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_color_based_ctr_multipart_request_experimenter_set(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_multipart_request_subtype_get(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_color_based_ctr_multipart_request_subtype_set(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get block_index from an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param block_index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_multipart_request_block_index_get(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t *block_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, block_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set block_index in an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param block_index The value to write into the object
 */
void
of_ofdpa_color_based_ctr_multipart_request_block_index_set(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint32_t block_index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, block_index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get color from an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_color_based_ctr_multipart_request_color_get(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
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
 * Set color in an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_color_based_ctr_multipart_request.
 * @param color The value to write into the object
 */
void
of_ofdpa_color_based_ctr_multipart_request_color_set(
    of_ofdpa_color_based_ctr_multipart_request_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_COLOR_BASED_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
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

static void
of_ofdpa_l2_interface_remark_action_mod_msg_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0xa); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_l2_interface_remark_action_mod_msg of_ofdpa_l2_interface_remark_action_mod_msg
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_l2_interface_remark_action_mod_msg_push_wire_values(of_ofdpa_l2_interface_remark_action_mod_msg_t *obj)
{

    of_ofdpa_l2_interface_remark_action_mod_msg_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_l2_interface_remark_action_mod_msg object
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
 * \ingroup of_ofdpa_l2_interface_remark_action_mod_msg
 */

of_ofdpa_l2_interface_remark_action_mod_msg_t *
of_ofdpa_l2_interface_remark_action_mod_msg_new(of_version_t version)
{
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG] + of_object_extra_len[version][OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_l2_interface_remark_action_mod_msg_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_l2_interface_remark_action_mod_msg_init(obj, version, bytes, 0);

    if (of_ofdpa_l2_interface_remark_action_mod_msg_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
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
of_ofdpa_l2_interface_remark_action_mod_msg_init(of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG] + of_object_extra_len[version][OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_l2_interface_remark_action_mod_msg_push_wire_types;

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
 * Create a new of_ofdpa_l2_interface_remark_action_mod_msg object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_l2_interface_remark_action_mod_msg
 */

of_ofdpa_l2_interface_remark_action_mod_msg_t *
of_ofdpa_l2_interface_remark_action_mod_msg_new_from_message(of_message_t msg)
{
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_l2_interface_remark_action_mod_msg_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_l2_interface_remark_action_mod_msg_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_mod_msg_xid_get(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
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
 * Set xid in an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param xid The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_mod_msg_xid_set(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
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
 * Get experimenter from an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_mod_msg_experimenter_get(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_mod_msg_experimenter_set(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_mod_msg_subtype_get(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_mod_msg_subtype_set(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mod_command from an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param mod_command Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_mod_msg_mod_command_get(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t *mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mod_command in an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param mod_command The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_mod_msg_mod_command_set(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_mod_msg_index_get(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set index in an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param index The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_mod_msg_index_set(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_mod_msg_traffic_class_get(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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
 * Set traffic_class in an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param traffic_class The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_mod_msg_traffic_class_set(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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
 * Get color from an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_mod_msg_color_get(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 25;
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
 * Set color in an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param color The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_mod_msg_color_set(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 25;
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

/**
 * Get dscp from an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param dscp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_mod_msg_dscp_get(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t *dscp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, dscp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set dscp in an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param dscp The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_mod_msg_dscp_set(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t dscp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, dscp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_pcp from an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param vlan_pcp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_mod_msg_vlan_pcp_get(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t *vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 27;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_pcp in an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param vlan_pcp The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_mod_msg_vlan_pcp_set(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 27;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_dei from an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param vlan_dei Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_mod_msg_vlan_dei_get(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t *vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_dei in an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_mod_msg.
 * @param vlan_dei The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_mod_msg_vlan_dei_set(
    of_ofdpa_l2_interface_remark_action_mod_msg_t *obj,
    uint8_t vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_dei);

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
of_ofdpa_l2_interface_remark_action_multipart_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0xb); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_l2_interface_remark_action_multipart_reply of_ofdpa_l2_interface_remark_action_multipart_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_l2_interface_remark_action_multipart_reply_push_wire_values(of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj)
{

    of_ofdpa_l2_interface_remark_action_multipart_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_l2_interface_remark_action_multipart_reply object
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
 * \ingroup of_ofdpa_l2_interface_remark_action_multipart_reply
 */

of_ofdpa_l2_interface_remark_action_multipart_reply_t *
of_ofdpa_l2_interface_remark_action_multipart_reply_new(of_version_t version)
{
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY] + of_object_extra_len[version][OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_l2_interface_remark_action_multipart_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_l2_interface_remark_action_multipart_reply_init(obj, version, bytes, 0);

    if (of_ofdpa_l2_interface_remark_action_multipart_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
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
of_ofdpa_l2_interface_remark_action_multipart_reply_init(of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY] + of_object_extra_len[version][OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_l2_interface_remark_action_multipart_reply_push_wire_types;

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
 * Create a new of_ofdpa_l2_interface_remark_action_multipart_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_l2_interface_remark_action_multipart_reply
 */

of_ofdpa_l2_interface_remark_action_multipart_reply_t *
of_ofdpa_l2_interface_remark_action_multipart_reply_new_from_message(of_message_t msg)
{
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_l2_interface_remark_action_multipart_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_l2_interface_remark_action_multipart_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_multipart_reply_xid_get(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
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
 * Set xid in an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param xid The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_multipart_reply_xid_set(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
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
 * Get flags from an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_multipart_reply_flags_get(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
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
 * Set flags in an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param flags The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_multipart_reply_flags_set(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
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
 * Get experimenter from an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_multipart_reply_experimenter_get(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_multipart_reply_experimenter_set(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_multipart_reply_subtype_get(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_multipart_reply_subtype_set(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_multipart_reply_index_get(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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
 * Set index in an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param index The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_multipart_reply_index_set(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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

/**
 * Get traffic_class from an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_multipart_reply_traffic_class_get(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
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
 * Set traffic_class in an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param traffic_class The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_multipart_reply_traffic_class_set(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
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
 * Get color from an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_multipart_reply_color_get(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
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
 * Set color in an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param color The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_multipart_reply_color_set(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
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

/**
 * Get mpls_tc from an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_multipart_reply_mpls_tc_get(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mpls_tc in an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param mpls_tc The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_multipart_reply_mpls_tc_set(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_pcp from an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param vlan_pcp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_multipart_reply_vlan_pcp_get(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t *vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_pcp in an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param vlan_pcp The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_multipart_reply_vlan_pcp_set(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_dei from an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param vlan_dei Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_multipart_reply_vlan_dei_get(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t *vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_dei in an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_reply.
 * @param vlan_dei The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_multipart_reply_vlan_dei_set(
    of_ofdpa_l2_interface_remark_action_multipart_reply_t *obj,
    uint8_t vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_dei);

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
of_ofdpa_l2_interface_remark_action_multipart_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0xb); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_l2_interface_remark_action_multipart_request of_ofdpa_l2_interface_remark_action_multipart_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_l2_interface_remark_action_multipart_request_push_wire_values(of_ofdpa_l2_interface_remark_action_multipart_request_t *obj)
{

    of_ofdpa_l2_interface_remark_action_multipart_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_l2_interface_remark_action_multipart_request object
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
 * \ingroup of_ofdpa_l2_interface_remark_action_multipart_request
 */

of_ofdpa_l2_interface_remark_action_multipart_request_t *
of_ofdpa_l2_interface_remark_action_multipart_request_new(of_version_t version)
{
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST] + of_object_extra_len[version][OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_l2_interface_remark_action_multipart_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_l2_interface_remark_action_multipart_request_init(obj, version, bytes, 0);

    if (of_ofdpa_l2_interface_remark_action_multipart_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
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
of_ofdpa_l2_interface_remark_action_multipart_request_init(of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST] + of_object_extra_len[version][OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_l2_interface_remark_action_multipart_request_push_wire_types;

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
 * Create a new of_ofdpa_l2_interface_remark_action_multipart_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_l2_interface_remark_action_multipart_request
 */

of_ofdpa_l2_interface_remark_action_multipart_request_t *
of_ofdpa_l2_interface_remark_action_multipart_request_new_from_message(of_message_t msg)
{
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_l2_interface_remark_action_multipart_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_l2_interface_remark_action_multipart_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_multipart_request_xid_get(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
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
 * Set xid in an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param xid The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_multipart_request_xid_set(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
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
 * Get flags from an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_multipart_request_flags_get(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
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
 * Set flags in an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param flags The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_multipart_request_flags_set(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
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
 * Get experimenter from an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_multipart_request_experimenter_get(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_multipart_request_experimenter_set(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_multipart_request_subtype_get(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_multipart_request_subtype_set(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_multipart_request_index_get(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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
 * Set index in an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param index The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_multipart_request_index_set(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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

/**
 * Get traffic_class from an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_multipart_request_traffic_class_get(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
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
 * Set traffic_class in an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param traffic_class The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_multipart_request_traffic_class_set(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
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
 * Get color from an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_multipart_request_color_get(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
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
 * Set color in an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param color The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_multipart_request_color_set(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
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

/**
 * Get mpls_tc from an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_multipart_request_mpls_tc_get(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mpls_tc in an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param mpls_tc The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_multipart_request_mpls_tc_set(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_pcp from an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param vlan_pcp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_multipart_request_vlan_pcp_get(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t *vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_pcp in an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param vlan_pcp The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_multipart_request_vlan_pcp_set(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_dei from an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param vlan_dei Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_l2_interface_remark_action_multipart_request_vlan_dei_get(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t *vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_dei in an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_l2_interface_remark_action_multipart_request.
 * @param vlan_dei The value to write into the object
 */
void
of_ofdpa_l2_interface_remark_action_multipart_request_vlan_dei_set(
    of_ofdpa_l2_interface_remark_action_multipart_request_t *obj,
    uint8_t vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_L2_INTERFACE_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_dei);

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
of_ofdpa_mpls_tunnel_label_multipart_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x9); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_mpls_tunnel_label_multipart_reply of_ofdpa_mpls_tunnel_label_multipart_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_mpls_tunnel_label_multipart_reply_push_wire_values(of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj)
{

    of_ofdpa_mpls_tunnel_label_multipart_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_mpls_tunnel_label_multipart_reply object
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
 * \ingroup of_ofdpa_mpls_tunnel_label_multipart_reply
 */

of_ofdpa_mpls_tunnel_label_multipart_reply_t *
of_ofdpa_mpls_tunnel_label_multipart_reply_new(of_version_t version)
{
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY] + of_object_extra_len[version][OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_mpls_tunnel_label_multipart_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_mpls_tunnel_label_multipart_reply_init(obj, version, bytes, 0);

    if (of_ofdpa_mpls_tunnel_label_multipart_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
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
of_ofdpa_mpls_tunnel_label_multipart_reply_init(of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY] + of_object_extra_len[version][OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_mpls_tunnel_label_multipart_reply_push_wire_types;

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
 * Create a new of_ofdpa_mpls_tunnel_label_multipart_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_mpls_tunnel_label_multipart_reply
 */

of_ofdpa_mpls_tunnel_label_multipart_reply_t *
of_ofdpa_mpls_tunnel_label_multipart_reply_new_from_message(of_message_t msg)
{
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_mpls_tunnel_label_multipart_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_mpls_tunnel_label_multipart_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_multipart_reply_xid_get(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
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
 * Set xid in an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param xid The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_multipart_reply_xid_set(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
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
 * Get flags from an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_multipart_reply_flags_get(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
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
 * Set flags in an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param flags The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_multipart_reply_flags_set(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
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
 * Get experimenter from an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_multipart_reply_experimenter_get(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_multipart_reply_experimenter_set(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_multipart_reply_subtype_get(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_multipart_reply_subtype_set(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_multipart_reply_index_get(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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
 * Set index in an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param index The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_multipart_reply_index_set(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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

/**
 * Get traffic_class from an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_multipart_reply_traffic_class_get(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
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
 * Set traffic_class in an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param traffic_class The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_multipart_reply_traffic_class_set(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
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
 * Get color from an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_multipart_reply_color_get(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
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
 * Set color in an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param color The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_multipart_reply_color_set(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
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

/**
 * Get mpls_tc from an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_multipart_reply_mpls_tc_get(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mpls_tc in an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param mpls_tc The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_multipart_reply_mpls_tc_set(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_pcp from an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param vlan_pcp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_multipart_reply_vlan_pcp_get(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t *vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_pcp in an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param vlan_pcp The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_multipart_reply_vlan_pcp_set(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_dei from an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param vlan_dei Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_multipart_reply_vlan_dei_get(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t *vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_dei in an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_reply.
 * @param vlan_dei The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_multipart_reply_vlan_dei_set(
    of_ofdpa_mpls_tunnel_label_multipart_reply_t *obj,
    uint8_t vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_dei);

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
of_ofdpa_mpls_tunnel_label_multipart_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x9); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_mpls_tunnel_label_multipart_request of_ofdpa_mpls_tunnel_label_multipart_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_mpls_tunnel_label_multipart_request_push_wire_values(of_ofdpa_mpls_tunnel_label_multipart_request_t *obj)
{

    of_ofdpa_mpls_tunnel_label_multipart_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_mpls_tunnel_label_multipart_request object
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
 * \ingroup of_ofdpa_mpls_tunnel_label_multipart_request
 */

of_ofdpa_mpls_tunnel_label_multipart_request_t *
of_ofdpa_mpls_tunnel_label_multipart_request_new(of_version_t version)
{
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST] + of_object_extra_len[version][OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_mpls_tunnel_label_multipart_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_mpls_tunnel_label_multipart_request_init(obj, version, bytes, 0);

    if (of_ofdpa_mpls_tunnel_label_multipart_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
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
of_ofdpa_mpls_tunnel_label_multipart_request_init(of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST] + of_object_extra_len[version][OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_mpls_tunnel_label_multipart_request_push_wire_types;

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
 * Create a new of_ofdpa_mpls_tunnel_label_multipart_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_mpls_tunnel_label_multipart_request
 */

of_ofdpa_mpls_tunnel_label_multipart_request_t *
of_ofdpa_mpls_tunnel_label_multipart_request_new_from_message(of_message_t msg)
{
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_mpls_tunnel_label_multipart_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_mpls_tunnel_label_multipart_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_multipart_request_xid_get(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
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
 * Set xid in an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param xid The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_multipart_request_xid_set(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
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
 * Get flags from an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_multipart_request_flags_get(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
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
 * Set flags in an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param flags The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_multipart_request_flags_set(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
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
 * Get experimenter from an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_multipart_request_experimenter_get(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_multipart_request_experimenter_set(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_multipart_request_subtype_get(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_multipart_request_subtype_set(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_multipart_request_index_get(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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
 * Set index in an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param index The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_multipart_request_index_set(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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

/**
 * Get traffic_class from an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_multipart_request_traffic_class_get(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
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
 * Set traffic_class in an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param traffic_class The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_multipart_request_traffic_class_set(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
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
 * Get color from an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_multipart_request_color_get(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
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
 * Set color in an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param color The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_multipart_request_color_set(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
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

/**
 * Get mpls_tc from an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_multipart_request_mpls_tc_get(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mpls_tc in an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param mpls_tc The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_multipart_request_mpls_tc_set(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_pcp from an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param vlan_pcp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_multipart_request_vlan_pcp_get(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t *vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_pcp in an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param vlan_pcp The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_multipart_request_vlan_pcp_set(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_dei from an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param vlan_dei Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_multipart_request_vlan_dei_get(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t *vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_dei in an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_multipart_request.
 * @param vlan_dei The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_multipart_request_vlan_dei_set(
    of_ofdpa_mpls_tunnel_label_multipart_request_t *obj,
    uint8_t vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_dei);

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
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0x8); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_mpls_tunnel_label_remark_action_mod_msg of_ofdpa_mpls_tunnel_label_remark_action_mod_msg
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_push_wire_values(of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj)
{

    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_mpls_tunnel_label_remark_action_mod_msg object
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
 * \ingroup of_ofdpa_mpls_tunnel_label_remark_action_mod_msg
 */

of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_new(of_version_t version)
{
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG] + of_object_extra_len[version][OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_init(obj, version, bytes, 0);

    if (of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
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
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_init(of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG] + of_object_extra_len[version][OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_push_wire_types;

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
 * Create a new of_ofdpa_mpls_tunnel_label_remark_action_mod_msg object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_mpls_tunnel_label_remark_action_mod_msg
 */

of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_new_from_message(of_message_t msg)
{
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_xid_get(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
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
 * Set xid in an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param xid The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_xid_set(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
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
 * Get experimenter from an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_experimenter_get(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_experimenter_set(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_subtype_get(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_subtype_set(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mod_command from an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param mod_command Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_mod_command_get(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t *mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mod_command in an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param mod_command The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_mod_command_set(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_index_get(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set index in an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param index The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_index_set(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_traffic_class_get(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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
 * Set traffic_class in an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param traffic_class The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_traffic_class_set(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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
 * Get color from an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_color_get(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 25;
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
 * Set color in an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param color The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_color_set(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 25;
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

/**
 * Get mpls_tc from an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_mpls_tc_get(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mpls_tc in an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param mpls_tc The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_mpls_tc_set(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_pcp from an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param vlan_pcp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_vlan_pcp_get(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t *vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 27;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_pcp in an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param vlan_pcp The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_vlan_pcp_set(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 27;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_dei from an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param vlan_dei Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_vlan_dei_get(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t *vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_dei in an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_tunnel_label_remark_action_mod_msg.
 * @param vlan_dei The value to write into the object
 */
void
of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_vlan_dei_set(
    of_ofdpa_mpls_tunnel_label_remark_action_mod_msg_t *obj,
    uint8_t vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_TUNNEL_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_dei);

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
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0x6); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_mpls_vpn_label_remark_action_mod_msg of_ofdpa_mpls_vpn_label_remark_action_mod_msg
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_push_wire_values(of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj)
{

    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_mpls_vpn_label_remark_action_mod_msg object
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
 * \ingroup of_ofdpa_mpls_vpn_label_remark_action_mod_msg
 */

of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_new(of_version_t version)
{
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG] + of_object_extra_len[version][OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_init(obj, version, bytes, 0);

    if (of_ofdpa_mpls_vpn_label_remark_action_mod_msg_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
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
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_init(of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG] + of_object_extra_len[version][OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_mpls_vpn_label_remark_action_mod_msg_push_wire_types;

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
 * Create a new of_ofdpa_mpls_vpn_label_remark_action_mod_msg object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_mpls_vpn_label_remark_action_mod_msg
 */

of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_new_from_message(of_message_t msg)
{
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_xid_get(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
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
 * Set xid in an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param xid The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_xid_set(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
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
 * Get experimenter from an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_experimenter_get(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_experimenter_set(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_subtype_get(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_subtype_set(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mod_command from an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param mod_command Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_mod_command_get(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t *mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mod_command in an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param mod_command The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_mod_command_set(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_index_get(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set index in an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param index The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_index_set(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_traffic_class_get(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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
 * Set traffic_class in an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param traffic_class The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_traffic_class_set(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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
 * Get color from an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_color_get(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 25;
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
 * Set color in an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param color The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_color_set(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 25;
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

/**
 * Get mpls_tc from an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_mpls_tc_get(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mpls_tc in an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param mpls_tc The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_mpls_tc_set(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_pcp from an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param vlan_pcp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_vlan_pcp_get(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t *vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 27;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_pcp in an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param vlan_pcp The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_vlan_pcp_set(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 27;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_dei from an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param vlan_dei Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_vlan_dei_get(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t *vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_dei in an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_mod_msg.
 * @param vlan_dei The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_mod_msg_vlan_dei_set(
    of_ofdpa_mpls_vpn_label_remark_action_mod_msg_t *obj,
    uint8_t vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_dei);

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
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x7); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_mpls_vpn_label_remark_action_multipart_reply of_ofdpa_mpls_vpn_label_remark_action_multipart_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_push_wire_values(of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj)
{

    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_mpls_vpn_label_remark_action_multipart_reply object
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
 * \ingroup of_ofdpa_mpls_vpn_label_remark_action_multipart_reply
 */

of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_new(of_version_t version)
{
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY] + of_object_extra_len[version][OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_init(obj, version, bytes, 0);

    if (of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
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
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_init(of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY] + of_object_extra_len[version][OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_push_wire_types;

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
 * Create a new of_ofdpa_mpls_vpn_label_remark_action_multipart_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_mpls_vpn_label_remark_action_multipart_reply
 */

of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_new_from_message(of_message_t msg)
{
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_xid_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
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
 * Set xid in an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param xid The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_xid_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
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
 * Get flags from an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_flags_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
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
 * Set flags in an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param flags The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_flags_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
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
 * Get experimenter from an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_experimenter_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_experimenter_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_subtype_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_subtype_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_index_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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
 * Set index in an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param index The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_index_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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

/**
 * Get traffic_class from an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_traffic_class_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
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
 * Set traffic_class in an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param traffic_class The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_traffic_class_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
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
 * Get color from an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_color_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
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
 * Set color in an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param color The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_color_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
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

/**
 * Get mpls_tc from an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_mpls_tc_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mpls_tc in an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param mpls_tc The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_mpls_tc_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_pcp from an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param vlan_pcp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_vlan_pcp_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t *vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_pcp in an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param vlan_pcp The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_vlan_pcp_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_dei from an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param vlan_dei Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_vlan_dei_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t *vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_dei in an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_reply.
 * @param vlan_dei The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_vlan_dei_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_reply_t *obj,
    uint8_t vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_dei);

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
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x7); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_mpls_vpn_label_remark_action_multipart_request of_ofdpa_mpls_vpn_label_remark_action_multipart_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_push_wire_values(of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj)
{

    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_mpls_vpn_label_remark_action_multipart_request object
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
 * \ingroup of_ofdpa_mpls_vpn_label_remark_action_multipart_request
 */

of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_new(of_version_t version)
{
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST] + of_object_extra_len[version][OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_init(obj, version, bytes, 0);

    if (of_ofdpa_mpls_vpn_label_remark_action_multipart_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
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
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_init(of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST] + of_object_extra_len[version][OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_mpls_vpn_label_remark_action_multipart_request_push_wire_types;

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
 * Create a new of_ofdpa_mpls_vpn_label_remark_action_multipart_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_mpls_vpn_label_remark_action_multipart_request
 */

of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_new_from_message(of_message_t msg)
{
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_xid_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
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
 * Set xid in an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param xid The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_xid_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
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
 * Get flags from an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_flags_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
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
 * Set flags in an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param flags The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_flags_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
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
 * Get experimenter from an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_experimenter_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_experimenter_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_subtype_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_subtype_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_index_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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
 * Set index in an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param index The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_index_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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

/**
 * Get traffic_class from an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_traffic_class_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
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
 * Set traffic_class in an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param traffic_class The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_traffic_class_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
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
 * Get color from an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param color Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_color_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t *color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
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
 * Set color in an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param color The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_color_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t color)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
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

/**
 * Get mpls_tc from an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param mpls_tc Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_mpls_tc_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t *mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mpls_tc in an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param mpls_tc The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_mpls_tc_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t mpls_tc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, mpls_tc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_pcp from an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param vlan_pcp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_vlan_pcp_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t *vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_pcp in an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param vlan_pcp The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_vlan_pcp_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 31;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_pcp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_dei from an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param vlan_dei Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_vlan_dei_get(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t *vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, vlan_dei);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_dei in an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_mpls_vpn_label_remark_action_multipart_request.
 * @param vlan_dei The value to write into the object
 */
void
of_ofdpa_mpls_vpn_label_remark_action_multipart_request_vlan_dei_set(
    of_ofdpa_mpls_vpn_label_remark_action_multipart_request_t *obj,
    uint8_t vlan_dei)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_MPLS_VPN_LABEL_REMARK_ACTION_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, vlan_dei);

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
of_ofdpa_oam_dataplane_ctr_mod_msg_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0x2); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_oam_dataplane_ctr_mod_msg of_ofdpa_oam_dataplane_ctr_mod_msg
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_oam_dataplane_ctr_mod_msg_push_wire_values(of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj)
{

    of_ofdpa_oam_dataplane_ctr_mod_msg_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_oam_dataplane_ctr_mod_msg object
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
 * \ingroup of_ofdpa_oam_dataplane_ctr_mod_msg
 */

of_ofdpa_oam_dataplane_ctr_mod_msg_t *
of_ofdpa_oam_dataplane_ctr_mod_msg_new(of_version_t version)
{
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG] + of_object_extra_len[version][OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_oam_dataplane_ctr_mod_msg_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_oam_dataplane_ctr_mod_msg_init(obj, version, bytes, 0);

    if (of_ofdpa_oam_dataplane_ctr_mod_msg_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
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
of_ofdpa_oam_dataplane_ctr_mod_msg_init(of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG] + of_object_extra_len[version][OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_oam_dataplane_ctr_mod_msg_push_wire_types;

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
 * Create a new of_ofdpa_oam_dataplane_ctr_mod_msg object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_oam_dataplane_ctr_mod_msg
 */

of_ofdpa_oam_dataplane_ctr_mod_msg_t *
of_ofdpa_oam_dataplane_ctr_mod_msg_new_from_message(of_message_t msg)
{
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_oam_dataplane_ctr_mod_msg_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_oam_dataplane_ctr_mod_msg_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_mod_msg_xid_get(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
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
 * Set xid in an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param xid The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_mod_msg_xid_set(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
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
 * Get experimenter from an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_mod_msg_experimenter_get(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_mod_msg_experimenter_set(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_mod_msg_subtype_get(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_mod_msg_subtype_set(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mod_command from an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param mod_command Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_mod_msg_mod_command_get(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t *mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mod_command in an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param mod_command The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_mod_msg_mod_command_set(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get lmep_id from an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param lmep_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_mod_msg_lmep_id_get(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t *lmep_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, lmep_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set lmep_id in an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param lmep_id The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_mod_msg_lmep_id_set(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint32_t lmep_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, lmep_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get transmit_packets from an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param transmit_packets Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_mod_msg_transmit_packets_get(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint64_t *transmit_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, transmit_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set transmit_packets in an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param transmit_packets The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_mod_msg_transmit_packets_set(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint64_t transmit_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, transmit_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get receive_packets from an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param receive_packets Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_mod_msg_receive_packets_get(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint64_t *receive_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, receive_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set receive_packets in an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param receive_packets The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_mod_msg_receive_packets_set(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint64_t receive_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, receive_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_mod_msg_traffic_class_get(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 40;
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
 * Set traffic_class in an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_mod_msg.
 * @param traffic_class The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_mod_msg_traffic_class_set(
    of_ofdpa_oam_dataplane_ctr_mod_msg_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 40;
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
of_ofdpa_oam_dataplane_ctr_multipart_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x3); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_oam_dataplane_ctr_multipart_reply of_ofdpa_oam_dataplane_ctr_multipart_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_oam_dataplane_ctr_multipart_reply_push_wire_values(of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj)
{

    of_ofdpa_oam_dataplane_ctr_multipart_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_oam_dataplane_ctr_multipart_reply object
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
 * \ingroup of_ofdpa_oam_dataplane_ctr_multipart_reply
 */

of_ofdpa_oam_dataplane_ctr_multipart_reply_t *
of_ofdpa_oam_dataplane_ctr_multipart_reply_new(of_version_t version)
{
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY] + of_object_extra_len[version][OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_oam_dataplane_ctr_multipart_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_oam_dataplane_ctr_multipart_reply_init(obj, version, bytes, 0);

    if (of_ofdpa_oam_dataplane_ctr_multipart_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
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
of_ofdpa_oam_dataplane_ctr_multipart_reply_init(of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY] + of_object_extra_len[version][OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_oam_dataplane_ctr_multipart_reply_push_wire_types;

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
 * Create a new of_ofdpa_oam_dataplane_ctr_multipart_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_oam_dataplane_ctr_multipart_reply
 */

of_ofdpa_oam_dataplane_ctr_multipart_reply_t *
of_ofdpa_oam_dataplane_ctr_multipart_reply_new_from_message(of_message_t msg)
{
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_oam_dataplane_ctr_multipart_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_oam_dataplane_ctr_multipart_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_reply_xid_get(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
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
 * Set xid in an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param xid The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_reply_xid_set(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
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
 * Get flags from an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_reply_flags_get(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
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
 * Set flags in an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param flags The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_reply_flags_set(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
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
 * Get experimenter from an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_reply_experimenter_get(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_reply_experimenter_set(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_reply_subtype_get(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_reply_subtype_set(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get lmep_id from an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param lmep_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_reply_lmep_id_get(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t *lmep_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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
 * Set lmep_id in an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param lmep_id The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_reply_lmep_id_set(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint32_t lmep_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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

/**
 * Get receive_packets from an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param receive_packets Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_reply_receive_packets_get(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint64_t *receive_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, receive_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set receive_packets in an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param receive_packets The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_reply_receive_packets_set(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint64_t receive_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, receive_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get transmit_packets from an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param transmit_packets Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_reply_transmit_packets_get(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint64_t *transmit_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, transmit_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set transmit_packets in an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param transmit_packets The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_reply_transmit_packets_set(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint64_t transmit_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, transmit_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get reference_count from an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param reference_count Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_reply_reference_count_get(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint64_t *reference_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 44;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, reference_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set reference_count in an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param reference_count The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_reply_reference_count_set(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint64_t reference_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 44;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, reference_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_reply_traffic_class_get(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 52;
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
 * Set traffic_class in an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_reply.
 * @param traffic_class The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_reply_traffic_class_set(
    of_ofdpa_oam_dataplane_ctr_multipart_reply_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 52;
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
of_ofdpa_oam_dataplane_ctr_multipart_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x3); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_oam_dataplane_ctr_multipart_request of_ofdpa_oam_dataplane_ctr_multipart_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_oam_dataplane_ctr_multipart_request_push_wire_values(of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj)
{

    of_ofdpa_oam_dataplane_ctr_multipart_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_oam_dataplane_ctr_multipart_request object
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
 * \ingroup of_ofdpa_oam_dataplane_ctr_multipart_request
 */

of_ofdpa_oam_dataplane_ctr_multipart_request_t *
of_ofdpa_oam_dataplane_ctr_multipart_request_new(of_version_t version)
{
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST] + of_object_extra_len[version][OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_oam_dataplane_ctr_multipart_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_oam_dataplane_ctr_multipart_request_init(obj, version, bytes, 0);

    if (of_ofdpa_oam_dataplane_ctr_multipart_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
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
of_ofdpa_oam_dataplane_ctr_multipart_request_init(of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST] + of_object_extra_len[version][OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_oam_dataplane_ctr_multipart_request_push_wire_types;

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
 * Create a new of_ofdpa_oam_dataplane_ctr_multipart_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_oam_dataplane_ctr_multipart_request
 */

of_ofdpa_oam_dataplane_ctr_multipart_request_t *
of_ofdpa_oam_dataplane_ctr_multipart_request_new_from_message(of_message_t msg)
{
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_oam_dataplane_ctr_multipart_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_oam_dataplane_ctr_multipart_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_request_xid_get(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
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
 * Set xid in an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param xid The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_request_xid_set(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
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
 * Get flags from an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_request_flags_get(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
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
 * Set flags in an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param flags The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_request_flags_set(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
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
 * Get experimenter from an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_request_experimenter_get(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_request_experimenter_set(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_request_subtype_get(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_request_subtype_set(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get lmep_id from an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param lmep_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_request_lmep_id_get(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t *lmep_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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
 * Set lmep_id in an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param lmep_id The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_request_lmep_id_set(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint32_t lmep_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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

/**
 * Get transmit_packets from an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param transmit_packets Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_request_transmit_packets_get(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint64_t *transmit_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, transmit_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set transmit_packets in an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param transmit_packets The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_request_transmit_packets_set(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint64_t transmit_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, transmit_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get receive_packets from an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param receive_packets Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_request_receive_packets_get(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint64_t *receive_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, receive_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set receive_packets in an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param receive_packets The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_request_receive_packets_set(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint64_t receive_packets)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, receive_packets);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get reference_count from an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param reference_count Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_request_reference_count_get(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint64_t *reference_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 44;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, reference_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set reference_count in an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param reference_count The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_request_reference_count_set(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint64_t reference_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 44;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, reference_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get traffic_class from an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param traffic_class Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_request_traffic_class_get(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint8_t *traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 52;
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
 * Set traffic_class in an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_dataplane_ctr_multipart_request.
 * @param traffic_class The value to write into the object
 */
void
of_ofdpa_oam_dataplane_ctr_multipart_request_traffic_class_set(
    of_ofdpa_oam_dataplane_ctr_multipart_request_t *obj,
    uint8_t traffic_class)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DATAPLANE_CTR_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 52;
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
of_ofdpa_oam_drop_status_mod_msg_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0x4); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_oam_drop_status_mod_msg of_ofdpa_oam_drop_status_mod_msg
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_oam_drop_status_mod_msg_push_wire_values(of_ofdpa_oam_drop_status_mod_msg_t *obj)
{

    of_ofdpa_oam_drop_status_mod_msg_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_oam_drop_status_mod_msg object
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
 * \ingroup of_ofdpa_oam_drop_status_mod_msg
 */

of_ofdpa_oam_drop_status_mod_msg_t *
of_ofdpa_oam_drop_status_mod_msg_new(of_version_t version)
{
    of_ofdpa_oam_drop_status_mod_msg_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_OAM_DROP_STATUS_MOD_MSG] + of_object_extra_len[version][OF_OFDPA_OAM_DROP_STATUS_MOD_MSG];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_oam_drop_status_mod_msg_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_oam_drop_status_mod_msg_init(obj, version, bytes, 0);

    if (of_ofdpa_oam_drop_status_mod_msg_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_oam_drop_status_mod_msg.
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
of_ofdpa_oam_drop_status_mod_msg_init(of_ofdpa_oam_drop_status_mod_msg_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_OAM_DROP_STATUS_MOD_MSG] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_OAM_DROP_STATUS_MOD_MSG] + of_object_extra_len[version][OF_OFDPA_OAM_DROP_STATUS_MOD_MSG];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_OAM_DROP_STATUS_MOD_MSG;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_oam_drop_status_mod_msg_push_wire_types;

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
 * Create a new of_ofdpa_oam_drop_status_mod_msg object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_oam_drop_status_mod_msg
 */

of_ofdpa_oam_drop_status_mod_msg_t *
of_ofdpa_oam_drop_status_mod_msg_new_from_message(of_message_t msg)
{
    of_ofdpa_oam_drop_status_mod_msg_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_oam_drop_status_mod_msg_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_oam_drop_status_mod_msg_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_mod_msg_xid_get(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MOD_MSG);
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
 * Set xid in an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param xid The value to write into the object
 */
void
of_ofdpa_oam_drop_status_mod_msg_xid_set(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MOD_MSG);
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
 * Get experimenter from an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_mod_msg_experimenter_get(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_oam_drop_status_mod_msg_experimenter_set(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_mod_msg_subtype_get(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_oam_drop_status_mod_msg_subtype_set(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mod_command from an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param mod_command Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_mod_msg_mod_command_get(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t *mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mod_command in an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param mod_command The value to write into the object
 */
void
of_ofdpa_oam_drop_status_mod_msg_mod_command_set(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_mod_msg_index_get(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MOD_MSG);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set index in an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param index The value to write into the object
 */
void
of_ofdpa_oam_drop_status_mod_msg_index_set(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MOD_MSG);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, index);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get entry_type from an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param entry_type Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_mod_msg_entry_type_get(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint8_t *entry_type)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, entry_type);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set entry_type in an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param entry_type The value to write into the object
 */
void
of_ofdpa_oam_drop_status_mod_msg_entry_type_set(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint8_t entry_type)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, entry_type);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get drop_status from an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param drop_status Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_mod_msg_drop_status_get(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint8_t *drop_status)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 25;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, drop_status);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set drop_status in an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_mod_msg.
 * @param drop_status The value to write into the object
 */
void
of_ofdpa_oam_drop_status_mod_msg_drop_status_set(
    of_ofdpa_oam_drop_status_mod_msg_t *obj,
    uint8_t drop_status)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 25;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, drop_status);

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
of_ofdpa_oam_drop_status_multipart_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x5); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_oam_drop_status_multipart_reply of_ofdpa_oam_drop_status_multipart_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_oam_drop_status_multipart_reply_push_wire_values(of_ofdpa_oam_drop_status_multipart_reply_t *obj)
{

    of_ofdpa_oam_drop_status_multipart_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_oam_drop_status_multipart_reply object
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
 * \ingroup of_ofdpa_oam_drop_status_multipart_reply
 */

of_ofdpa_oam_drop_status_multipart_reply_t *
of_ofdpa_oam_drop_status_multipart_reply_new(of_version_t version)
{
    of_ofdpa_oam_drop_status_multipart_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY] + of_object_extra_len[version][OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_oam_drop_status_multipart_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_oam_drop_status_multipart_reply_init(obj, version, bytes, 0);

    if (of_ofdpa_oam_drop_status_multipart_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_oam_drop_status_multipart_reply.
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
of_ofdpa_oam_drop_status_multipart_reply_init(of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY] + of_object_extra_len[version][OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_oam_drop_status_multipart_reply_push_wire_types;

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
 * Create a new of_ofdpa_oam_drop_status_multipart_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_oam_drop_status_multipart_reply
 */

of_ofdpa_oam_drop_status_multipart_reply_t *
of_ofdpa_oam_drop_status_multipart_reply_new_from_message(of_message_t msg)
{
    of_ofdpa_oam_drop_status_multipart_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_oam_drop_status_multipart_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_oam_drop_status_multipart_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_multipart_reply_xid_get(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
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
 * Set xid in an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param xid The value to write into the object
 */
void
of_ofdpa_oam_drop_status_multipart_reply_xid_set(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
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
 * Get flags from an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_multipart_reply_flags_get(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
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
 * Set flags in an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param flags The value to write into the object
 */
void
of_ofdpa_oam_drop_status_multipart_reply_flags_set(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
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
 * Get experimenter from an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_multipart_reply_experimenter_get(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_oam_drop_status_multipart_reply_experimenter_set(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_multipart_reply_subtype_get(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_oam_drop_status_multipart_reply_subtype_set(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_multipart_reply_index_get(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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

/*****************************************************************************
 函 数 名  : of_dev_feature_multipart_reply_set
 功能描述  : Set experimeter value
 输入参数  : of_ofdpa_oam_drop_status_multipart_reply_t *obj  
             int addr                                         
             of_dev_feature_id_t feature                      
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月7日
    作    者   : JiaoYong
    修改内容   : 新生成函数

*****************************************************************************/
void
of_dev_feature_multipart_reply_set(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    int addr, of_dev_feature_id_t feature)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_EXPERIMENTER_MULTIPART_REPLY_OFDPA);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = addr;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, feature);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}


/**
 * Set index in an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param index The value to write into the object
 */
void
of_ofdpa_oam_drop_status_multipart_reply_index_set(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
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

/**
 * Get entry_type from an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param entry_type Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_multipart_reply_entry_type_get(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint8_t *entry_type)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, entry_type);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set entry_type in an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param entry_type The value to write into the object
 */
void
of_ofdpa_oam_drop_status_multipart_reply_entry_type_set(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint8_t entry_type)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, entry_type);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get drop_status from an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param drop_status Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_multipart_reply_drop_status_get(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint8_t *drop_status)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, drop_status);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set drop_status in an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_reply.
 * @param drop_status The value to write into the object
 */
void
of_ofdpa_oam_drop_status_multipart_reply_drop_status_set(
    of_ofdpa_oam_drop_status_multipart_reply_t *obj,
    uint8_t drop_status)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 29;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, drop_status);

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
of_ofdpa_oam_drop_status_multipart_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x5); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_oam_drop_status_multipart_request of_ofdpa_oam_drop_status_multipart_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_oam_drop_status_multipart_request_push_wire_values(of_ofdpa_oam_drop_status_multipart_request_t *obj)
{

    of_ofdpa_oam_drop_status_multipart_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_oam_drop_status_multipart_request object
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
 * \ingroup of_ofdpa_oam_drop_status_multipart_request
 */

of_ofdpa_oam_drop_status_multipart_request_t *
of_ofdpa_oam_drop_status_multipart_request_new(of_version_t version)
{
    of_ofdpa_oam_drop_status_multipart_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST] + of_object_extra_len[version][OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_oam_drop_status_multipart_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_oam_drop_status_multipart_request_init(obj, version, bytes, 0);

    if (of_ofdpa_oam_drop_status_multipart_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_oam_drop_status_multipart_request.
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
of_ofdpa_oam_drop_status_multipart_request_init(of_ofdpa_oam_drop_status_multipart_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST] + of_object_extra_len[version][OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_oam_drop_status_multipart_request_push_wire_types;

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
 * Create a new of_ofdpa_oam_drop_status_multipart_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_oam_drop_status_multipart_request
 */

of_ofdpa_oam_drop_status_multipart_request_t *
of_ofdpa_oam_drop_status_multipart_request_new_from_message(of_message_t msg)
{
    of_ofdpa_oam_drop_status_multipart_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_oam_drop_status_multipart_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_oam_drop_status_multipart_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_multipart_request_xid_get(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
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
 * Set xid in an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param xid The value to write into the object
 */
void
of_ofdpa_oam_drop_status_multipart_request_xid_set(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
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
 * Get flags from an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_multipart_request_flags_get(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
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
 * Set flags in an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param flags The value to write into the object
 */
void
of_ofdpa_oam_drop_status_multipart_request_flags_set(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
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
 * Get experimenter from an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_multipart_request_experimenter_get(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_oam_drop_status_multipart_request_experimenter_set(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_multipart_request_subtype_get(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_oam_drop_status_multipart_request_subtype_set(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mod_command from an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param mod_command Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_multipart_request_mod_command_get(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t *mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mod_command in an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param mod_command The value to write into the object
 */
void
of_ofdpa_oam_drop_status_multipart_request_mod_command_set(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, mod_command);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get index from an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param index Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_multipart_request_index_get(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t *index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
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
 * Set index in an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param index The value to write into the object
 */
void
of_ofdpa_oam_drop_status_multipart_request_index_set(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint32_t index)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 28;
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

/**
 * Get entry_type from an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param entry_type Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_multipart_request_entry_type_get(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint8_t *entry_type)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, entry_type);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set entry_type in an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param entry_type The value to write into the object
 */
void
of_ofdpa_oam_drop_status_multipart_request_entry_type_set(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint8_t entry_type)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, entry_type);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get drop_status from an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param drop_status Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_ofdpa_oam_drop_status_multipart_request_drop_status_get(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint8_t *drop_status)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 33;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, drop_status);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set drop_status in an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_oam_drop_status_multipart_request.
 * @param drop_status The value to write into the object
 */
void
of_ofdpa_oam_drop_status_multipart_request_drop_status_set(
    of_ofdpa_oam_drop_status_multipart_request_t *obj,
    uint8_t drop_status)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_OAM_DROP_STATUS_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 33;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, drop_status);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
