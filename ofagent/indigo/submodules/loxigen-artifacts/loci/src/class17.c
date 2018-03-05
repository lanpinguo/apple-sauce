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
of_oxm_onf_actset_output_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff560C); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x4f4e4600); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_onf_actset_output of_oxm_onf_actset_output
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_onf_actset_output_push_wire_values(of_oxm_onf_actset_output_t *obj)
{

    of_oxm_onf_actset_output_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_onf_actset_output object
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
 * \ingroup of_oxm_onf_actset_output
 */

of_oxm_onf_actset_output_t *
of_oxm_onf_actset_output_new(of_version_t version)
{
    of_oxm_onf_actset_output_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_ONF_ACTSET_OUTPUT] + of_object_extra_len[version][OF_OXM_ONF_ACTSET_OUTPUT];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_onf_actset_output_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_onf_actset_output_init(obj, version, bytes, 0);

    if (of_oxm_onf_actset_output_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_onf_actset_output.
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
of_oxm_onf_actset_output_init(of_oxm_onf_actset_output_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_ONF_ACTSET_OUTPUT] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_ONF_ACTSET_OUTPUT] + of_object_extra_len[version][OF_OXM_ONF_ACTSET_OUTPUT];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_ONF_ACTSET_OUTPUT;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_onf_actset_output_push_wire_types;

    obj->wire_length_get = of_oxm_wire_length_get;
    obj->wire_type_get = of_oxm_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_oxm_onf_actset_output.
 * @param obj Pointer to an object of type of_oxm_onf_actset_output.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_onf_actset_output_experimenter_get(
    of_oxm_onf_actset_output_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ONF_ACTSET_OUTPUT);
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
 * Set experimenter in an object of type of_oxm_onf_actset_output.
 * @param obj Pointer to an object of type of_oxm_onf_actset_output.
 * @param experimenter The value to write into the object
 */
void
of_oxm_onf_actset_output_experimenter_set(
    of_oxm_onf_actset_output_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ONF_ACTSET_OUTPUT);
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
 * Get value from an object of type of_oxm_onf_actset_output.
 * @param obj Pointer to an object of type of_oxm_onf_actset_output.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_onf_actset_output_value_get(
    of_oxm_onf_actset_output_t *obj,
    uint64_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ONF_ACTSET_OUTPUT);
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
    of_wire_buffer_u64_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_onf_actset_output.
 * @param obj Pointer to an object of type of_oxm_onf_actset_output.
 * @param value The value to write into the object
 */
void
of_oxm_onf_actset_output_value_set(
    of_oxm_onf_actset_output_t *obj,
    uint64_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_ONF_ACTSET_OUTPUT);
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
    of_wire_buffer_u64_set(wbuf, abs_offset, value);

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
of_port_desc_stats_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xd); /* stats_type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_port_desc_stats_reply of_port_desc_stats_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_port_desc_stats_reply_push_wire_values(of_port_desc_stats_reply_t *obj)
{

    of_port_desc_stats_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_port_desc_stats_reply object
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
 * \ingroup of_port_desc_stats_reply
 */

of_port_desc_stats_reply_t *
of_port_desc_stats_reply_new(of_version_t version)
{
    of_port_desc_stats_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_PORT_DESC_STATS_REPLY] + of_object_extra_len[version][OF_PORT_DESC_STATS_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_port_desc_stats_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_port_desc_stats_reply_init(obj, version, bytes, 0);

    if (of_port_desc_stats_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_port_desc_stats_reply.
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
of_port_desc_stats_reply_init(of_port_desc_stats_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_PORT_DESC_STATS_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_PORT_DESC_STATS_REPLY] + of_object_extra_len[version][OF_PORT_DESC_STATS_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_PORT_DESC_STATS_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_port_desc_stats_reply_push_wire_types;

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
 * Create a new of_port_desc_stats_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_port_desc_stats_reply
 */

of_port_desc_stats_reply_t *
of_port_desc_stats_reply_new_from_message(of_message_t msg)
{
    of_port_desc_stats_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_port_desc_stats_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_port_desc_stats_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_port_desc_stats_reply.
 * @param obj Pointer to an object of type of_port_desc_stats_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_port_desc_stats_reply_xid_get(
    of_port_desc_stats_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_PORT_DESC_STATS_REPLY);
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
 * Set xid in an object of type of_port_desc_stats_reply.
 * @param obj Pointer to an object of type of_port_desc_stats_reply.
 * @param xid The value to write into the object
 */
void
of_port_desc_stats_reply_xid_set(
    of_port_desc_stats_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_PORT_DESC_STATS_REPLY);
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
 * Get flags from an object of type of_port_desc_stats_reply.
 * @param obj Pointer to an object of type of_port_desc_stats_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_port_desc_stats_reply_flags_get(
    of_port_desc_stats_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_PORT_DESC_STATS_REPLY);
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
 * Set flags in an object of type of_port_desc_stats_reply.
 * @param obj Pointer to an object of type of_port_desc_stats_reply.
 * @param flags The value to write into the object
 */
void
of_port_desc_stats_reply_flags_set(
    of_port_desc_stats_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_PORT_DESC_STATS_REPLY);
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
 * Bind an object of type of_list_port_desc_t to the parent of type of_port_desc_stats_reply for
 * member entries
 * @param obj Pointer to an object of type of_port_desc_stats_reply.
 * @param entries Pointer to the child object of type
 * of_list_port_desc_t to be filled out.
 * \ingroup of_port_desc_stats_reply
 *
 * The parameter entries is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_port_desc_stats_reply_entries_bind(
    of_port_desc_stats_reply_t *obj,
    of_list_port_desc_t *entries)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_PORT_DESC_STATS_REPLY);
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
    of_list_port_desc_init(entries, obj->version, 0, 1);
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
 * Create a copy of entries into a new variable of type of_list_port_desc_t from
 * a of_port_desc_stats_reply instance.
 *
 * @param obj Pointer to the source of type of_port_desc_stats_reply_t
 * @returns A pointer to a new instance of type of_list_port_desc_t whose contents
 * match that of entries from source
 * @returns NULL if an error occurs
 */
of_list_port_desc_t *
of_port_desc_stats_reply_entries_get(of_port_desc_stats_reply_t *obj) {
    of_list_port_desc_t _entries;
    of_list_port_desc_t *_entries_ptr;

    of_port_desc_stats_reply_entries_bind(obj, &_entries);
    _entries_ptr = (of_list_port_desc_t *)of_object_dup(&_entries);
    return _entries_ptr;
}

/**
 * Set entries in an object of type of_port_desc_stats_reply.
 * @param obj Pointer to an object of type of_port_desc_stats_reply.
 * @param entries Pointer to the child of type of_list_port_desc_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_port_desc_stats_reply_entries_set(
    of_port_desc_stats_reply_t *obj,
    of_list_port_desc_t *entries)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_PORT_DESC_STATS_REPLY);
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
of_port_desc_stats_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xd); /* stats_type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_port_desc_stats_request of_port_desc_stats_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_port_desc_stats_request_push_wire_values(of_port_desc_stats_request_t *obj)
{

    of_port_desc_stats_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_port_desc_stats_request object
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
 * \ingroup of_port_desc_stats_request
 */

of_port_desc_stats_request_t *
of_port_desc_stats_request_new(of_version_t version)
{
    of_port_desc_stats_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_PORT_DESC_STATS_REQUEST] + of_object_extra_len[version][OF_PORT_DESC_STATS_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_port_desc_stats_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_port_desc_stats_request_init(obj, version, bytes, 0);

    if (of_port_desc_stats_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_port_desc_stats_request.
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
of_port_desc_stats_request_init(of_port_desc_stats_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_PORT_DESC_STATS_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_PORT_DESC_STATS_REQUEST] + of_object_extra_len[version][OF_PORT_DESC_STATS_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_PORT_DESC_STATS_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_port_desc_stats_request_push_wire_types;

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
 * Create a new of_port_desc_stats_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_port_desc_stats_request
 */

of_port_desc_stats_request_t *
of_port_desc_stats_request_new_from_message(of_message_t msg)
{
    of_port_desc_stats_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_port_desc_stats_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_port_desc_stats_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_port_desc_stats_request.
 * @param obj Pointer to an object of type of_port_desc_stats_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_port_desc_stats_request_xid_get(
    of_port_desc_stats_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_PORT_DESC_STATS_REQUEST);
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
 * Set xid in an object of type of_port_desc_stats_request.
 * @param obj Pointer to an object of type of_port_desc_stats_request.
 * @param xid The value to write into the object
 */
void
of_port_desc_stats_request_xid_set(
    of_port_desc_stats_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_PORT_DESC_STATS_REQUEST);
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
 * Get flags from an object of type of_port_desc_stats_request.
 * @param obj Pointer to an object of type of_port_desc_stats_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_port_desc_stats_request_flags_get(
    of_port_desc_stats_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_PORT_DESC_STATS_REQUEST);
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
 * Set flags in an object of type of_port_desc_stats_request.
 * @param obj Pointer to an object of type of_port_desc_stats_request.
 * @param flags The value to write into the object
 */
void
of_port_desc_stats_request_flags_set(
    of_port_desc_stats_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_PORT_DESC_STATS_REQUEST);
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


void
of_table_feature_prop_wire_object_id_get(of_object_t *obj, of_object_id_t *id)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3: {
        uint16_t value = U16_NTOH(*(uint16_t *)(buf + 0)); /* type */
        switch (value) {
        case 0x0:
            *id = OF_TABLE_FEATURE_PROP_INSTRUCTIONS;
            break;
        case 0x1:
            *id = OF_TABLE_FEATURE_PROP_INSTRUCTIONS_MISS;
            break;
        case 0x2:
            *id = OF_TABLE_FEATURE_PROP_NEXT_TABLES;
            break;
        case 0x3:
            *id = OF_TABLE_FEATURE_PROP_NEXT_TABLES_MISS;
            break;
        case 0x4:
            *id = OF_TABLE_FEATURE_PROP_WRITE_ACTIONS;
            break;
        case 0x5:
            *id = OF_TABLE_FEATURE_PROP_WRITE_ACTIONS_MISS;
            break;
        case 0x6:
            *id = OF_TABLE_FEATURE_PROP_APPLY_ACTIONS;
            break;
        case 0x7:
            *id = OF_TABLE_FEATURE_PROP_APPLY_ACTIONS_MISS;
            break;
        case 0x8:
            *id = OF_TABLE_FEATURE_PROP_MATCH;
            break;
        case 0xa:
            *id = OF_TABLE_FEATURE_PROP_WILDCARDS;
            break;
        case 0xc:
            *id = OF_TABLE_FEATURE_PROP_WRITE_SETFIELD;
            break;
        case 0xd:
            *id = OF_TABLE_FEATURE_PROP_WRITE_SETFIELD_MISS;
            break;
        case 0xe:
            *id = OF_TABLE_FEATURE_PROP_APPLY_SETFIELD;
            break;
        case 0xf:
            *id = OF_TABLE_FEATURE_PROP_APPLY_SETFIELD_MISS;
            break;
        case 0xfffe:
            *id = OF_TABLE_FEATURE_PROP_EXPERIMENTER;
            break;
        case 0xffff:
            *id = OF_TABLE_FEATURE_PROP_EXPERIMENTER_MISS;
            break;
        default:
            *id = OF_TABLE_FEATURE_PROP;
            break;
        }
        break;
    }
    default:
        LOCI_ASSERT(0);
    }
}


/**
 * \defgroup of_table_feature_prop of_table_feature_prop
 */

/**
 * Create a new of_table_feature_prop object
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
 * \ingroup of_table_feature_prop
 */

of_table_feature_prop_t *
of_table_feature_prop_new(of_version_t version)
{
    of_table_feature_prop_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_table_feature_prop_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_table_feature_prop_init(obj, version, bytes, 0);

    return obj;
}

/**
 * Initialize an object of type of_table_feature_prop.
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
of_table_feature_prop_init(of_table_feature_prop_t *obj_p,
    of_version_t version, int bytes, int clean_wire)
{
    of_table_feature_prop_header_t *obj;

    obj = &obj_p->header;  /* Need instantiable subclass */

    LOCI_ASSERT(of_object_fixed_len[version][OF_TABLE_FEATURE_PROP] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_TABLE_FEATURE_PROP;

    /* Set up the object's function pointers */

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_table_feature_prop_wire_object_id_get;

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
of_table_feature_prop_apply_actions_push_wire_types(of_object_t *obj)
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
 * \defgroup of_table_feature_prop_apply_actions of_table_feature_prop_apply_actions
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_table_feature_prop_apply_actions_push_wire_values(of_table_feature_prop_apply_actions_t *obj)
{

    of_table_feature_prop_apply_actions_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_table_feature_prop_apply_actions object
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
 * \ingroup of_table_feature_prop_apply_actions
 */

of_table_feature_prop_apply_actions_t *
of_table_feature_prop_apply_actions_new(of_version_t version)
{
    of_table_feature_prop_apply_actions_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_APPLY_ACTIONS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_APPLY_ACTIONS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_table_feature_prop_apply_actions_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_table_feature_prop_apply_actions_init(obj, version, bytes, 0);

    if (of_table_feature_prop_apply_actions_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_table_feature_prop_apply_actions.
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
of_table_feature_prop_apply_actions_init(of_table_feature_prop_apply_actions_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_APPLY_ACTIONS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_APPLY_ACTIONS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_APPLY_ACTIONS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_TABLE_FEATURE_PROP_APPLY_ACTIONS;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_table_feature_prop_apply_actions_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_table_feature_prop_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Bind an object of type of_list_action_id_t to the parent of type of_table_feature_prop_apply_actions for
 * member action_ids
 * @param obj Pointer to an object of type of_table_feature_prop_apply_actions.
 * @param action_ids Pointer to the child object of type
 * of_list_action_id_t to be filled out.
 * \ingroup of_table_feature_prop_apply_actions
 *
 * The parameter action_ids is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_table_feature_prop_apply_actions_action_ids_bind(
    of_table_feature_prop_apply_actions_t *obj,
    of_list_action_id_t *action_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_APPLY_ACTIONS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_action_id_init(action_ids, obj->version, 0, 1);
    /* Attach to parent */
    action_ids->parent = (of_object_t *)obj;
    action_ids->wire_object.wbuf = obj->wire_object.wbuf;
    action_ids->wire_object.obj_offset = abs_offset;
    action_ids->wire_object.owned = 0;
    action_ids->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of action_ids into a new variable of type of_list_action_id_t from
 * a of_table_feature_prop_apply_actions instance.
 *
 * @param obj Pointer to the source of type of_table_feature_prop_apply_actions_t
 * @returns A pointer to a new instance of type of_list_action_id_t whose contents
 * match that of action_ids from source
 * @returns NULL if an error occurs
 */
of_list_action_id_t *
of_table_feature_prop_apply_actions_action_ids_get(of_table_feature_prop_apply_actions_t *obj) {
    of_list_action_id_t _action_ids;
    of_list_action_id_t *_action_ids_ptr;

    of_table_feature_prop_apply_actions_action_ids_bind(obj, &_action_ids);
    _action_ids_ptr = (of_list_action_id_t *)of_object_dup(&_action_ids);
    return _action_ids_ptr;
}

/**
 * Set action_ids in an object of type of_table_feature_prop_apply_actions.
 * @param obj Pointer to an object of type of_table_feature_prop_apply_actions.
 * @param action_ids Pointer to the child of type of_list_action_id_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_table_feature_prop_apply_actions_action_ids_set(
    of_table_feature_prop_apply_actions_t *obj,
    of_list_action_id_t *action_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_APPLY_ACTIONS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = action_ids->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == action_ids->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(action_ids, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(action_ids, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (action_ids->wire_length_set != NULL) {
        action_ids->wire_length_set((of_object_t *)action_ids, action_ids->length);
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
of_table_feature_prop_apply_actions_miss_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x7); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_table_feature_prop_apply_actions_miss of_table_feature_prop_apply_actions_miss
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_table_feature_prop_apply_actions_miss_push_wire_values(of_table_feature_prop_apply_actions_miss_t *obj)
{

    of_table_feature_prop_apply_actions_miss_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_table_feature_prop_apply_actions_miss object
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
 * \ingroup of_table_feature_prop_apply_actions_miss
 */

of_table_feature_prop_apply_actions_miss_t *
of_table_feature_prop_apply_actions_miss_new(of_version_t version)
{
    of_table_feature_prop_apply_actions_miss_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_APPLY_ACTIONS_MISS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_APPLY_ACTIONS_MISS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_table_feature_prop_apply_actions_miss_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_table_feature_prop_apply_actions_miss_init(obj, version, bytes, 0);

    if (of_table_feature_prop_apply_actions_miss_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_table_feature_prop_apply_actions_miss.
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
of_table_feature_prop_apply_actions_miss_init(of_table_feature_prop_apply_actions_miss_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_APPLY_ACTIONS_MISS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_APPLY_ACTIONS_MISS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_APPLY_ACTIONS_MISS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_TABLE_FEATURE_PROP_APPLY_ACTIONS_MISS;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_table_feature_prop_apply_actions_miss_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_table_feature_prop_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Bind an object of type of_list_action_id_t to the parent of type of_table_feature_prop_apply_actions_miss for
 * member action_ids
 * @param obj Pointer to an object of type of_table_feature_prop_apply_actions_miss.
 * @param action_ids Pointer to the child object of type
 * of_list_action_id_t to be filled out.
 * \ingroup of_table_feature_prop_apply_actions_miss
 *
 * The parameter action_ids is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_table_feature_prop_apply_actions_miss_action_ids_bind(
    of_table_feature_prop_apply_actions_miss_t *obj,
    of_list_action_id_t *action_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_APPLY_ACTIONS_MISS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_action_id_init(action_ids, obj->version, 0, 1);
    /* Attach to parent */
    action_ids->parent = (of_object_t *)obj;
    action_ids->wire_object.wbuf = obj->wire_object.wbuf;
    action_ids->wire_object.obj_offset = abs_offset;
    action_ids->wire_object.owned = 0;
    action_ids->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of action_ids into a new variable of type of_list_action_id_t from
 * a of_table_feature_prop_apply_actions_miss instance.
 *
 * @param obj Pointer to the source of type of_table_feature_prop_apply_actions_miss_t
 * @returns A pointer to a new instance of type of_list_action_id_t whose contents
 * match that of action_ids from source
 * @returns NULL if an error occurs
 */
of_list_action_id_t *
of_table_feature_prop_apply_actions_miss_action_ids_get(of_table_feature_prop_apply_actions_miss_t *obj) {
    of_list_action_id_t _action_ids;
    of_list_action_id_t *_action_ids_ptr;

    of_table_feature_prop_apply_actions_miss_action_ids_bind(obj, &_action_ids);
    _action_ids_ptr = (of_list_action_id_t *)of_object_dup(&_action_ids);
    return _action_ids_ptr;
}

/**
 * Set action_ids in an object of type of_table_feature_prop_apply_actions_miss.
 * @param obj Pointer to an object of type of_table_feature_prop_apply_actions_miss.
 * @param action_ids Pointer to the child of type of_list_action_id_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_table_feature_prop_apply_actions_miss_action_ids_set(
    of_table_feature_prop_apply_actions_miss_t *obj,
    of_list_action_id_t *action_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_APPLY_ACTIONS_MISS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = action_ids->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == action_ids->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(action_ids, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(action_ids, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (action_ids->wire_length_set != NULL) {
        action_ids->wire_length_set((of_object_t *)action_ids, action_ids->length);
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
of_table_feature_prop_apply_setfield_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xe); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_table_feature_prop_apply_setfield of_table_feature_prop_apply_setfield
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_table_feature_prop_apply_setfield_push_wire_values(of_table_feature_prop_apply_setfield_t *obj)
{

    of_table_feature_prop_apply_setfield_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_table_feature_prop_apply_setfield object
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
 * \ingroup of_table_feature_prop_apply_setfield
 */

of_table_feature_prop_apply_setfield_t *
of_table_feature_prop_apply_setfield_new(of_version_t version)
{
    of_table_feature_prop_apply_setfield_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_APPLY_SETFIELD] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_APPLY_SETFIELD];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_table_feature_prop_apply_setfield_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_table_feature_prop_apply_setfield_init(obj, version, bytes, 0);

    if (of_table_feature_prop_apply_setfield_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_table_feature_prop_apply_setfield.
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
of_table_feature_prop_apply_setfield_init(of_table_feature_prop_apply_setfield_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_APPLY_SETFIELD] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_APPLY_SETFIELD] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_APPLY_SETFIELD];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_TABLE_FEATURE_PROP_APPLY_SETFIELD;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_table_feature_prop_apply_setfield_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_table_feature_prop_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Bind an object of type of_list_uint32_t to the parent of type of_table_feature_prop_apply_setfield for
 * member oxm_ids
 * @param obj Pointer to an object of type of_table_feature_prop_apply_setfield.
 * @param oxm_ids Pointer to the child object of type
 * of_list_uint32_t to be filled out.
 * \ingroup of_table_feature_prop_apply_setfield
 *
 * The parameter oxm_ids is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_table_feature_prop_apply_setfield_oxm_ids_bind(
    of_table_feature_prop_apply_setfield_t *obj,
    of_list_uint32_t *oxm_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_APPLY_SETFIELD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_uint32_init(oxm_ids, obj->version, 0, 1);
    /* Attach to parent */
    oxm_ids->parent = (of_object_t *)obj;
    oxm_ids->wire_object.wbuf = obj->wire_object.wbuf;
    oxm_ids->wire_object.obj_offset = abs_offset;
    oxm_ids->wire_object.owned = 0;
    oxm_ids->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of oxm_ids into a new variable of type of_list_uint32_t from
 * a of_table_feature_prop_apply_setfield instance.
 *
 * @param obj Pointer to the source of type of_table_feature_prop_apply_setfield_t
 * @returns A pointer to a new instance of type of_list_uint32_t whose contents
 * match that of oxm_ids from source
 * @returns NULL if an error occurs
 */
of_list_uint32_t *
of_table_feature_prop_apply_setfield_oxm_ids_get(of_table_feature_prop_apply_setfield_t *obj) {
    of_list_uint32_t _oxm_ids;
    of_list_uint32_t *_oxm_ids_ptr;

    of_table_feature_prop_apply_setfield_oxm_ids_bind(obj, &_oxm_ids);
    _oxm_ids_ptr = (of_list_uint32_t *)of_object_dup(&_oxm_ids);
    return _oxm_ids_ptr;
}

/**
 * Set oxm_ids in an object of type of_table_feature_prop_apply_setfield.
 * @param obj Pointer to an object of type of_table_feature_prop_apply_setfield.
 * @param oxm_ids Pointer to the child of type of_list_uint32_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_table_feature_prop_apply_setfield_oxm_ids_set(
    of_table_feature_prop_apply_setfield_t *obj,
    of_list_uint32_t *oxm_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_APPLY_SETFIELD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = oxm_ids->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == oxm_ids->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(oxm_ids, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(oxm_ids, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (oxm_ids->wire_length_set != NULL) {
        oxm_ids->wire_length_set((of_object_t *)oxm_ids, oxm_ids->length);
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
of_table_feature_prop_apply_setfield_miss_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xf); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_table_feature_prop_apply_setfield_miss of_table_feature_prop_apply_setfield_miss
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_table_feature_prop_apply_setfield_miss_push_wire_values(of_table_feature_prop_apply_setfield_miss_t *obj)
{

    of_table_feature_prop_apply_setfield_miss_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_table_feature_prop_apply_setfield_miss object
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
 * \ingroup of_table_feature_prop_apply_setfield_miss
 */

of_table_feature_prop_apply_setfield_miss_t *
of_table_feature_prop_apply_setfield_miss_new(of_version_t version)
{
    of_table_feature_prop_apply_setfield_miss_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_APPLY_SETFIELD_MISS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_APPLY_SETFIELD_MISS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_table_feature_prop_apply_setfield_miss_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_table_feature_prop_apply_setfield_miss_init(obj, version, bytes, 0);

    if (of_table_feature_prop_apply_setfield_miss_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_table_feature_prop_apply_setfield_miss.
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
of_table_feature_prop_apply_setfield_miss_init(of_table_feature_prop_apply_setfield_miss_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_APPLY_SETFIELD_MISS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_APPLY_SETFIELD_MISS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_APPLY_SETFIELD_MISS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_TABLE_FEATURE_PROP_APPLY_SETFIELD_MISS;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_table_feature_prop_apply_setfield_miss_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_table_feature_prop_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Bind an object of type of_list_uint32_t to the parent of type of_table_feature_prop_apply_setfield_miss for
 * member oxm_ids
 * @param obj Pointer to an object of type of_table_feature_prop_apply_setfield_miss.
 * @param oxm_ids Pointer to the child object of type
 * of_list_uint32_t to be filled out.
 * \ingroup of_table_feature_prop_apply_setfield_miss
 *
 * The parameter oxm_ids is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_table_feature_prop_apply_setfield_miss_oxm_ids_bind(
    of_table_feature_prop_apply_setfield_miss_t *obj,
    of_list_uint32_t *oxm_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_APPLY_SETFIELD_MISS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_uint32_init(oxm_ids, obj->version, 0, 1);
    /* Attach to parent */
    oxm_ids->parent = (of_object_t *)obj;
    oxm_ids->wire_object.wbuf = obj->wire_object.wbuf;
    oxm_ids->wire_object.obj_offset = abs_offset;
    oxm_ids->wire_object.owned = 0;
    oxm_ids->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of oxm_ids into a new variable of type of_list_uint32_t from
 * a of_table_feature_prop_apply_setfield_miss instance.
 *
 * @param obj Pointer to the source of type of_table_feature_prop_apply_setfield_miss_t
 * @returns A pointer to a new instance of type of_list_uint32_t whose contents
 * match that of oxm_ids from source
 * @returns NULL if an error occurs
 */
of_list_uint32_t *
of_table_feature_prop_apply_setfield_miss_oxm_ids_get(of_table_feature_prop_apply_setfield_miss_t *obj) {
    of_list_uint32_t _oxm_ids;
    of_list_uint32_t *_oxm_ids_ptr;

    of_table_feature_prop_apply_setfield_miss_oxm_ids_bind(obj, &_oxm_ids);
    _oxm_ids_ptr = (of_list_uint32_t *)of_object_dup(&_oxm_ids);
    return _oxm_ids_ptr;
}

/**
 * Set oxm_ids in an object of type of_table_feature_prop_apply_setfield_miss.
 * @param obj Pointer to an object of type of_table_feature_prop_apply_setfield_miss.
 * @param oxm_ids Pointer to the child of type of_list_uint32_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_table_feature_prop_apply_setfield_miss_oxm_ids_set(
    of_table_feature_prop_apply_setfield_miss_t *obj,
    of_list_uint32_t *oxm_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_APPLY_SETFIELD_MISS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = oxm_ids->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == oxm_ids->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(oxm_ids, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(oxm_ids, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (oxm_ids->wire_length_set != NULL) {
        oxm_ids->wire_length_set((of_object_t *)oxm_ids, oxm_ids->length);
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
of_table_feature_prop_experimenter_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xfffe); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_table_feature_prop_experimenter of_table_feature_prop_experimenter
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_table_feature_prop_experimenter_push_wire_values(of_table_feature_prop_experimenter_t *obj)
{

    of_table_feature_prop_experimenter_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_table_feature_prop_experimenter object
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
 * \ingroup of_table_feature_prop_experimenter
 */

of_table_feature_prop_experimenter_t *
of_table_feature_prop_experimenter_new(of_version_t version)
{
    of_table_feature_prop_experimenter_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_EXPERIMENTER] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_EXPERIMENTER];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_table_feature_prop_experimenter_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_table_feature_prop_experimenter_init(obj, version, bytes, 0);

    if (of_table_feature_prop_experimenter_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_table_feature_prop_experimenter.
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
of_table_feature_prop_experimenter_init(of_table_feature_prop_experimenter_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_EXPERIMENTER] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_EXPERIMENTER] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_EXPERIMENTER];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_TABLE_FEATURE_PROP_EXPERIMENTER;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_table_feature_prop_experimenter_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_table_feature_prop_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_table_feature_prop_experimenter.
 * @param obj Pointer to an object of type of_table_feature_prop_experimenter.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_table_feature_prop_experimenter_experimenter_get(
    of_table_feature_prop_experimenter_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_EXPERIMENTER);
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
 * Set experimenter in an object of type of_table_feature_prop_experimenter.
 * @param obj Pointer to an object of type of_table_feature_prop_experimenter.
 * @param experimenter The value to write into the object
 */
void
of_table_feature_prop_experimenter_experimenter_set(
    of_table_feature_prop_experimenter_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_EXPERIMENTER);
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
 * Get subtype from an object of type of_table_feature_prop_experimenter.
 * @param obj Pointer to an object of type of_table_feature_prop_experimenter.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_table_feature_prop_experimenter_subtype_get(
    of_table_feature_prop_experimenter_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_EXPERIMENTER);
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
 * Set subtype in an object of type of_table_feature_prop_experimenter.
 * @param obj Pointer to an object of type of_table_feature_prop_experimenter.
 * @param subtype The value to write into the object
 */
void
of_table_feature_prop_experimenter_subtype_set(
    of_table_feature_prop_experimenter_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_EXPERIMENTER);
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

/**
 * Get experimenter_data from an object of type of_table_feature_prop_experimenter.
 * @param obj Pointer to an object of type of_table_feature_prop_experimenter.
 * @param experimenter_data Pointer to the child object of type
 * of_octets_t to be filled out.
 *
 */
void
of_table_feature_prop_experimenter_experimenter_data_get(
    of_table_feature_prop_experimenter_t *obj,
    of_octets_t *experimenter_data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_EXPERIMENTER);
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
    experimenter_data->bytes = cur_len;
    experimenter_data->data = OF_WIRE_BUFFER_INDEX(wbuf, abs_offset);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter_data in an object of type of_table_feature_prop_experimenter.
 * @param obj Pointer to an object of type of_table_feature_prop_experimenter.
 * @param experimenter_data The value to write into the object
 */
int WARN_UNUSED_RESULT
of_table_feature_prop_experimenter_experimenter_data_set(
    of_table_feature_prop_experimenter_t *obj,
    of_octets_t *experimenter_data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_EXPERIMENTER);
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
    new_len = experimenter_data->bytes;
    of_wire_buffer_grow(wbuf, abs_offset + (new_len - cur_len));
    of_wire_buffer_octets_data_set(wbuf, abs_offset, experimenter_data, cur_len);

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
of_table_feature_prop_experimenter_miss_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xffff); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_table_feature_prop_experimenter_miss of_table_feature_prop_experimenter_miss
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_table_feature_prop_experimenter_miss_push_wire_values(of_table_feature_prop_experimenter_miss_t *obj)
{

    of_table_feature_prop_experimenter_miss_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_table_feature_prop_experimenter_miss object
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
 * \ingroup of_table_feature_prop_experimenter_miss
 */

of_table_feature_prop_experimenter_miss_t *
of_table_feature_prop_experimenter_miss_new(of_version_t version)
{
    of_table_feature_prop_experimenter_miss_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_EXPERIMENTER_MISS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_EXPERIMENTER_MISS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_table_feature_prop_experimenter_miss_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_table_feature_prop_experimenter_miss_init(obj, version, bytes, 0);

    if (of_table_feature_prop_experimenter_miss_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_table_feature_prop_experimenter_miss.
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
of_table_feature_prop_experimenter_miss_init(of_table_feature_prop_experimenter_miss_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_EXPERIMENTER_MISS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_EXPERIMENTER_MISS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_EXPERIMENTER_MISS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_TABLE_FEATURE_PROP_EXPERIMENTER_MISS;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_table_feature_prop_experimenter_miss_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_table_feature_prop_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get experimenter from an object of type of_table_feature_prop_experimenter_miss.
 * @param obj Pointer to an object of type of_table_feature_prop_experimenter_miss.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_table_feature_prop_experimenter_miss_experimenter_get(
    of_table_feature_prop_experimenter_miss_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_EXPERIMENTER_MISS);
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
 * Set experimenter in an object of type of_table_feature_prop_experimenter_miss.
 * @param obj Pointer to an object of type of_table_feature_prop_experimenter_miss.
 * @param experimenter The value to write into the object
 */
void
of_table_feature_prop_experimenter_miss_experimenter_set(
    of_table_feature_prop_experimenter_miss_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_EXPERIMENTER_MISS);
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
 * Get subtype from an object of type of_table_feature_prop_experimenter_miss.
 * @param obj Pointer to an object of type of_table_feature_prop_experimenter_miss.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_table_feature_prop_experimenter_miss_subtype_get(
    of_table_feature_prop_experimenter_miss_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_EXPERIMENTER_MISS);
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
 * Set subtype in an object of type of_table_feature_prop_experimenter_miss.
 * @param obj Pointer to an object of type of_table_feature_prop_experimenter_miss.
 * @param subtype The value to write into the object
 */
void
of_table_feature_prop_experimenter_miss_subtype_set(
    of_table_feature_prop_experimenter_miss_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_EXPERIMENTER_MISS);
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

/**
 * Get experimenter_data from an object of type of_table_feature_prop_experimenter_miss.
 * @param obj Pointer to an object of type of_table_feature_prop_experimenter_miss.
 * @param experimenter_data Pointer to the child object of type
 * of_octets_t to be filled out.
 *
 */
void
of_table_feature_prop_experimenter_miss_experimenter_data_get(
    of_table_feature_prop_experimenter_miss_t *obj,
    of_octets_t *experimenter_data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_EXPERIMENTER_MISS);
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
    experimenter_data->bytes = cur_len;
    experimenter_data->data = OF_WIRE_BUFFER_INDEX(wbuf, abs_offset);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter_data in an object of type of_table_feature_prop_experimenter_miss.
 * @param obj Pointer to an object of type of_table_feature_prop_experimenter_miss.
 * @param experimenter_data The value to write into the object
 */
int WARN_UNUSED_RESULT
of_table_feature_prop_experimenter_miss_experimenter_data_set(
    of_table_feature_prop_experimenter_miss_t *obj,
    of_octets_t *experimenter_data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_EXPERIMENTER_MISS);
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
    new_len = experimenter_data->bytes;
    of_wire_buffer_grow(wbuf, abs_offset + (new_len - cur_len));
    of_wire_buffer_octets_data_set(wbuf, abs_offset, experimenter_data, cur_len);

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
of_table_feature_prop_instructions_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x0); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_table_feature_prop_instructions of_table_feature_prop_instructions
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_table_feature_prop_instructions_push_wire_values(of_table_feature_prop_instructions_t *obj)
{

    of_table_feature_prop_instructions_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_table_feature_prop_instructions object
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
 * \ingroup of_table_feature_prop_instructions
 */

of_table_feature_prop_instructions_t *
of_table_feature_prop_instructions_new(of_version_t version)
{
    of_table_feature_prop_instructions_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_INSTRUCTIONS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_INSTRUCTIONS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_table_feature_prop_instructions_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_table_feature_prop_instructions_init(obj, version, bytes, 0);

    if (of_table_feature_prop_instructions_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_table_feature_prop_instructions.
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
of_table_feature_prop_instructions_init(of_table_feature_prop_instructions_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_INSTRUCTIONS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_INSTRUCTIONS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_INSTRUCTIONS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_TABLE_FEATURE_PROP_INSTRUCTIONS;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_table_feature_prop_instructions_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_table_feature_prop_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Bind an object of type of_list_instruction_id_t to the parent of type of_table_feature_prop_instructions for
 * member instruction_ids
 * @param obj Pointer to an object of type of_table_feature_prop_instructions.
 * @param instruction_ids Pointer to the child object of type
 * of_list_instruction_id_t to be filled out.
 * \ingroup of_table_feature_prop_instructions
 *
 * The parameter instruction_ids is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_table_feature_prop_instructions_instruction_ids_bind(
    of_table_feature_prop_instructions_t *obj,
    of_list_instruction_id_t *instruction_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_INSTRUCTIONS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_instruction_id_init(instruction_ids, obj->version, 0, 1);
    /* Attach to parent */
    instruction_ids->parent = (of_object_t *)obj;
    instruction_ids->wire_object.wbuf = obj->wire_object.wbuf;
    instruction_ids->wire_object.obj_offset = abs_offset;
    instruction_ids->wire_object.owned = 0;
    instruction_ids->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of instruction_ids into a new variable of type of_list_instruction_id_t from
 * a of_table_feature_prop_instructions instance.
 *
 * @param obj Pointer to the source of type of_table_feature_prop_instructions_t
 * @returns A pointer to a new instance of type of_list_instruction_id_t whose contents
 * match that of instruction_ids from source
 * @returns NULL if an error occurs
 */
of_list_instruction_id_t *
of_table_feature_prop_instructions_instruction_ids_get(of_table_feature_prop_instructions_t *obj) {
    of_list_instruction_id_t _instruction_ids;
    of_list_instruction_id_t *_instruction_ids_ptr;

    of_table_feature_prop_instructions_instruction_ids_bind(obj, &_instruction_ids);
    _instruction_ids_ptr = (of_list_instruction_id_t *)of_object_dup(&_instruction_ids);
    return _instruction_ids_ptr;
}

/**
 * Set instruction_ids in an object of type of_table_feature_prop_instructions.
 * @param obj Pointer to an object of type of_table_feature_prop_instructions.
 * @param instruction_ids Pointer to the child of type of_list_instruction_id_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_table_feature_prop_instructions_instruction_ids_set(
    of_table_feature_prop_instructions_t *obj,
    of_list_instruction_id_t *instruction_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_INSTRUCTIONS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = instruction_ids->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == instruction_ids->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(instruction_ids, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(instruction_ids, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (instruction_ids->wire_length_set != NULL) {
        instruction_ids->wire_length_set((of_object_t *)instruction_ids, instruction_ids->length);
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
of_table_feature_prop_instructions_miss_push_wire_types(of_object_t *obj)
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
 * \defgroup of_table_feature_prop_instructions_miss of_table_feature_prop_instructions_miss
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_table_feature_prop_instructions_miss_push_wire_values(of_table_feature_prop_instructions_miss_t *obj)
{

    of_table_feature_prop_instructions_miss_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_table_feature_prop_instructions_miss object
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
 * \ingroup of_table_feature_prop_instructions_miss
 */

of_table_feature_prop_instructions_miss_t *
of_table_feature_prop_instructions_miss_new(of_version_t version)
{
    of_table_feature_prop_instructions_miss_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_INSTRUCTIONS_MISS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_INSTRUCTIONS_MISS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_table_feature_prop_instructions_miss_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_table_feature_prop_instructions_miss_init(obj, version, bytes, 0);

    if (of_table_feature_prop_instructions_miss_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_table_feature_prop_instructions_miss.
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
of_table_feature_prop_instructions_miss_init(of_table_feature_prop_instructions_miss_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_INSTRUCTIONS_MISS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_INSTRUCTIONS_MISS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_INSTRUCTIONS_MISS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_TABLE_FEATURE_PROP_INSTRUCTIONS_MISS;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_table_feature_prop_instructions_miss_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_table_feature_prop_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Bind an object of type of_list_instruction_id_t to the parent of type of_table_feature_prop_instructions_miss for
 * member instruction_ids
 * @param obj Pointer to an object of type of_table_feature_prop_instructions_miss.
 * @param instruction_ids Pointer to the child object of type
 * of_list_instruction_id_t to be filled out.
 * \ingroup of_table_feature_prop_instructions_miss
 *
 * The parameter instruction_ids is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_table_feature_prop_instructions_miss_instruction_ids_bind(
    of_table_feature_prop_instructions_miss_t *obj,
    of_list_instruction_id_t *instruction_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_INSTRUCTIONS_MISS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_instruction_id_init(instruction_ids, obj->version, 0, 1);
    /* Attach to parent */
    instruction_ids->parent = (of_object_t *)obj;
    instruction_ids->wire_object.wbuf = obj->wire_object.wbuf;
    instruction_ids->wire_object.obj_offset = abs_offset;
    instruction_ids->wire_object.owned = 0;
    instruction_ids->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of instruction_ids into a new variable of type of_list_instruction_id_t from
 * a of_table_feature_prop_instructions_miss instance.
 *
 * @param obj Pointer to the source of type of_table_feature_prop_instructions_miss_t
 * @returns A pointer to a new instance of type of_list_instruction_id_t whose contents
 * match that of instruction_ids from source
 * @returns NULL if an error occurs
 */
of_list_instruction_id_t *
of_table_feature_prop_instructions_miss_instruction_ids_get(of_table_feature_prop_instructions_miss_t *obj) {
    of_list_instruction_id_t _instruction_ids;
    of_list_instruction_id_t *_instruction_ids_ptr;

    of_table_feature_prop_instructions_miss_instruction_ids_bind(obj, &_instruction_ids);
    _instruction_ids_ptr = (of_list_instruction_id_t *)of_object_dup(&_instruction_ids);
    return _instruction_ids_ptr;
}

/**
 * Set instruction_ids in an object of type of_table_feature_prop_instructions_miss.
 * @param obj Pointer to an object of type of_table_feature_prop_instructions_miss.
 * @param instruction_ids Pointer to the child of type of_list_instruction_id_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_table_feature_prop_instructions_miss_instruction_ids_set(
    of_table_feature_prop_instructions_miss_t *obj,
    of_list_instruction_id_t *instruction_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_INSTRUCTIONS_MISS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = instruction_ids->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == instruction_ids->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(instruction_ids, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(instruction_ids, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (instruction_ids->wire_length_set != NULL) {
        instruction_ids->wire_length_set((of_object_t *)instruction_ids, instruction_ids->length);
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
of_table_feature_prop_match_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x8); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_table_feature_prop_match of_table_feature_prop_match
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_table_feature_prop_match_push_wire_values(of_table_feature_prop_match_t *obj)
{

    of_table_feature_prop_match_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_table_feature_prop_match object
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
 * \ingroup of_table_feature_prop_match
 */

of_table_feature_prop_match_t *
of_table_feature_prop_match_new(of_version_t version)
{
    of_table_feature_prop_match_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_MATCH] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_MATCH];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_table_feature_prop_match_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_table_feature_prop_match_init(obj, version, bytes, 0);

    if (of_table_feature_prop_match_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_table_feature_prop_match.
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
of_table_feature_prop_match_init(of_table_feature_prop_match_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_MATCH] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_MATCH] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_MATCH];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_TABLE_FEATURE_PROP_MATCH;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_table_feature_prop_match_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_table_feature_prop_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Bind an object of type of_list_uint32_t to the parent of type of_table_feature_prop_match for
 * member oxm_ids
 * @param obj Pointer to an object of type of_table_feature_prop_match.
 * @param oxm_ids Pointer to the child object of type
 * of_list_uint32_t to be filled out.
 * \ingroup of_table_feature_prop_match
 *
 * The parameter oxm_ids is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_table_feature_prop_match_oxm_ids_bind(
    of_table_feature_prop_match_t *obj,
    of_list_uint32_t *oxm_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_MATCH);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_uint32_init(oxm_ids, obj->version, 0, 1);
    /* Attach to parent */
    oxm_ids->parent = (of_object_t *)obj;
    oxm_ids->wire_object.wbuf = obj->wire_object.wbuf;
    oxm_ids->wire_object.obj_offset = abs_offset;
    oxm_ids->wire_object.owned = 0;
    oxm_ids->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of oxm_ids into a new variable of type of_list_uint32_t from
 * a of_table_feature_prop_match instance.
 *
 * @param obj Pointer to the source of type of_table_feature_prop_match_t
 * @returns A pointer to a new instance of type of_list_uint32_t whose contents
 * match that of oxm_ids from source
 * @returns NULL if an error occurs
 */
of_list_uint32_t *
of_table_feature_prop_match_oxm_ids_get(of_table_feature_prop_match_t *obj) {
    of_list_uint32_t _oxm_ids;
    of_list_uint32_t *_oxm_ids_ptr;

    of_table_feature_prop_match_oxm_ids_bind(obj, &_oxm_ids);
    _oxm_ids_ptr = (of_list_uint32_t *)of_object_dup(&_oxm_ids);
    return _oxm_ids_ptr;
}

/**
 * Set oxm_ids in an object of type of_table_feature_prop_match.
 * @param obj Pointer to an object of type of_table_feature_prop_match.
 * @param oxm_ids Pointer to the child of type of_list_uint32_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_table_feature_prop_match_oxm_ids_set(
    of_table_feature_prop_match_t *obj,
    of_list_uint32_t *oxm_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_MATCH);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = oxm_ids->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == oxm_ids->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(oxm_ids, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(oxm_ids, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (oxm_ids->wire_length_set != NULL) {
        oxm_ids->wire_length_set((of_object_t *)oxm_ids, oxm_ids->length);
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
of_table_feature_prop_next_tables_push_wire_types(of_object_t *obj)
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
 * \defgroup of_table_feature_prop_next_tables of_table_feature_prop_next_tables
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_table_feature_prop_next_tables_push_wire_values(of_table_feature_prop_next_tables_t *obj)
{

    of_table_feature_prop_next_tables_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_table_feature_prop_next_tables object
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
 * \ingroup of_table_feature_prop_next_tables
 */

of_table_feature_prop_next_tables_t *
of_table_feature_prop_next_tables_new(of_version_t version)
{
    of_table_feature_prop_next_tables_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_NEXT_TABLES] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_NEXT_TABLES];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_table_feature_prop_next_tables_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_table_feature_prop_next_tables_init(obj, version, bytes, 0);

    if (of_table_feature_prop_next_tables_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_table_feature_prop_next_tables.
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
of_table_feature_prop_next_tables_init(of_table_feature_prop_next_tables_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_NEXT_TABLES] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_NEXT_TABLES] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_NEXT_TABLES];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_TABLE_FEATURE_PROP_NEXT_TABLES;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_table_feature_prop_next_tables_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_table_feature_prop_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Bind an object of type of_list_uint8_t to the parent of type of_table_feature_prop_next_tables for
 * member next_table_ids
 * @param obj Pointer to an object of type of_table_feature_prop_next_tables.
 * @param next_table_ids Pointer to the child object of type
 * of_list_uint8_t to be filled out.
 * \ingroup of_table_feature_prop_next_tables
 *
 * The parameter next_table_ids is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_table_feature_prop_next_tables_next_table_ids_bind(
    of_table_feature_prop_next_tables_t *obj,
    of_list_uint8_t *next_table_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_NEXT_TABLES);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_uint8_init(next_table_ids, obj->version, 0, 1);
    /* Attach to parent */
    next_table_ids->parent = (of_object_t *)obj;
    next_table_ids->wire_object.wbuf = obj->wire_object.wbuf;
    next_table_ids->wire_object.obj_offset = abs_offset;
    next_table_ids->wire_object.owned = 0;
    next_table_ids->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of next_table_ids into a new variable of type of_list_uint8_t from
 * a of_table_feature_prop_next_tables instance.
 *
 * @param obj Pointer to the source of type of_table_feature_prop_next_tables_t
 * @returns A pointer to a new instance of type of_list_uint8_t whose contents
 * match that of next_table_ids from source
 * @returns NULL if an error occurs
 */
of_list_uint8_t *
of_table_feature_prop_next_tables_next_table_ids_get(of_table_feature_prop_next_tables_t *obj) {
    of_list_uint8_t _next_table_ids;
    of_list_uint8_t *_next_table_ids_ptr;

    of_table_feature_prop_next_tables_next_table_ids_bind(obj, &_next_table_ids);
    _next_table_ids_ptr = (of_list_uint8_t *)of_object_dup(&_next_table_ids);
    return _next_table_ids_ptr;
}

/**
 * Set next_table_ids in an object of type of_table_feature_prop_next_tables.
 * @param obj Pointer to an object of type of_table_feature_prop_next_tables.
 * @param next_table_ids Pointer to the child of type of_list_uint8_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_table_feature_prop_next_tables_next_table_ids_set(
    of_table_feature_prop_next_tables_t *obj,
    of_list_uint8_t *next_table_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_NEXT_TABLES);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = next_table_ids->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == next_table_ids->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(next_table_ids, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(next_table_ids, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (next_table_ids->wire_length_set != NULL) {
        next_table_ids->wire_length_set((of_object_t *)next_table_ids, next_table_ids->length);
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
of_table_feature_prop_next_tables_miss_push_wire_types(of_object_t *obj)
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
 * \defgroup of_table_feature_prop_next_tables_miss of_table_feature_prop_next_tables_miss
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_table_feature_prop_next_tables_miss_push_wire_values(of_table_feature_prop_next_tables_miss_t *obj)
{

    of_table_feature_prop_next_tables_miss_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_table_feature_prop_next_tables_miss object
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
 * \ingroup of_table_feature_prop_next_tables_miss
 */

of_table_feature_prop_next_tables_miss_t *
of_table_feature_prop_next_tables_miss_new(of_version_t version)
{
    of_table_feature_prop_next_tables_miss_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_NEXT_TABLES_MISS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_NEXT_TABLES_MISS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_table_feature_prop_next_tables_miss_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_table_feature_prop_next_tables_miss_init(obj, version, bytes, 0);

    if (of_table_feature_prop_next_tables_miss_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_table_feature_prop_next_tables_miss.
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
of_table_feature_prop_next_tables_miss_init(of_table_feature_prop_next_tables_miss_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_NEXT_TABLES_MISS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_NEXT_TABLES_MISS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_NEXT_TABLES_MISS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_TABLE_FEATURE_PROP_NEXT_TABLES_MISS;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_table_feature_prop_next_tables_miss_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_table_feature_prop_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Bind an object of type of_list_uint8_t to the parent of type of_table_feature_prop_next_tables_miss for
 * member next_table_ids
 * @param obj Pointer to an object of type of_table_feature_prop_next_tables_miss.
 * @param next_table_ids Pointer to the child object of type
 * of_list_uint8_t to be filled out.
 * \ingroup of_table_feature_prop_next_tables_miss
 *
 * The parameter next_table_ids is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_table_feature_prop_next_tables_miss_next_table_ids_bind(
    of_table_feature_prop_next_tables_miss_t *obj,
    of_list_uint8_t *next_table_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_NEXT_TABLES_MISS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_uint8_init(next_table_ids, obj->version, 0, 1);
    /* Attach to parent */
    next_table_ids->parent = (of_object_t *)obj;
    next_table_ids->wire_object.wbuf = obj->wire_object.wbuf;
    next_table_ids->wire_object.obj_offset = abs_offset;
    next_table_ids->wire_object.owned = 0;
    next_table_ids->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of next_table_ids into a new variable of type of_list_uint8_t from
 * a of_table_feature_prop_next_tables_miss instance.
 *
 * @param obj Pointer to the source of type of_table_feature_prop_next_tables_miss_t
 * @returns A pointer to a new instance of type of_list_uint8_t whose contents
 * match that of next_table_ids from source
 * @returns NULL if an error occurs
 */
of_list_uint8_t *
of_table_feature_prop_next_tables_miss_next_table_ids_get(of_table_feature_prop_next_tables_miss_t *obj) {
    of_list_uint8_t _next_table_ids;
    of_list_uint8_t *_next_table_ids_ptr;

    of_table_feature_prop_next_tables_miss_next_table_ids_bind(obj, &_next_table_ids);
    _next_table_ids_ptr = (of_list_uint8_t *)of_object_dup(&_next_table_ids);
    return _next_table_ids_ptr;
}

/**
 * Set next_table_ids in an object of type of_table_feature_prop_next_tables_miss.
 * @param obj Pointer to an object of type of_table_feature_prop_next_tables_miss.
 * @param next_table_ids Pointer to the child of type of_list_uint8_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_table_feature_prop_next_tables_miss_next_table_ids_set(
    of_table_feature_prop_next_tables_miss_t *obj,
    of_list_uint8_t *next_table_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_NEXT_TABLES_MISS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = next_table_ids->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == next_table_ids->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(next_table_ids, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(next_table_ids, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (next_table_ids->wire_length_set != NULL) {
        next_table_ids->wire_length_set((of_object_t *)next_table_ids, next_table_ids->length);
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
of_table_feature_prop_wildcards_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xa); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_table_feature_prop_wildcards of_table_feature_prop_wildcards
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_table_feature_prop_wildcards_push_wire_values(of_table_feature_prop_wildcards_t *obj)
{

    of_table_feature_prop_wildcards_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_table_feature_prop_wildcards object
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
 * \ingroup of_table_feature_prop_wildcards
 */

of_table_feature_prop_wildcards_t *
of_table_feature_prop_wildcards_new(of_version_t version)
{
    of_table_feature_prop_wildcards_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_WILDCARDS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_WILDCARDS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_table_feature_prop_wildcards_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_table_feature_prop_wildcards_init(obj, version, bytes, 0);

    if (of_table_feature_prop_wildcards_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_table_feature_prop_wildcards.
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
of_table_feature_prop_wildcards_init(of_table_feature_prop_wildcards_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_WILDCARDS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_WILDCARDS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_WILDCARDS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_TABLE_FEATURE_PROP_WILDCARDS;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_table_feature_prop_wildcards_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_table_feature_prop_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Bind an object of type of_list_uint32_t to the parent of type of_table_feature_prop_wildcards for
 * member oxm_ids
 * @param obj Pointer to an object of type of_table_feature_prop_wildcards.
 * @param oxm_ids Pointer to the child object of type
 * of_list_uint32_t to be filled out.
 * \ingroup of_table_feature_prop_wildcards
 *
 * The parameter oxm_ids is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_table_feature_prop_wildcards_oxm_ids_bind(
    of_table_feature_prop_wildcards_t *obj,
    of_list_uint32_t *oxm_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_WILDCARDS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_uint32_init(oxm_ids, obj->version, 0, 1);
    /* Attach to parent */
    oxm_ids->parent = (of_object_t *)obj;
    oxm_ids->wire_object.wbuf = obj->wire_object.wbuf;
    oxm_ids->wire_object.obj_offset = abs_offset;
    oxm_ids->wire_object.owned = 0;
    oxm_ids->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of oxm_ids into a new variable of type of_list_uint32_t from
 * a of_table_feature_prop_wildcards instance.
 *
 * @param obj Pointer to the source of type of_table_feature_prop_wildcards_t
 * @returns A pointer to a new instance of type of_list_uint32_t whose contents
 * match that of oxm_ids from source
 * @returns NULL if an error occurs
 */
of_list_uint32_t *
of_table_feature_prop_wildcards_oxm_ids_get(of_table_feature_prop_wildcards_t *obj) {
    of_list_uint32_t _oxm_ids;
    of_list_uint32_t *_oxm_ids_ptr;

    of_table_feature_prop_wildcards_oxm_ids_bind(obj, &_oxm_ids);
    _oxm_ids_ptr = (of_list_uint32_t *)of_object_dup(&_oxm_ids);
    return _oxm_ids_ptr;
}

/**
 * Set oxm_ids in an object of type of_table_feature_prop_wildcards.
 * @param obj Pointer to an object of type of_table_feature_prop_wildcards.
 * @param oxm_ids Pointer to the child of type of_list_uint32_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_table_feature_prop_wildcards_oxm_ids_set(
    of_table_feature_prop_wildcards_t *obj,
    of_list_uint32_t *oxm_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_WILDCARDS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = oxm_ids->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == oxm_ids->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(oxm_ids, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(oxm_ids, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (oxm_ids->wire_length_set != NULL) {
        oxm_ids->wire_length_set((of_object_t *)oxm_ids, oxm_ids->length);
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
of_table_feature_prop_write_actions_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x4); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_table_feature_prop_write_actions of_table_feature_prop_write_actions
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_table_feature_prop_write_actions_push_wire_values(of_table_feature_prop_write_actions_t *obj)
{

    of_table_feature_prop_write_actions_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_table_feature_prop_write_actions object
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
 * \ingroup of_table_feature_prop_write_actions
 */

of_table_feature_prop_write_actions_t *
of_table_feature_prop_write_actions_new(of_version_t version)
{
    of_table_feature_prop_write_actions_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_WRITE_ACTIONS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_WRITE_ACTIONS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_table_feature_prop_write_actions_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_table_feature_prop_write_actions_init(obj, version, bytes, 0);

    if (of_table_feature_prop_write_actions_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_table_feature_prop_write_actions.
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
of_table_feature_prop_write_actions_init(of_table_feature_prop_write_actions_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_WRITE_ACTIONS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_WRITE_ACTIONS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_WRITE_ACTIONS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_TABLE_FEATURE_PROP_WRITE_ACTIONS;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_table_feature_prop_write_actions_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_table_feature_prop_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Bind an object of type of_list_action_id_t to the parent of type of_table_feature_prop_write_actions for
 * member action_ids
 * @param obj Pointer to an object of type of_table_feature_prop_write_actions.
 * @param action_ids Pointer to the child object of type
 * of_list_action_id_t to be filled out.
 * \ingroup of_table_feature_prop_write_actions
 *
 * The parameter action_ids is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_table_feature_prop_write_actions_action_ids_bind(
    of_table_feature_prop_write_actions_t *obj,
    of_list_action_id_t *action_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_WRITE_ACTIONS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_action_id_init(action_ids, obj->version, 0, 1);
    /* Attach to parent */
    action_ids->parent = (of_object_t *)obj;
    action_ids->wire_object.wbuf = obj->wire_object.wbuf;
    action_ids->wire_object.obj_offset = abs_offset;
    action_ids->wire_object.owned = 0;
    action_ids->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of action_ids into a new variable of type of_list_action_id_t from
 * a of_table_feature_prop_write_actions instance.
 *
 * @param obj Pointer to the source of type of_table_feature_prop_write_actions_t
 * @returns A pointer to a new instance of type of_list_action_id_t whose contents
 * match that of action_ids from source
 * @returns NULL if an error occurs
 */
of_list_action_id_t *
of_table_feature_prop_write_actions_action_ids_get(of_table_feature_prop_write_actions_t *obj) {
    of_list_action_id_t _action_ids;
    of_list_action_id_t *_action_ids_ptr;

    of_table_feature_prop_write_actions_action_ids_bind(obj, &_action_ids);
    _action_ids_ptr = (of_list_action_id_t *)of_object_dup(&_action_ids);
    return _action_ids_ptr;
}

/**
 * Set action_ids in an object of type of_table_feature_prop_write_actions.
 * @param obj Pointer to an object of type of_table_feature_prop_write_actions.
 * @param action_ids Pointer to the child of type of_list_action_id_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_table_feature_prop_write_actions_action_ids_set(
    of_table_feature_prop_write_actions_t *obj,
    of_list_action_id_t *action_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_WRITE_ACTIONS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = action_ids->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == action_ids->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(action_ids, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(action_ids, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (action_ids->wire_length_set != NULL) {
        action_ids->wire_length_set((of_object_t *)action_ids, action_ids->length);
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
of_table_feature_prop_write_actions_miss_push_wire_types(of_object_t *obj)
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
 * \defgroup of_table_feature_prop_write_actions_miss of_table_feature_prop_write_actions_miss
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_table_feature_prop_write_actions_miss_push_wire_values(of_table_feature_prop_write_actions_miss_t *obj)
{

    of_table_feature_prop_write_actions_miss_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_table_feature_prop_write_actions_miss object
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
 * \ingroup of_table_feature_prop_write_actions_miss
 */

of_table_feature_prop_write_actions_miss_t *
of_table_feature_prop_write_actions_miss_new(of_version_t version)
{
    of_table_feature_prop_write_actions_miss_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_WRITE_ACTIONS_MISS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_WRITE_ACTIONS_MISS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_table_feature_prop_write_actions_miss_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_table_feature_prop_write_actions_miss_init(obj, version, bytes, 0);

    if (of_table_feature_prop_write_actions_miss_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_table_feature_prop_write_actions_miss.
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
of_table_feature_prop_write_actions_miss_init(of_table_feature_prop_write_actions_miss_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_WRITE_ACTIONS_MISS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_WRITE_ACTIONS_MISS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_WRITE_ACTIONS_MISS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_TABLE_FEATURE_PROP_WRITE_ACTIONS_MISS;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_table_feature_prop_write_actions_miss_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_table_feature_prop_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Bind an object of type of_list_action_id_t to the parent of type of_table_feature_prop_write_actions_miss for
 * member action_ids
 * @param obj Pointer to an object of type of_table_feature_prop_write_actions_miss.
 * @param action_ids Pointer to the child object of type
 * of_list_action_id_t to be filled out.
 * \ingroup of_table_feature_prop_write_actions_miss
 *
 * The parameter action_ids is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_table_feature_prop_write_actions_miss_action_ids_bind(
    of_table_feature_prop_write_actions_miss_t *obj,
    of_list_action_id_t *action_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_WRITE_ACTIONS_MISS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_action_id_init(action_ids, obj->version, 0, 1);
    /* Attach to parent */
    action_ids->parent = (of_object_t *)obj;
    action_ids->wire_object.wbuf = obj->wire_object.wbuf;
    action_ids->wire_object.obj_offset = abs_offset;
    action_ids->wire_object.owned = 0;
    action_ids->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of action_ids into a new variable of type of_list_action_id_t from
 * a of_table_feature_prop_write_actions_miss instance.
 *
 * @param obj Pointer to the source of type of_table_feature_prop_write_actions_miss_t
 * @returns A pointer to a new instance of type of_list_action_id_t whose contents
 * match that of action_ids from source
 * @returns NULL if an error occurs
 */
of_list_action_id_t *
of_table_feature_prop_write_actions_miss_action_ids_get(of_table_feature_prop_write_actions_miss_t *obj) {
    of_list_action_id_t _action_ids;
    of_list_action_id_t *_action_ids_ptr;

    of_table_feature_prop_write_actions_miss_action_ids_bind(obj, &_action_ids);
    _action_ids_ptr = (of_list_action_id_t *)of_object_dup(&_action_ids);
    return _action_ids_ptr;
}

/**
 * Set action_ids in an object of type of_table_feature_prop_write_actions_miss.
 * @param obj Pointer to an object of type of_table_feature_prop_write_actions_miss.
 * @param action_ids Pointer to the child of type of_list_action_id_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_table_feature_prop_write_actions_miss_action_ids_set(
    of_table_feature_prop_write_actions_miss_t *obj,
    of_list_action_id_t *action_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_WRITE_ACTIONS_MISS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = action_ids->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == action_ids->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(action_ids, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(action_ids, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (action_ids->wire_length_set != NULL) {
        action_ids->wire_length_set((of_object_t *)action_ids, action_ids->length);
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
of_table_feature_prop_write_setfield_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xc); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_table_feature_prop_write_setfield of_table_feature_prop_write_setfield
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_table_feature_prop_write_setfield_push_wire_values(of_table_feature_prop_write_setfield_t *obj)
{

    of_table_feature_prop_write_setfield_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_table_feature_prop_write_setfield object
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
 * \ingroup of_table_feature_prop_write_setfield
 */

of_table_feature_prop_write_setfield_t *
of_table_feature_prop_write_setfield_new(of_version_t version)
{
    of_table_feature_prop_write_setfield_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_WRITE_SETFIELD] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_WRITE_SETFIELD];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_table_feature_prop_write_setfield_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_table_feature_prop_write_setfield_init(obj, version, bytes, 0);

    if (of_table_feature_prop_write_setfield_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_table_feature_prop_write_setfield.
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
of_table_feature_prop_write_setfield_init(of_table_feature_prop_write_setfield_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_WRITE_SETFIELD] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_WRITE_SETFIELD] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_WRITE_SETFIELD];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_TABLE_FEATURE_PROP_WRITE_SETFIELD;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_table_feature_prop_write_setfield_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_table_feature_prop_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Bind an object of type of_list_uint32_t to the parent of type of_table_feature_prop_write_setfield for
 * member oxm_ids
 * @param obj Pointer to an object of type of_table_feature_prop_write_setfield.
 * @param oxm_ids Pointer to the child object of type
 * of_list_uint32_t to be filled out.
 * \ingroup of_table_feature_prop_write_setfield
 *
 * The parameter oxm_ids is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_table_feature_prop_write_setfield_oxm_ids_bind(
    of_table_feature_prop_write_setfield_t *obj,
    of_list_uint32_t *oxm_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_WRITE_SETFIELD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_uint32_init(oxm_ids, obj->version, 0, 1);
    /* Attach to parent */
    oxm_ids->parent = (of_object_t *)obj;
    oxm_ids->wire_object.wbuf = obj->wire_object.wbuf;
    oxm_ids->wire_object.obj_offset = abs_offset;
    oxm_ids->wire_object.owned = 0;
    oxm_ids->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of oxm_ids into a new variable of type of_list_uint32_t from
 * a of_table_feature_prop_write_setfield instance.
 *
 * @param obj Pointer to the source of type of_table_feature_prop_write_setfield_t
 * @returns A pointer to a new instance of type of_list_uint32_t whose contents
 * match that of oxm_ids from source
 * @returns NULL if an error occurs
 */
of_list_uint32_t *
of_table_feature_prop_write_setfield_oxm_ids_get(of_table_feature_prop_write_setfield_t *obj) {
    of_list_uint32_t _oxm_ids;
    of_list_uint32_t *_oxm_ids_ptr;

    of_table_feature_prop_write_setfield_oxm_ids_bind(obj, &_oxm_ids);
    _oxm_ids_ptr = (of_list_uint32_t *)of_object_dup(&_oxm_ids);
    return _oxm_ids_ptr;
}

/**
 * Set oxm_ids in an object of type of_table_feature_prop_write_setfield.
 * @param obj Pointer to an object of type of_table_feature_prop_write_setfield.
 * @param oxm_ids Pointer to the child of type of_list_uint32_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_table_feature_prop_write_setfield_oxm_ids_set(
    of_table_feature_prop_write_setfield_t *obj,
    of_list_uint32_t *oxm_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_WRITE_SETFIELD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = oxm_ids->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == oxm_ids->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(oxm_ids, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(oxm_ids, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (oxm_ids->wire_length_set != NULL) {
        oxm_ids->wire_length_set((of_object_t *)oxm_ids, oxm_ids->length);
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
of_table_feature_prop_write_setfield_miss_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0xd); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_table_feature_prop_write_setfield_miss of_table_feature_prop_write_setfield_miss
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_table_feature_prop_write_setfield_miss_push_wire_values(of_table_feature_prop_write_setfield_miss_t *obj)
{

    of_table_feature_prop_write_setfield_miss_push_wire_types(obj);

    /* TLV obj; set length */
    of_tlv16_wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_table_feature_prop_write_setfield_miss object
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
 * \ingroup of_table_feature_prop_write_setfield_miss
 */

of_table_feature_prop_write_setfield_miss_t *
of_table_feature_prop_write_setfield_miss_new(of_version_t version)
{
    of_table_feature_prop_write_setfield_miss_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_WRITE_SETFIELD_MISS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_WRITE_SETFIELD_MISS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_table_feature_prop_write_setfield_miss_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_table_feature_prop_write_setfield_miss_init(obj, version, bytes, 0);

    if (of_table_feature_prop_write_setfield_miss_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_table_feature_prop_write_setfield_miss.
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
of_table_feature_prop_write_setfield_miss_init(of_table_feature_prop_write_setfield_miss_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_WRITE_SETFIELD_MISS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_TABLE_FEATURE_PROP_WRITE_SETFIELD_MISS] + of_object_extra_len[version][OF_TABLE_FEATURE_PROP_WRITE_SETFIELD_MISS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_TABLE_FEATURE_PROP_WRITE_SETFIELD_MISS;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_table_feature_prop_write_setfield_miss_push_wire_types;

    obj->wire_length_set = of_tlv16_wire_length_set;

    obj->wire_length_get = of_tlv16_wire_length_get;

    obj->wire_type_get = of_table_feature_prop_wire_object_id_get;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Bind an object of type of_list_uint32_t to the parent of type of_table_feature_prop_write_setfield_miss for
 * member oxm_ids
 * @param obj Pointer to an object of type of_table_feature_prop_write_setfield_miss.
 * @param oxm_ids Pointer to the child object of type
 * of_list_uint32_t to be filled out.
 * \ingroup of_table_feature_prop_write_setfield_miss
 *
 * The parameter oxm_ids is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_table_feature_prop_write_setfield_miss_oxm_ids_bind(
    of_table_feature_prop_write_setfield_miss_t *obj,
    of_list_uint32_t *oxm_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_WRITE_SETFIELD_MISS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_uint32_init(oxm_ids, obj->version, 0, 1);
    /* Attach to parent */
    oxm_ids->parent = (of_object_t *)obj;
    oxm_ids->wire_object.wbuf = obj->wire_object.wbuf;
    oxm_ids->wire_object.obj_offset = abs_offset;
    oxm_ids->wire_object.owned = 0;
    oxm_ids->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of oxm_ids into a new variable of type of_list_uint32_t from
 * a of_table_feature_prop_write_setfield_miss instance.
 *
 * @param obj Pointer to the source of type of_table_feature_prop_write_setfield_miss_t
 * @returns A pointer to a new instance of type of_list_uint32_t whose contents
 * match that of oxm_ids from source
 * @returns NULL if an error occurs
 */
of_list_uint32_t *
of_table_feature_prop_write_setfield_miss_oxm_ids_get(of_table_feature_prop_write_setfield_miss_t *obj) {
    of_list_uint32_t _oxm_ids;
    of_list_uint32_t *_oxm_ids_ptr;

    of_table_feature_prop_write_setfield_miss_oxm_ids_bind(obj, &_oxm_ids);
    _oxm_ids_ptr = (of_list_uint32_t *)of_object_dup(&_oxm_ids);
    return _oxm_ids_ptr;
}

/**
 * Set oxm_ids in an object of type of_table_feature_prop_write_setfield_miss.
 * @param obj Pointer to an object of type of_table_feature_prop_write_setfield_miss.
 * @param oxm_ids Pointer to the child of type of_list_uint32_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_table_feature_prop_write_setfield_miss_oxm_ids_set(
    of_table_feature_prop_write_setfield_miss_t *obj,
    of_list_uint32_t *oxm_ids)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURE_PROP_WRITE_SETFIELD_MISS);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 4;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = oxm_ids->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == oxm_ids->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(oxm_ids, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(oxm_ids, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (oxm_ids->wire_length_set != NULL) {
        oxm_ids->wire_length_set((of_object_t *)oxm_ids, oxm_ids->length);
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
 * \defgroup of_table_features of_table_features
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_table_features_push_wire_values(of_table_features_t *obj)
{

    obj->wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_table_features object
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
 * \ingroup of_table_features
 */

of_table_features_t *
of_table_features_new(of_version_t version)
{
    of_table_features_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_TABLE_FEATURES] + of_object_extra_len[version][OF_TABLE_FEATURES];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_table_features_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_table_features_init(obj, version, bytes, 0);

    if (of_table_features_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_table_features.
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
of_table_features_init(of_table_features_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_TABLE_FEATURES] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_TABLE_FEATURES] + of_object_extra_len[version][OF_TABLE_FEATURES];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_TABLE_FEATURES;

    /* Set up the object's function pointers */

    obj->wire_length_get = of_u16_len_wire_length_get;
    obj->wire_length_set = of_u16_len_wire_length_set;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get table_id from an object of type of_table_features.
 * @param obj Pointer to an object of type of_table_features.
 * @param table_id Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_table_features_table_id_get(
    of_table_features_t *obj,
    uint8_t *table_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, table_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set table_id in an object of type of_table_features.
 * @param obj Pointer to an object of type of_table_features.
 * @param table_id The value to write into the object
 */
void
of_table_features_table_id_set(
    of_table_features_t *obj,
    uint8_t table_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES);
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
    of_wire_buffer_u8_set(wbuf, abs_offset, table_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get name from an object of type of_table_features.
 * @param obj Pointer to an object of type of_table_features.
 * @param name Pointer to the child object of type
 * of_table_name_t to be filled out.
 *
 */
void
of_table_features_name_get(
    of_table_features_t *obj,
    of_table_name_t *name)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES);
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
    of_wire_buffer_tab_name_get(wbuf, abs_offset, name);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set name in an object of type of_table_features.
 * @param obj Pointer to an object of type of_table_features.
 * @param name The value to write into the object
 */
void
of_table_features_name_set(
    of_table_features_t *obj,
    of_table_name_t name)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES);
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
    of_wire_buffer_tab_name_set(wbuf, abs_offset, name);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get metadata_match from an object of type of_table_features.
 * @param obj Pointer to an object of type of_table_features.
 * @param metadata_match Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_table_features_metadata_match_get(
    of_table_features_t *obj,
    uint64_t *metadata_match)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES);
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
    of_wire_buffer_u64_get(wbuf, abs_offset, metadata_match);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set metadata_match in an object of type of_table_features.
 * @param obj Pointer to an object of type of_table_features.
 * @param metadata_match The value to write into the object
 */
void
of_table_features_metadata_match_set(
    of_table_features_t *obj,
    uint64_t metadata_match)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES);
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
    of_wire_buffer_u64_set(wbuf, abs_offset, metadata_match);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get metadata_write from an object of type of_table_features.
 * @param obj Pointer to an object of type of_table_features.
 * @param metadata_write Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_table_features_metadata_write_get(
    of_table_features_t *obj,
    uint64_t *metadata_write)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES);
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
    of_wire_buffer_u64_get(wbuf, abs_offset, metadata_write);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set metadata_write in an object of type of_table_features.
 * @param obj Pointer to an object of type of_table_features.
 * @param metadata_write The value to write into the object
 */
void
of_table_features_metadata_write_set(
    of_table_features_t *obj,
    uint64_t metadata_write)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES);
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
    of_wire_buffer_u64_set(wbuf, abs_offset, metadata_write);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get config from an object of type of_table_features.
 * @param obj Pointer to an object of type of_table_features.
 * @param config Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_table_features_config_get(
    of_table_features_t *obj,
    uint32_t *config)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, config);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set config in an object of type of_table_features.
 * @param obj Pointer to an object of type of_table_features.
 * @param config The value to write into the object
 */
void
of_table_features_config_set(
    of_table_features_t *obj,
    uint32_t config)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, config);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get max_entries from an object of type of_table_features.
 * @param obj Pointer to an object of type of_table_features.
 * @param max_entries Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_table_features_max_entries_get(
    of_table_features_t *obj,
    uint32_t *max_entries)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 60;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, max_entries);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set max_entries in an object of type of_table_features.
 * @param obj Pointer to an object of type of_table_features.
 * @param max_entries The value to write into the object
 */
void
of_table_features_max_entries_set(
    of_table_features_t *obj,
    uint32_t max_entries)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 60;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, max_entries);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Bind an object of type of_list_table_feature_prop_t to the parent of type of_table_features for
 * member properties
 * @param obj Pointer to an object of type of_table_features.
 * @param properties Pointer to the child object of type
 * of_list_table_feature_prop_t to be filled out.
 * \ingroup of_table_features
 *
 * The parameter properties is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_table_features_properties_bind(
    of_table_features_t *obj,
    of_list_table_feature_prop_t *properties)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 64;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_table_feature_prop_init(properties, obj->version, 0, 1);
    /* Attach to parent */
    properties->parent = (of_object_t *)obj;
    properties->wire_object.wbuf = obj->wire_object.wbuf;
    properties->wire_object.obj_offset = abs_offset;
    properties->wire_object.owned = 0;
    properties->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of properties into a new variable of type of_list_table_feature_prop_t from
 * a of_table_features instance.
 *
 * @param obj Pointer to the source of type of_table_features_t
 * @returns A pointer to a new instance of type of_list_table_feature_prop_t whose contents
 * match that of properties from source
 * @returns NULL if an error occurs
 */
of_list_table_feature_prop_t *
of_table_features_properties_get(of_table_features_t *obj) {
    of_list_table_feature_prop_t _properties;
    of_list_table_feature_prop_t *_properties_ptr;

    of_table_features_properties_bind(obj, &_properties);
    _properties_ptr = (of_list_table_feature_prop_t *)of_object_dup(&_properties);
    return _properties_ptr;
}

/**
 * Set properties in an object of type of_table_features.
 * @param obj Pointer to an object of type of_table_features.
 * @param properties Pointer to the child of type of_list_table_feature_prop_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_table_features_properties_set(
    of_table_features_t *obj,
    of_list_table_feature_prop_t *properties)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 64;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = properties->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == properties->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(properties, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(properties, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (properties->wire_length_set != NULL) {
        properties->wire_length_set((of_object_t *)properties, properties->length);
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
of_table_features_failed_error_msg_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x1; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xd); /* err_type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_table_features_failed_error_msg of_table_features_failed_error_msg
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_table_features_failed_error_msg_push_wire_values(of_table_features_failed_error_msg_t *obj)
{

    of_table_features_failed_error_msg_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_table_features_failed_error_msg object
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
 * \ingroup of_table_features_failed_error_msg
 */

of_table_features_failed_error_msg_t *
of_table_features_failed_error_msg_new(of_version_t version)
{
    of_table_features_failed_error_msg_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_TABLE_FEATURES_FAILED_ERROR_MSG] + of_object_extra_len[version][OF_TABLE_FEATURES_FAILED_ERROR_MSG];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_table_features_failed_error_msg_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_table_features_failed_error_msg_init(obj, version, bytes, 0);

    if (of_table_features_failed_error_msg_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_table_features_failed_error_msg.
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
of_table_features_failed_error_msg_init(of_table_features_failed_error_msg_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_TABLE_FEATURES_FAILED_ERROR_MSG] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_TABLE_FEATURES_FAILED_ERROR_MSG] + of_object_extra_len[version][OF_TABLE_FEATURES_FAILED_ERROR_MSG];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_TABLE_FEATURES_FAILED_ERROR_MSG;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_table_features_failed_error_msg_push_wire_types;

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
 * Create a new of_table_features_failed_error_msg object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_table_features_failed_error_msg
 */

of_table_features_failed_error_msg_t *
of_table_features_failed_error_msg_new_from_message(of_message_t msg)
{
    of_table_features_failed_error_msg_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_table_features_failed_error_msg_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_table_features_failed_error_msg_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_table_features_failed_error_msg.
 * @param obj Pointer to an object of type of_table_features_failed_error_msg.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_table_features_failed_error_msg_xid_get(
    of_table_features_failed_error_msg_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES_FAILED_ERROR_MSG);
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
 * Set xid in an object of type of_table_features_failed_error_msg.
 * @param obj Pointer to an object of type of_table_features_failed_error_msg.
 * @param xid The value to write into the object
 */
void
of_table_features_failed_error_msg_xid_set(
    of_table_features_failed_error_msg_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES_FAILED_ERROR_MSG);
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
 * Get code from an object of type of_table_features_failed_error_msg.
 * @param obj Pointer to an object of type of_table_features_failed_error_msg.
 * @param code Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_table_features_failed_error_msg_code_get(
    of_table_features_failed_error_msg_t *obj,
    uint16_t *code)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES_FAILED_ERROR_MSG);
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
 * Set code in an object of type of_table_features_failed_error_msg.
 * @param obj Pointer to an object of type of_table_features_failed_error_msg.
 * @param code The value to write into the object
 */
void
of_table_features_failed_error_msg_code_set(
    of_table_features_failed_error_msg_t *obj,
    uint16_t code)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES_FAILED_ERROR_MSG);
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
 * Get data from an object of type of_table_features_failed_error_msg.
 * @param obj Pointer to an object of type of_table_features_failed_error_msg.
 * @param data Pointer to the child object of type
 * of_octets_t to be filled out.
 *
 */
void
of_table_features_failed_error_msg_data_get(
    of_table_features_failed_error_msg_t *obj,
    of_octets_t *data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES_FAILED_ERROR_MSG);
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
 * Set data in an object of type of_table_features_failed_error_msg.
 * @param obj Pointer to an object of type of_table_features_failed_error_msg.
 * @param data The value to write into the object
 */
int WARN_UNUSED_RESULT
of_table_features_failed_error_msg_data_set(
    of_table_features_failed_error_msg_t *obj,
    of_octets_t *data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES_FAILED_ERROR_MSG);
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
of_table_features_stats_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xc); /* stats_type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_table_features_stats_reply of_table_features_stats_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_table_features_stats_reply_push_wire_values(of_table_features_stats_reply_t *obj)
{

    of_table_features_stats_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_table_features_stats_reply object
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
 * \ingroup of_table_features_stats_reply
 */

of_table_features_stats_reply_t *
of_table_features_stats_reply_new(of_version_t version)
{
    of_table_features_stats_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_TABLE_FEATURES_STATS_REPLY] + of_object_extra_len[version][OF_TABLE_FEATURES_STATS_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_table_features_stats_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_table_features_stats_reply_init(obj, version, bytes, 0);

    if (of_table_features_stats_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_table_features_stats_reply.
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
of_table_features_stats_reply_init(of_table_features_stats_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_TABLE_FEATURES_STATS_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_TABLE_FEATURES_STATS_REPLY] + of_object_extra_len[version][OF_TABLE_FEATURES_STATS_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_TABLE_FEATURES_STATS_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_table_features_stats_reply_push_wire_types;

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
 * Create a new of_table_features_stats_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_table_features_stats_reply
 */

of_table_features_stats_reply_t *
of_table_features_stats_reply_new_from_message(of_message_t msg)
{
    of_table_features_stats_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_table_features_stats_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_table_features_stats_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_table_features_stats_reply.
 * @param obj Pointer to an object of type of_table_features_stats_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_table_features_stats_reply_xid_get(
    of_table_features_stats_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES_STATS_REPLY);
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
 * Set xid in an object of type of_table_features_stats_reply.
 * @param obj Pointer to an object of type of_table_features_stats_reply.
 * @param xid The value to write into the object
 */
void
of_table_features_stats_reply_xid_set(
    of_table_features_stats_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES_STATS_REPLY);
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
 * Get flags from an object of type of_table_features_stats_reply.
 * @param obj Pointer to an object of type of_table_features_stats_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_table_features_stats_reply_flags_get(
    of_table_features_stats_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES_STATS_REPLY);
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
 * Set flags in an object of type of_table_features_stats_reply.
 * @param obj Pointer to an object of type of_table_features_stats_reply.
 * @param flags The value to write into the object
 */
void
of_table_features_stats_reply_flags_set(
    of_table_features_stats_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES_STATS_REPLY);
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
 * Bind an object of type of_list_table_features_t to the parent of type of_table_features_stats_reply for
 * member entries
 * @param obj Pointer to an object of type of_table_features_stats_reply.
 * @param entries Pointer to the child object of type
 * of_list_table_features_t to be filled out.
 * \ingroup of_table_features_stats_reply
 *
 * The parameter entries is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_table_features_stats_reply_entries_bind(
    of_table_features_stats_reply_t *obj,
    of_list_table_features_t *entries)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES_STATS_REPLY);
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
    of_list_table_features_init(entries, obj->version, 0, 1);
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
 * Create a copy of entries into a new variable of type of_list_table_features_t from
 * a of_table_features_stats_reply instance.
 *
 * @param obj Pointer to the source of type of_table_features_stats_reply_t
 * @returns A pointer to a new instance of type of_list_table_features_t whose contents
 * match that of entries from source
 * @returns NULL if an error occurs
 */
of_list_table_features_t *
of_table_features_stats_reply_entries_get(of_table_features_stats_reply_t *obj) {
    of_list_table_features_t _entries;
    of_list_table_features_t *_entries_ptr;

    of_table_features_stats_reply_entries_bind(obj, &_entries);
    _entries_ptr = (of_list_table_features_t *)of_object_dup(&_entries);
    return _entries_ptr;
}

/**
 * Set entries in an object of type of_table_features_stats_reply.
 * @param obj Pointer to an object of type of_table_features_stats_reply.
 * @param entries Pointer to the child of type of_list_table_features_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_table_features_stats_reply_entries_set(
    of_table_features_stats_reply_t *obj,
    of_list_table_features_t *entries)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES_STATS_REPLY);
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
of_table_features_stats_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xc); /* stats_type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_table_features_stats_request of_table_features_stats_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_table_features_stats_request_push_wire_values(of_table_features_stats_request_t *obj)
{

    of_table_features_stats_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_table_features_stats_request object
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
 * \ingroup of_table_features_stats_request
 */

of_table_features_stats_request_t *
of_table_features_stats_request_new(of_version_t version)
{
    of_table_features_stats_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_TABLE_FEATURES_STATS_REQUEST] + of_object_extra_len[version][OF_TABLE_FEATURES_STATS_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_table_features_stats_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_table_features_stats_request_init(obj, version, bytes, 0);

    if (of_table_features_stats_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_table_features_stats_request.
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
of_table_features_stats_request_init(of_table_features_stats_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_TABLE_FEATURES_STATS_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_TABLE_FEATURES_STATS_REQUEST] + of_object_extra_len[version][OF_TABLE_FEATURES_STATS_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_TABLE_FEATURES_STATS_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_table_features_stats_request_push_wire_types;

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
 * Create a new of_table_features_stats_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_table_features_stats_request
 */

of_table_features_stats_request_t *
of_table_features_stats_request_new_from_message(of_message_t msg)
{
    of_table_features_stats_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_table_features_stats_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_table_features_stats_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_table_features_stats_request.
 * @param obj Pointer to an object of type of_table_features_stats_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_table_features_stats_request_xid_get(
    of_table_features_stats_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES_STATS_REQUEST);
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
 * Set xid in an object of type of_table_features_stats_request.
 * @param obj Pointer to an object of type of_table_features_stats_request.
 * @param xid The value to write into the object
 */
void
of_table_features_stats_request_xid_set(
    of_table_features_stats_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES_STATS_REQUEST);
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
 * Get flags from an object of type of_table_features_stats_request.
 * @param obj Pointer to an object of type of_table_features_stats_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_table_features_stats_request_flags_get(
    of_table_features_stats_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES_STATS_REQUEST);
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
 * Set flags in an object of type of_table_features_stats_request.
 * @param obj Pointer to an object of type of_table_features_stats_request.
 * @param flags The value to write into the object
 */
void
of_table_features_stats_request_flags_set(
    of_table_features_stats_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES_STATS_REQUEST);
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
 * Bind an object of type of_list_table_features_t to the parent of type of_table_features_stats_request for
 * member entries
 * @param obj Pointer to an object of type of_table_features_stats_request.
 * @param entries Pointer to the child object of type
 * of_list_table_features_t to be filled out.
 * \ingroup of_table_features_stats_request
 *
 * The parameter entries is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_table_features_stats_request_entries_bind(
    of_table_features_stats_request_t *obj,
    of_list_table_features_t *entries)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES_STATS_REQUEST);
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
    of_list_table_features_init(entries, obj->version, 0, 1);
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
 * Create a copy of entries into a new variable of type of_list_table_features_t from
 * a of_table_features_stats_request instance.
 *
 * @param obj Pointer to the source of type of_table_features_stats_request_t
 * @returns A pointer to a new instance of type of_list_table_features_t whose contents
 * match that of entries from source
 * @returns NULL if an error occurs
 */
of_list_table_features_t *
of_table_features_stats_request_entries_get(of_table_features_stats_request_t *obj) {
    of_list_table_features_t _entries;
    of_list_table_features_t *_entries_ptr;

    of_table_features_stats_request_entries_bind(obj, &_entries);
    _entries_ptr = (of_list_table_features_t *)of_object_dup(&_entries);
    return _entries_ptr;
}

/**
 * Set entries in an object of type of_table_features_stats_request.
 * @param obj Pointer to an object of type of_table_features_stats_request.
 * @param entries Pointer to the child of type of_list_table_features_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_table_features_stats_request_entries_set(
    of_table_features_stats_request_t *obj,
    of_list_table_features_t *entries)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_TABLE_FEATURES_STATS_REQUEST);
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



/**
 * \defgroup of_uint32 of_uint32
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_uint32_push_wire_values(of_uint32_t *obj)
{

    return OF_ERROR_NONE;
}

/**
 * Create a new of_uint32 object
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
 * \ingroup of_uint32
 */

of_uint32_t *
of_uint32_new(of_version_t version)
{
    of_uint32_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_UINT32] + of_object_extra_len[version][OF_UINT32];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_uint32_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_uint32_init(obj, version, bytes, 0);

    if (of_uint32_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_uint32.
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
of_uint32_init(of_uint32_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_UINT32] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_UINT32] + of_object_extra_len[version][OF_UINT32];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_UINT32;

    /* Set up the object's function pointers */

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_uint32.
 * @param obj Pointer to an object of type of_uint32.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_uint32_value_get(
    of_uint32_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_UINT32);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_uint32.
 * @param obj Pointer to an object of type of_uint32.
 * @param value The value to write into the object
 */
void
of_uint32_value_set(
    of_uint32_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_UINT32);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, value);

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
 * \defgroup of_uint64 of_uint64
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_uint64_push_wire_values(of_uint64_t *obj)
{

    return OF_ERROR_NONE;
}

/**
 * Create a new of_uint64 object
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
 * \ingroup of_uint64
 */

of_uint64_t *
of_uint64_new(of_version_t version)
{
    of_uint64_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_UINT64] + of_object_extra_len[version][OF_UINT64];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_uint64_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_uint64_init(obj, version, bytes, 0);

    if (of_uint64_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_uint64.
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
of_uint64_init(of_uint64_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_UINT64] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_UINT64] + of_object_extra_len[version][OF_UINT64];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_UINT64;

    /* Set up the object's function pointers */

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_uint64.
 * @param obj Pointer to an object of type of_uint64.
 * @param value Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_uint64_value_get(
    of_uint64_t *obj,
    uint64_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_UINT64);
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
    of_wire_buffer_u64_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_uint64.
 * @param obj Pointer to an object of type of_uint64.
 * @param value The value to write into the object
 */
void
of_uint64_value_set(
    of_uint64_t *obj,
    uint64_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_UINT64);
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
    of_wire_buffer_u64_set(wbuf, abs_offset, value);

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
 * \defgroup of_uint8 of_uint8
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_uint8_push_wire_values(of_uint8_t *obj)
{

    return OF_ERROR_NONE;
}

/**
 * Create a new of_uint8 object
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
 * \ingroup of_uint8
 */

of_uint8_t *
of_uint8_new(of_version_t version)
{
    of_uint8_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_UINT8] + of_object_extra_len[version][OF_UINT8];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_uint8_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_uint8_init(obj, version, bytes, 0);

    if (of_uint8_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_uint8.
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
of_uint8_init(of_uint8_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_UINT8] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_UINT8] + of_object_extra_len[version][OF_UINT8];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_UINT8;

    /* Set up the object's function pointers */

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type of_uint8.
 * @param obj Pointer to an object of type of_uint8.
 * @param value Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_uint8_value_get(
    of_uint8_t *obj,
    uint8_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_UINT8);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_uint8.
 * @param obj Pointer to an object of type of_uint8.
 * @param value The value to write into the object
 */
void
of_uint8_value_set(
    of_uint8_t *obj,
    uint8_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_UINT8);
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
    of_wire_buffer_u8_set(wbuf, abs_offset, value);

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
 * \defgroup ofp_queue_desc ofp_queue_desc
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
ofp_queue_desc_push_wire_values(ofp_queue_desc_t *obj)
{

    return OF_ERROR_NONE;
}

/**
 * Create a new ofp_queue_desc object
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
 * \ingroup ofp_queue_desc
 */

ofp_queue_desc_t *
ofp_queue_desc_new(of_version_t version)
{
    ofp_queue_desc_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OFP_QUEUE_DESC] + of_object_extra_len[version][OFP_QUEUE_DESC];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (ofp_queue_desc_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    ofp_queue_desc_init(obj, version, bytes, 0);

    if (ofp_queue_desc_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type ofp_queue_desc.
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
ofp_queue_desc_init(ofp_queue_desc_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OFP_QUEUE_DESC] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OFP_QUEUE_DESC] + of_object_extra_len[version][OFP_QUEUE_DESC];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OFP_QUEUE_DESC;

    /* Set up the object's function pointers */

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get port_no from an object of type ofp_queue_desc.
 * @param obj Pointer to an object of type ofp_queue_desc.
 * @param port_no Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofp_queue_desc_port_no_get(
    ofp_queue_desc_t *obj,
    uint32_t *port_no)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFP_QUEUE_DESC);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, port_no);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set port_no in an object of type ofp_queue_desc.
 * @param obj Pointer to an object of type ofp_queue_desc.
 * @param port_no The value to write into the object
 */
void
ofp_queue_desc_port_no_set(
    ofp_queue_desc_t *obj,
    uint32_t port_no)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFP_QUEUE_DESC);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, port_no);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get queue_id from an object of type ofp_queue_desc.
 * @param obj Pointer to an object of type ofp_queue_desc.
 * @param queue_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
ofp_queue_desc_queue_id_get(
    ofp_queue_desc_t *obj,
    uint32_t *queue_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFP_QUEUE_DESC);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, queue_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set queue_id in an object of type ofp_queue_desc.
 * @param obj Pointer to an object of type ofp_queue_desc.
 * @param queue_id The value to write into the object
 */
void
ofp_queue_desc_queue_id_set(
    ofp_queue_desc_t *obj,
    uint32_t queue_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OFP_QUEUE_DESC);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, queue_id);

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
onf_oxm_packet_regs0_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x80010008); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup onf_oxm_packet_regs0 onf_oxm_packet_regs0
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
onf_oxm_packet_regs0_push_wire_values(onf_oxm_packet_regs0_t *obj)
{

    onf_oxm_packet_regs0_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new onf_oxm_packet_regs0 object
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
 * \ingroup onf_oxm_packet_regs0
 */

onf_oxm_packet_regs0_t *
onf_oxm_packet_regs0_new(of_version_t version)
{
    onf_oxm_packet_regs0_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][ONF_OXM_PACKET_REGS0] + of_object_extra_len[version][ONF_OXM_PACKET_REGS0];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (onf_oxm_packet_regs0_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    onf_oxm_packet_regs0_init(obj, version, bytes, 0);

    if (onf_oxm_packet_regs0_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type onf_oxm_packet_regs0.
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
onf_oxm_packet_regs0_init(onf_oxm_packet_regs0_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][ONF_OXM_PACKET_REGS0] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][ONF_OXM_PACKET_REGS0] + of_object_extra_len[version][ONF_OXM_PACKET_REGS0];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = ONF_OXM_PACKET_REGS0;

    /* Set up the object's function pointers */

    obj->wire_type_set = onf_oxm_packet_regs0_push_wire_types;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type onf_oxm_packet_regs0.
 * @param obj Pointer to an object of type onf_oxm_packet_regs0.
 * @param value Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
onf_oxm_packet_regs0_value_get(
    onf_oxm_packet_regs0_t *obj,
    uint64_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == ONF_OXM_PACKET_REGS0);
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
    of_wire_buffer_u64_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type onf_oxm_packet_regs0.
 * @param obj Pointer to an object of type onf_oxm_packet_regs0.
 * @param value The value to write into the object
 */
void
onf_oxm_packet_regs0_value_set(
    onf_oxm_packet_regs0_t *obj,
    uint64_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == ONF_OXM_PACKET_REGS0);
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
    of_wire_buffer_u64_set(wbuf, abs_offset, value);

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
onf_oxm_packet_regs1_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x80010208); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup onf_oxm_packet_regs1 onf_oxm_packet_regs1
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
onf_oxm_packet_regs1_push_wire_values(onf_oxm_packet_regs1_t *obj)
{

    onf_oxm_packet_regs1_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new onf_oxm_packet_regs1 object
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
 * \ingroup onf_oxm_packet_regs1
 */

onf_oxm_packet_regs1_t *
onf_oxm_packet_regs1_new(of_version_t version)
{
    onf_oxm_packet_regs1_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][ONF_OXM_PACKET_REGS1] + of_object_extra_len[version][ONF_OXM_PACKET_REGS1];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (onf_oxm_packet_regs1_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    onf_oxm_packet_regs1_init(obj, version, bytes, 0);

    if (onf_oxm_packet_regs1_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type onf_oxm_packet_regs1.
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
onf_oxm_packet_regs1_init(onf_oxm_packet_regs1_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][ONF_OXM_PACKET_REGS1] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][ONF_OXM_PACKET_REGS1] + of_object_extra_len[version][ONF_OXM_PACKET_REGS1];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = ONF_OXM_PACKET_REGS1;

    /* Set up the object's function pointers */

    obj->wire_type_set = onf_oxm_packet_regs1_push_wire_types;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type onf_oxm_packet_regs1.
 * @param obj Pointer to an object of type onf_oxm_packet_regs1.
 * @param value Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
onf_oxm_packet_regs1_value_get(
    onf_oxm_packet_regs1_t *obj,
    uint64_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == ONF_OXM_PACKET_REGS1);
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
    of_wire_buffer_u64_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type onf_oxm_packet_regs1.
 * @param obj Pointer to an object of type onf_oxm_packet_regs1.
 * @param value The value to write into the object
 */
void
onf_oxm_packet_regs1_value_set(
    onf_oxm_packet_regs1_t *obj,
    uint64_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == ONF_OXM_PACKET_REGS1);
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
    of_wire_buffer_u64_set(wbuf, abs_offset, value);

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
onf_oxm_packet_regs2_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x80010408); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup onf_oxm_packet_regs2 onf_oxm_packet_regs2
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
onf_oxm_packet_regs2_push_wire_values(onf_oxm_packet_regs2_t *obj)
{

    onf_oxm_packet_regs2_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new onf_oxm_packet_regs2 object
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
 * \ingroup onf_oxm_packet_regs2
 */

onf_oxm_packet_regs2_t *
onf_oxm_packet_regs2_new(of_version_t version)
{
    onf_oxm_packet_regs2_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][ONF_OXM_PACKET_REGS2] + of_object_extra_len[version][ONF_OXM_PACKET_REGS2];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (onf_oxm_packet_regs2_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    onf_oxm_packet_regs2_init(obj, version, bytes, 0);

    if (onf_oxm_packet_regs2_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type onf_oxm_packet_regs2.
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
onf_oxm_packet_regs2_init(onf_oxm_packet_regs2_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][ONF_OXM_PACKET_REGS2] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][ONF_OXM_PACKET_REGS2] + of_object_extra_len[version][ONF_OXM_PACKET_REGS2];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = ONF_OXM_PACKET_REGS2;

    /* Set up the object's function pointers */

    obj->wire_type_set = onf_oxm_packet_regs2_push_wire_types;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type onf_oxm_packet_regs2.
 * @param obj Pointer to an object of type onf_oxm_packet_regs2.
 * @param value Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
onf_oxm_packet_regs2_value_get(
    onf_oxm_packet_regs2_t *obj,
    uint64_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == ONF_OXM_PACKET_REGS2);
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
    of_wire_buffer_u64_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type onf_oxm_packet_regs2.
 * @param obj Pointer to an object of type onf_oxm_packet_regs2.
 * @param value The value to write into the object
 */
void
onf_oxm_packet_regs2_value_set(
    onf_oxm_packet_regs2_t *obj,
    uint64_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == ONF_OXM_PACKET_REGS2);
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
    of_wire_buffer_u64_set(wbuf, abs_offset, value);

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
onf_oxm_packet_regs3_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0x80010608); /* type_len */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup onf_oxm_packet_regs3 onf_oxm_packet_regs3
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
onf_oxm_packet_regs3_push_wire_values(onf_oxm_packet_regs3_t *obj)
{

    onf_oxm_packet_regs3_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new onf_oxm_packet_regs3 object
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
 * \ingroup onf_oxm_packet_regs3
 */

onf_oxm_packet_regs3_t *
onf_oxm_packet_regs3_new(of_version_t version)
{
    onf_oxm_packet_regs3_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][ONF_OXM_PACKET_REGS3] + of_object_extra_len[version][ONF_OXM_PACKET_REGS3];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (onf_oxm_packet_regs3_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    onf_oxm_packet_regs3_init(obj, version, bytes, 0);

    if (onf_oxm_packet_regs3_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type onf_oxm_packet_regs3.
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
onf_oxm_packet_regs3_init(onf_oxm_packet_regs3_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][ONF_OXM_PACKET_REGS3] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][ONF_OXM_PACKET_REGS3] + of_object_extra_len[version][ONF_OXM_PACKET_REGS3];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = ONF_OXM_PACKET_REGS3;

    /* Set up the object's function pointers */

    obj->wire_type_set = onf_oxm_packet_regs3_push_wire_types;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get value from an object of type onf_oxm_packet_regs3.
 * @param obj Pointer to an object of type onf_oxm_packet_regs3.
 * @param value Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
onf_oxm_packet_regs3_value_get(
    onf_oxm_packet_regs3_t *obj,
    uint64_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == ONF_OXM_PACKET_REGS3);
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
    of_wire_buffer_u64_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type onf_oxm_packet_regs3.
 * @param obj Pointer to an object of type onf_oxm_packet_regs3.
 * @param value The value to write into the object
 */
void
onf_oxm_packet_regs3_value_set(
    onf_oxm_packet_regs3_t *obj,
    uint64_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == ONF_OXM_PACKET_REGS3);
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
    of_wire_buffer_u64_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
