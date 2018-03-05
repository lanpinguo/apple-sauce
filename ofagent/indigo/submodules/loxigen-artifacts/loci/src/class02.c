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
of_experimenter_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 12)); /* experimenter */
        switch (value) {
        case 0x5c16c7:
            of_bsn_stats_request_wire_object_id_get(obj, id);
            break;
        default:
            *id = OF_EXPERIMENTER_STATS_REQUEST;
            break;
        }
        break;
    }
    case OF_VERSION_1_1: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 16)); /* experimenter */
        switch (value) {
        case 0x5c16c7:
            of_bsn_stats_request_wire_object_id_get(obj, id);
            break;
        default:
            *id = OF_EXPERIMENTER_STATS_REQUEST;
            break;
        }
        break;
    }
    case OF_VERSION_1_2: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 16)); /* experimenter */
        switch (value) {
        case 0x5c16c7:
            of_bsn_stats_request_wire_object_id_get(obj, id);
            break;
        default:
            *id = OF_EXPERIMENTER_STATS_REQUEST;
            break;
        }
        break;
    }
    case OF_VERSION_1_3: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 16)); /* experimenter */
        switch (value) {
        case 0x1018:
        /* Added by JiaoYong, 2017/4/6 设备能力上报EXPERIMENTER_ID,跟移动申请的相同*/
        case MULTIPART_EXPERIMENTER_ID:
            of_experimenter_multipart_request_ofdpa_wire_object_id_get(obj, id);
            break;
        case 0x5c16c7:
            of_bsn_stats_request_wire_object_id_get(obj, id);
            break;          
        default:
            *id = OF_EXPERIMENTER_STATS_REQUEST;
            break;
        }
        break;
    }
    default:
        LOCI_ASSERT(0);
    }
}


/**
 * \defgroup of_experimenter_stats_request of_experimenter_stats_request
 */

/**
 * Create a new of_experimenter_stats_request object
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
 * \ingroup of_experimenter_stats_request
 */

of_experimenter_stats_request_t *
of_experimenter_stats_request_new(of_version_t version)
{
    of_experimenter_stats_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_EXPERIMENTER_STATS_REQUEST] + of_object_extra_len[version][OF_EXPERIMENTER_STATS_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_experimenter_stats_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_experimenter_stats_request_init(obj, version, bytes, 0);

    return obj;
}

/**
 * Initialize an object of type of_experimenter_stats_request.
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
of_experimenter_stats_request_init(of_experimenter_stats_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_EXPERIMENTER_STATS_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_EXPERIMENTER_STATS_REQUEST] + of_object_extra_len[version][OF_EXPERIMENTER_STATS_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_EXPERIMENTER_STATS_REQUEST;

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
 * Create a new of_experimenter_stats_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_experimenter_stats_request
 */

of_experimenter_stats_request_t *
of_experimenter_stats_request_new_from_message(of_message_t msg)
{
    of_experimenter_stats_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_experimenter_stats_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_experimenter_stats_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_experimenter_stats_request.
 * @param obj Pointer to an object of type of_experimenter_stats_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_experimenter_stats_request_xid_get(
    of_experimenter_stats_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_EXPERIMENTER_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_experimenter_stats_request.
 * @param obj Pointer to an object of type of_experimenter_stats_request.
 * @param xid The value to write into the object
 */
void
of_experimenter_stats_request_xid_set(
    of_experimenter_stats_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_EXPERIMENTER_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get flags from an object of type of_experimenter_stats_request.
 * @param obj Pointer to an object of type of_experimenter_stats_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_experimenter_stats_request_flags_get(
    of_experimenter_stats_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_EXPERIMENTER_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set flags in an object of type of_experimenter_stats_request.
 * @param obj Pointer to an object of type of_experimenter_stats_request.
 * @param flags The value to write into the object
 */
void
of_experimenter_stats_request_flags_set(
    of_experimenter_stats_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_EXPERIMENTER_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get experimenter from an object of type of_experimenter_stats_request.
 * @param obj Pointer to an object of type of_experimenter_stats_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_experimenter_stats_request_experimenter_get(
    of_experimenter_stats_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_EXPERIMENTER_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 12;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set experimenter in an object of type of_experimenter_stats_request.
 * @param obj Pointer to an object of type of_experimenter_stats_request.
 * @param experimenter The value to write into the object
 */
void
of_experimenter_stats_request_experimenter_set(
    of_experimenter_stats_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_EXPERIMENTER_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 12;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get subtype from an object of type of_experimenter_stats_request.
 * @param obj Pointer to an object of type of_experimenter_stats_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_experimenter_stats_request_subtype_get(
    of_experimenter_stats_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_EXPERIMENTER_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
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
 * Set subtype in an object of type of_experimenter_stats_request.
 * @param obj Pointer to an object of type of_experimenter_stats_request.
 * @param subtype The value to write into the object
 */
void
of_experimenter_stats_request_subtype_set(
    of_experimenter_stats_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_EXPERIMENTER_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
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
 * Get data from an object of type of_experimenter_stats_request.
 * @param obj Pointer to an object of type of_experimenter_stats_request.
 * @param data Pointer to the child object of type
 * of_octets_t to be filled out.
 *
 */
void
of_experimenter_stats_request_data_get(
    of_experimenter_stats_request_t *obj,
    of_octets_t *data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_EXPERIMENTER_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 16;
        cur_len = _END_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
        offset = 24;
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
 * Set data in an object of type of_experimenter_stats_request.
 * @param obj Pointer to an object of type of_experimenter_stats_request.
 * @param data The value to write into the object
 */
int WARN_UNUSED_RESULT
of_experimenter_stats_request_data_set(
    of_experimenter_stats_request_t *obj,
    of_octets_t *data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_EXPERIMENTER_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 16;
        cur_len = _END_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
        offset = 24;
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


/*****************************************************************************
 函 数 名  : of_bsn_stats_request_wire_object_id_get
 功能描述  : Device Feature Info for D-C
 输入参数  : of_object_t *obj    
             of_object_id_t *id  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2017年4月6日
    作    者   : JiaoYong
    修改内容   : 新生成函数

*****************************************************************************/
void
of_dev_feature_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 20)); /* subtype */
        switch (value) {
        default:
            *id = OF_BSN_STATS_REQUEST;
            break;
        }
        break;
    }
    case OF_VERSION_1_1: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 20)); /* subtype */
        switch (value) {
        default:
            *id = OF_BSN_STATS_REQUEST;
            break;
        }
        break;
    }
    case OF_VERSION_1_2: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 20)); /* subtype */
        switch (value) {
        default:
            *id = OF_BSN_STATS_REQUEST;
            break;
        }
        break;
    }
    case OF_VERSION_1_3: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 20)); /* subtype */
        switch (value) {
        case 0x1:
            *id = OF_BSN_LACP_STATS_REQUEST;
            break;
        case 0x2:
            *id = OF_BSN_GENTABLE_ENTRY_DESC_STATS_REQUEST;
            break;
        case 0x3:
            *id = OF_BSN_GENTABLE_ENTRY_STATS_REQUEST;
            break;
        case 0x4:
            *id = OF_BSN_GENTABLE_DESC_STATS_REQUEST;
            break;
        case 0x5:
            *id = OF_BSN_GENTABLE_BUCKET_STATS_REQUEST;
            break;
        case 0x6:
            *id = OF_BSN_SWITCH_PIPELINE_STATS_REQUEST;
            break;
        case 0x7:
            *id = OF_BSN_GENTABLE_STATS_REQUEST;
            break;
        case 0x8:
            *id = OF_BSN_PORT_COUNTER_STATS_REQUEST;
            break;
        case 0x9:
            *id = OF_BSN_VLAN_COUNTER_STATS_REQUEST;
            break;
        case 0xa:
            *id = OF_BSN_FLOW_CHECKSUM_BUCKET_STATS_REQUEST;
            break;
        case 0xb:
            *id = OF_BSN_TABLE_CHECKSUM_STATS_REQUEST;
            break;
        case 0xc:
            *id = OF_BSN_DEBUG_COUNTER_STATS_REQUEST;
            break;
        case 0xd:
            *id = OF_BSN_DEBUG_COUNTER_DESC_STATS_REQUEST;
            break;
        default:
            *id = OF_BSN_STATS_REQUEST;
            break;
        }
        break;
    }
    default:
        LOCI_ASSERT(0);
    }
}



void
of_bsn_stats_request_wire_object_id_get(of_object_t *obj, of_object_id_t *id)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 20)); /* subtype */
        switch (value) {
        default:
            *id = OF_BSN_STATS_REQUEST;
            break;
        }
        break;
    }
    case OF_VERSION_1_1: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 20)); /* subtype */
        switch (value) {
        default:
            *id = OF_BSN_STATS_REQUEST;
            break;
        }
        break;
    }
    case OF_VERSION_1_2: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 20)); /* subtype */
        switch (value) {
        default:
            *id = OF_BSN_STATS_REQUEST;
            break;
        }
        break;
    }
    case OF_VERSION_1_3: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 20)); /* subtype */
        switch (value) {
        case 0x1:
            *id = OF_BSN_LACP_STATS_REQUEST;
            break;
        case 0x2:
            *id = OF_BSN_GENTABLE_ENTRY_DESC_STATS_REQUEST;
            break;
        case 0x3:
            *id = OF_BSN_GENTABLE_ENTRY_STATS_REQUEST;
            break;
        case 0x4:
            *id = OF_BSN_GENTABLE_DESC_STATS_REQUEST;
            break;
        case 0x5:
            *id = OF_BSN_GENTABLE_BUCKET_STATS_REQUEST;
            break;
        case 0x6:
            *id = OF_BSN_SWITCH_PIPELINE_STATS_REQUEST;
            break;
        case 0x7:
            *id = OF_BSN_GENTABLE_STATS_REQUEST;
            break;
        case 0x8:
            *id = OF_BSN_PORT_COUNTER_STATS_REQUEST;
            break;
        case 0x9:
            *id = OF_BSN_VLAN_COUNTER_STATS_REQUEST;
            break;
        case 0xa:
            *id = OF_BSN_FLOW_CHECKSUM_BUCKET_STATS_REQUEST;
            break;
        case 0xb:
            *id = OF_BSN_TABLE_CHECKSUM_STATS_REQUEST;
            break;
        case 0xc:
            *id = OF_BSN_DEBUG_COUNTER_STATS_REQUEST;
            break;
        case 0xd:
            *id = OF_BSN_DEBUG_COUNTER_DESC_STATS_REQUEST;
            break;
        default:
            *id = OF_BSN_STATS_REQUEST;
            break;
        }
        break;
    }
    default:
        LOCI_ASSERT(0);
    }
}


/**
 * \defgroup of_bsn_stats_request of_bsn_stats_request
 */

/**
 * Create a new of_bsn_stats_request object
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
 * \ingroup of_bsn_stats_request
 */

of_bsn_stats_request_t *
of_bsn_stats_request_new(of_version_t version)
{
    of_bsn_stats_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_BSN_STATS_REQUEST] + of_object_extra_len[version][OF_BSN_STATS_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_bsn_stats_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_bsn_stats_request_init(obj, version, bytes, 0);

    return obj;
}

/**
 * Initialize an object of type of_bsn_stats_request.
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
of_bsn_stats_request_init(of_bsn_stats_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_BSN_STATS_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_BSN_STATS_REQUEST] + of_object_extra_len[version][OF_BSN_STATS_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_BSN_STATS_REQUEST;

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
 * Create a new of_bsn_stats_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_bsn_stats_request
 */

of_bsn_stats_request_t *
of_bsn_stats_request_new_from_message(of_message_t msg)
{
    of_bsn_stats_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_bsn_stats_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_bsn_stats_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_bsn_stats_request.
 * @param obj Pointer to an object of type of_bsn_stats_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_stats_request_xid_get(
    of_bsn_stats_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_bsn_stats_request.
 * @param obj Pointer to an object of type of_bsn_stats_request.
 * @param xid The value to write into the object
 */
void
of_bsn_stats_request_xid_set(
    of_bsn_stats_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get flags from an object of type of_bsn_stats_request.
 * @param obj Pointer to an object of type of_bsn_stats_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_bsn_stats_request_flags_get(
    of_bsn_stats_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set flags in an object of type of_bsn_stats_request.
 * @param obj Pointer to an object of type of_bsn_stats_request.
 * @param flags The value to write into the object
 */
void
of_bsn_stats_request_flags_set(
    of_bsn_stats_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get experimenter from an object of type of_bsn_stats_request.
 * @param obj Pointer to an object of type of_bsn_stats_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_stats_request_experimenter_get(
    of_bsn_stats_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set experimenter in an object of type of_bsn_stats_request.
 * @param obj Pointer to an object of type of_bsn_stats_request.
 * @param experimenter The value to write into the object
 */
void
of_bsn_stats_request_experimenter_set(
    of_bsn_stats_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get subtype from an object of type of_bsn_stats_request.
 * @param obj Pointer to an object of type of_bsn_stats_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_stats_request_subtype_get(
    of_bsn_stats_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set subtype in an object of type of_bsn_stats_request.
 * @param obj Pointer to an object of type of_bsn_stats_request.
 * @param subtype The value to write into the object
 */
void
of_bsn_stats_request_subtype_set(
    of_bsn_stats_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
of_bsn_virtual_port_create_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x5c16c7); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0x10); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_bsn_virtual_port_create_reply of_bsn_virtual_port_create_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_bsn_virtual_port_create_reply_push_wire_values(of_bsn_virtual_port_create_reply_t *obj)
{

    of_bsn_virtual_port_create_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_bsn_virtual_port_create_reply object
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
 * \ingroup of_bsn_virtual_port_create_reply
 */

of_bsn_virtual_port_create_reply_t *
of_bsn_virtual_port_create_reply_new(of_version_t version)
{
    of_bsn_virtual_port_create_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_BSN_VIRTUAL_PORT_CREATE_REPLY] + of_object_extra_len[version][OF_BSN_VIRTUAL_PORT_CREATE_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_bsn_virtual_port_create_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_bsn_virtual_port_create_reply_init(obj, version, bytes, 0);

    if (of_bsn_virtual_port_create_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_bsn_virtual_port_create_reply.
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
of_bsn_virtual_port_create_reply_init(of_bsn_virtual_port_create_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_BSN_VIRTUAL_PORT_CREATE_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_BSN_VIRTUAL_PORT_CREATE_REPLY] + of_object_extra_len[version][OF_BSN_VIRTUAL_PORT_CREATE_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_BSN_VIRTUAL_PORT_CREATE_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_bsn_virtual_port_create_reply_push_wire_types;

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
 * Create a new of_bsn_virtual_port_create_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_bsn_virtual_port_create_reply
 */

of_bsn_virtual_port_create_reply_t *
of_bsn_virtual_port_create_reply_new_from_message(of_message_t msg)
{
    of_bsn_virtual_port_create_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_bsn_virtual_port_create_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_bsn_virtual_port_create_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_bsn_virtual_port_create_reply.
 * @param obj Pointer to an object of type of_bsn_virtual_port_create_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_virtual_port_create_reply_xid_get(
    of_bsn_virtual_port_create_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_CREATE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_bsn_virtual_port_create_reply.
 * @param obj Pointer to an object of type of_bsn_virtual_port_create_reply.
 * @param xid The value to write into the object
 */
void
of_bsn_virtual_port_create_reply_xid_set(
    of_bsn_virtual_port_create_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_CREATE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get experimenter from an object of type of_bsn_virtual_port_create_reply.
 * @param obj Pointer to an object of type of_bsn_virtual_port_create_reply.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_virtual_port_create_reply_experimenter_get(
    of_bsn_virtual_port_create_reply_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_CREATE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set experimenter in an object of type of_bsn_virtual_port_create_reply.
 * @param obj Pointer to an object of type of_bsn_virtual_port_create_reply.
 * @param experimenter The value to write into the object
 */
void
of_bsn_virtual_port_create_reply_experimenter_set(
    of_bsn_virtual_port_create_reply_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_CREATE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get subtype from an object of type of_bsn_virtual_port_create_reply.
 * @param obj Pointer to an object of type of_bsn_virtual_port_create_reply.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_virtual_port_create_reply_subtype_get(
    of_bsn_virtual_port_create_reply_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_CREATE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set subtype in an object of type of_bsn_virtual_port_create_reply.
 * @param obj Pointer to an object of type of_bsn_virtual_port_create_reply.
 * @param subtype The value to write into the object
 */
void
of_bsn_virtual_port_create_reply_subtype_set(
    of_bsn_virtual_port_create_reply_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_CREATE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get status from an object of type of_bsn_virtual_port_create_reply.
 * @param obj Pointer to an object of type of_bsn_virtual_port_create_reply.
 * @param status Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_virtual_port_create_reply_status_get(
    of_bsn_virtual_port_create_reply_t *obj,
    uint32_t *status)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_CREATE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, status);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set status in an object of type of_bsn_virtual_port_create_reply.
 * @param obj Pointer to an object of type of_bsn_virtual_port_create_reply.
 * @param status The value to write into the object
 */
void
of_bsn_virtual_port_create_reply_status_set(
    of_bsn_virtual_port_create_reply_t *obj,
    uint32_t status)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_CREATE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, status);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vport_no from an object of type of_bsn_virtual_port_create_reply.
 * @param obj Pointer to an object of type of_bsn_virtual_port_create_reply.
 * @param vport_no Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_virtual_port_create_reply_vport_no_get(
    of_bsn_virtual_port_create_reply_t *obj,
    uint32_t *vport_no)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_CREATE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, vport_no);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vport_no in an object of type of_bsn_virtual_port_create_reply.
 * @param obj Pointer to an object of type of_bsn_virtual_port_create_reply.
 * @param vport_no The value to write into the object
 */
void
of_bsn_virtual_port_create_reply_vport_no_set(
    of_bsn_virtual_port_create_reply_t *obj,
    uint32_t vport_no)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_CREATE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, vport_no);

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
of_bsn_vport_wire_object_id_get(of_object_t *obj, of_object_id_t *id)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0: {
        uint16_t value = U16_NTOH(*(uint16_t *)(buf + 0)); /* type */
        switch (value) {
        case 0x0:
            *id = OF_BSN_VPORT_Q_IN_Q;
            break;
        default:
            *id = OF_BSN_VPORT;
            break;
        }
        break;
    }
    case OF_VERSION_1_1: {
        uint16_t value = U16_NTOH(*(uint16_t *)(buf + 0)); /* type */
        switch (value) {
        case 0x0:
            *id = OF_BSN_VPORT_Q_IN_Q;
            break;
        default:
            *id = OF_BSN_VPORT;
            break;
        }
        break;
    }
    case OF_VERSION_1_2: {
        uint16_t value = U16_NTOH(*(uint16_t *)(buf + 0)); /* type */
        switch (value) {
        case 0x0:
            *id = OF_BSN_VPORT_Q_IN_Q;
            break;
        default:
            *id = OF_BSN_VPORT;
            break;
        }
        break;
    }
    case OF_VERSION_1_3: {
        uint16_t value = U16_NTOH(*(uint16_t *)(buf + 0)); /* type */
        switch (value) {
        case 0x0:
            *id = OF_BSN_VPORT_Q_IN_Q;
            break;
        default:
            *id = OF_BSN_VPORT;
            break;
        }
        break;
    }
    default:
        LOCI_ASSERT(0);
    }
}


/**
 * \defgroup of_bsn_vport of_bsn_vport
 */

/**
 * Create a new of_bsn_vport object
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
 * \ingroup of_bsn_vport
 */

of_bsn_vport_t *
of_bsn_vport_new(of_version_t version)
{
    of_bsn_vport_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_BSN_VPORT] + of_object_extra_len[version][OF_BSN_VPORT];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_bsn_vport_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_bsn_vport_init(obj, version, bytes, 0);

    return obj;
}

/**
 * Initialize an object of type of_bsn_vport.
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
of_bsn_vport_init(of_bsn_vport_t *obj_p,
    of_version_t version, int bytes, int clean_wire)
{
    of_bsn_vport_header_t *obj;

    obj = &obj_p->header;  /* Need instantiable subclass */

    LOCI_ASSERT(of_object_fixed_len[version][OF_BSN_VPORT] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_BSN_VPORT] + of_object_extra_len[version][OF_BSN_VPORT];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_BSN_VPORT;

    /* Set up the object's function pointers */

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
of_bsn_vport_q_in_q_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint16_t *)(buf + 0) = U16_HTON(0x0); /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_bsn_vport_q_in_q of_bsn_vport_q_in_q
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_bsn_vport_q_in_q_push_wire_values(of_bsn_vport_q_in_q_t *obj)
{

    of_bsn_vport_q_in_q_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_bsn_vport_q_in_q object
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
 * \ingroup of_bsn_vport_q_in_q
 */

of_bsn_vport_q_in_q_t *
of_bsn_vport_q_in_q_new(of_version_t version)
{
    of_bsn_vport_q_in_q_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_BSN_VPORT_Q_IN_Q] + of_object_extra_len[version][OF_BSN_VPORT_Q_IN_Q];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_bsn_vport_q_in_q_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_bsn_vport_q_in_q_init(obj, version, bytes, 0);

    if (of_bsn_vport_q_in_q_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_bsn_vport_q_in_q.
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
of_bsn_vport_q_in_q_init(of_bsn_vport_q_in_q_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_BSN_VPORT_Q_IN_Q] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_BSN_VPORT_Q_IN_Q] + of_object_extra_len[version][OF_BSN_VPORT_Q_IN_Q];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_BSN_VPORT_Q_IN_Q;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_bsn_vport_q_in_q_push_wire_types;

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get port_no from an object of type of_bsn_vport_q_in_q.
 * @param obj Pointer to an object of type of_bsn_vport_q_in_q.
 * @param port_no Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_vport_q_in_q_port_no_get(
    of_bsn_vport_q_in_q_t *obj,
    uint32_t *port_no)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VPORT_Q_IN_Q);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
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
 * Set port_no in an object of type of_bsn_vport_q_in_q.
 * @param obj Pointer to an object of type of_bsn_vport_q_in_q.
 * @param port_no The value to write into the object
 */
void
of_bsn_vport_q_in_q_port_no_set(
    of_bsn_vport_q_in_q_t *obj,
    uint32_t port_no)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VPORT_Q_IN_Q);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
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
 * Get ingress_tpid from an object of type of_bsn_vport_q_in_q.
 * @param obj Pointer to an object of type of_bsn_vport_q_in_q.
 * @param ingress_tpid Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_bsn_vport_q_in_q_ingress_tpid_get(
    of_bsn_vport_q_in_q_t *obj,
    uint16_t *ingress_tpid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VPORT_Q_IN_Q);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, ingress_tpid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set ingress_tpid in an object of type of_bsn_vport_q_in_q.
 * @param obj Pointer to an object of type of_bsn_vport_q_in_q.
 * @param ingress_tpid The value to write into the object
 */
void
of_bsn_vport_q_in_q_ingress_tpid_set(
    of_bsn_vport_q_in_q_t *obj,
    uint16_t ingress_tpid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VPORT_Q_IN_Q);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, ingress_tpid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get ingress_vlan_id from an object of type of_bsn_vport_q_in_q.
 * @param obj Pointer to an object of type of_bsn_vport_q_in_q.
 * @param ingress_vlan_id Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_bsn_vport_q_in_q_ingress_vlan_id_get(
    of_bsn_vport_q_in_q_t *obj,
    uint16_t *ingress_vlan_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VPORT_Q_IN_Q);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, ingress_vlan_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set ingress_vlan_id in an object of type of_bsn_vport_q_in_q.
 * @param obj Pointer to an object of type of_bsn_vport_q_in_q.
 * @param ingress_vlan_id The value to write into the object
 */
void
of_bsn_vport_q_in_q_ingress_vlan_id_set(
    of_bsn_vport_q_in_q_t *obj,
    uint16_t ingress_vlan_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VPORT_Q_IN_Q);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, ingress_vlan_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get egress_tpid from an object of type of_bsn_vport_q_in_q.
 * @param obj Pointer to an object of type of_bsn_vport_q_in_q.
 * @param egress_tpid Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_bsn_vport_q_in_q_egress_tpid_get(
    of_bsn_vport_q_in_q_t *obj,
    uint16_t *egress_tpid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VPORT_Q_IN_Q);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, egress_tpid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set egress_tpid in an object of type of_bsn_vport_q_in_q.
 * @param obj Pointer to an object of type of_bsn_vport_q_in_q.
 * @param egress_tpid The value to write into the object
 */
void
of_bsn_vport_q_in_q_egress_tpid_set(
    of_bsn_vport_q_in_q_t *obj,
    uint16_t egress_tpid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VPORT_Q_IN_Q);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, egress_tpid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get egress_vlan_id from an object of type of_bsn_vport_q_in_q.
 * @param obj Pointer to an object of type of_bsn_vport_q_in_q.
 * @param egress_vlan_id Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_bsn_vport_q_in_q_egress_vlan_id_get(
    of_bsn_vport_q_in_q_t *obj,
    uint16_t *egress_vlan_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VPORT_Q_IN_Q);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 14;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, egress_vlan_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set egress_vlan_id in an object of type of_bsn_vport_q_in_q.
 * @param obj Pointer to an object of type of_bsn_vport_q_in_q.
 * @param egress_vlan_id The value to write into the object
 */
void
of_bsn_vport_q_in_q_egress_vlan_id_set(
    of_bsn_vport_q_in_q_t *obj,
    uint16_t egress_vlan_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VPORT_Q_IN_Q);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 14;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, egress_vlan_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get if_name from an object of type of_bsn_vport_q_in_q.
 * @param obj Pointer to an object of type of_bsn_vport_q_in_q.
 * @param if_name Pointer to the child object of type
 * of_port_name_t to be filled out.
 *
 */
void
of_bsn_vport_q_in_q_if_name_get(
    of_bsn_vport_q_in_q_t *obj,
    of_port_name_t *if_name)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VPORT_Q_IN_Q);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_port_name_get(wbuf, abs_offset, if_name);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set if_name in an object of type of_bsn_vport_q_in_q.
 * @param obj Pointer to an object of type of_bsn_vport_q_in_q.
 * @param if_name The value to write into the object
 */
void
of_bsn_vport_q_in_q_if_name_set(
    of_bsn_vport_q_in_q_t *obj,
    of_port_name_t if_name)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VPORT_Q_IN_Q);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_port_name_set(wbuf, abs_offset, if_name);

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
of_bsn_virtual_port_create_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x5c16c7); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0xf); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_bsn_virtual_port_create_request of_bsn_virtual_port_create_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_bsn_virtual_port_create_request_push_wire_values(of_bsn_virtual_port_create_request_t *obj)
{

    of_bsn_virtual_port_create_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_bsn_virtual_port_create_request object
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
 * \ingroup of_bsn_virtual_port_create_request
 */

of_bsn_virtual_port_create_request_t *
of_bsn_virtual_port_create_request_new(of_version_t version)
{
    of_bsn_virtual_port_create_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_BSN_VIRTUAL_PORT_CREATE_REQUEST] + of_object_extra_len[version][OF_BSN_VIRTUAL_PORT_CREATE_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_bsn_virtual_port_create_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_bsn_virtual_port_create_request_init(obj, version, bytes, 0);

    if (of_bsn_virtual_port_create_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_bsn_virtual_port_create_request.
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
of_bsn_virtual_port_create_request_init(of_bsn_virtual_port_create_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_BSN_VIRTUAL_PORT_CREATE_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_BSN_VIRTUAL_PORT_CREATE_REQUEST] + of_object_extra_len[version][OF_BSN_VIRTUAL_PORT_CREATE_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_BSN_VIRTUAL_PORT_CREATE_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_bsn_virtual_port_create_request_push_wire_types;

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
 * Create a new of_bsn_virtual_port_create_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_bsn_virtual_port_create_request
 */

of_bsn_virtual_port_create_request_t *
of_bsn_virtual_port_create_request_new_from_message(of_message_t msg)
{
    of_bsn_virtual_port_create_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_bsn_virtual_port_create_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_bsn_virtual_port_create_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_bsn_virtual_port_create_request.
 * @param obj Pointer to an object of type of_bsn_virtual_port_create_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_virtual_port_create_request_xid_get(
    of_bsn_virtual_port_create_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_CREATE_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_bsn_virtual_port_create_request.
 * @param obj Pointer to an object of type of_bsn_virtual_port_create_request.
 * @param xid The value to write into the object
 */
void
of_bsn_virtual_port_create_request_xid_set(
    of_bsn_virtual_port_create_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_CREATE_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get experimenter from an object of type of_bsn_virtual_port_create_request.
 * @param obj Pointer to an object of type of_bsn_virtual_port_create_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_virtual_port_create_request_experimenter_get(
    of_bsn_virtual_port_create_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_CREATE_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set experimenter in an object of type of_bsn_virtual_port_create_request.
 * @param obj Pointer to an object of type of_bsn_virtual_port_create_request.
 * @param experimenter The value to write into the object
 */
void
of_bsn_virtual_port_create_request_experimenter_set(
    of_bsn_virtual_port_create_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_CREATE_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get subtype from an object of type of_bsn_virtual_port_create_request.
 * @param obj Pointer to an object of type of_bsn_virtual_port_create_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_virtual_port_create_request_subtype_get(
    of_bsn_virtual_port_create_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_CREATE_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set subtype in an object of type of_bsn_virtual_port_create_request.
 * @param obj Pointer to an object of type of_bsn_virtual_port_create_request.
 * @param subtype The value to write into the object
 */
void
of_bsn_virtual_port_create_request_subtype_set(
    of_bsn_virtual_port_create_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_CREATE_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Bind an object of type of_bsn_vport_q_in_q_t to the parent of type of_bsn_virtual_port_create_request for
 * member vport
 * @param obj Pointer to an object of type of_bsn_virtual_port_create_request.
 * @param vport Pointer to the child object of type
 * of_bsn_vport_q_in_q_t to be filled out.
 * \ingroup of_bsn_virtual_port_create_request
 *
 * The parameter vport is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_bsn_virtual_port_create_request_vport_bind(
    of_bsn_virtual_port_create_request_t *obj,
    of_bsn_vport_q_in_q_t *vport)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_CREATE_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        cur_len = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_bsn_vport_q_in_q_init(vport, obj->version, 0, 1);
    /* Attach to parent */
    vport->parent = (of_object_t *)obj;
    vport->wire_object.wbuf = obj->wire_object.wbuf;
    vport->wire_object.obj_offset = abs_offset;
    vport->wire_object.owned = 0;
    vport->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of vport into a new variable of type of_bsn_vport_q_in_q_t from
 * a of_bsn_virtual_port_create_request instance.
 *
 * @param obj Pointer to the source of type of_bsn_virtual_port_create_request_t
 * @returns A pointer to a new instance of type of_bsn_vport_q_in_q_t whose contents
 * match that of vport from source
 * @returns NULL if an error occurs
 */
of_bsn_vport_q_in_q_t *
of_bsn_virtual_port_create_request_vport_get(of_bsn_virtual_port_create_request_t *obj) {
    of_bsn_vport_q_in_q_t _vport;
    of_bsn_vport_q_in_q_t *_vport_ptr;

    of_bsn_virtual_port_create_request_vport_bind(obj, &_vport);
    _vport_ptr = (of_bsn_vport_q_in_q_t *)of_object_dup(&_vport);
    return _vport_ptr;
}

/**
 * Set vport in an object of type of_bsn_virtual_port_create_request.
 * @param obj Pointer to an object of type of_bsn_virtual_port_create_request.
 * @param vport Pointer to the child of type of_bsn_vport_q_in_q_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
void
of_bsn_virtual_port_create_request_vport_set(
    of_bsn_virtual_port_create_request_t *obj,
    of_bsn_vport_q_in_q_t *vport)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_CREATE_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        cur_len = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = vport->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == vport->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(vport, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return ;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(vport, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (vport->wire_length_set != NULL) {
        vport->wire_length_set((of_object_t *)vport, vport->length);
    }

    /* Not scalar, update lengths if needed */
    delta = new_len - cur_len;
    if (delta != 0) {
        /* Update parent(s) */
        of_object_parent_length_update((of_object_t *)obj, delta);
    }

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
of_bsn_virtual_port_remove_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x5c16c7); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0x1a); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_bsn_virtual_port_remove_reply of_bsn_virtual_port_remove_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_bsn_virtual_port_remove_reply_push_wire_values(of_bsn_virtual_port_remove_reply_t *obj)
{

    of_bsn_virtual_port_remove_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_bsn_virtual_port_remove_reply object
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
 * \ingroup of_bsn_virtual_port_remove_reply
 */

of_bsn_virtual_port_remove_reply_t *
of_bsn_virtual_port_remove_reply_new(of_version_t version)
{
    of_bsn_virtual_port_remove_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_BSN_VIRTUAL_PORT_REMOVE_REPLY] + of_object_extra_len[version][OF_BSN_VIRTUAL_PORT_REMOVE_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_bsn_virtual_port_remove_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_bsn_virtual_port_remove_reply_init(obj, version, bytes, 0);

    if (of_bsn_virtual_port_remove_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_bsn_virtual_port_remove_reply.
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
of_bsn_virtual_port_remove_reply_init(of_bsn_virtual_port_remove_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_BSN_VIRTUAL_PORT_REMOVE_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_BSN_VIRTUAL_PORT_REMOVE_REPLY] + of_object_extra_len[version][OF_BSN_VIRTUAL_PORT_REMOVE_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_BSN_VIRTUAL_PORT_REMOVE_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_bsn_virtual_port_remove_reply_push_wire_types;

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
 * Create a new of_bsn_virtual_port_remove_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_bsn_virtual_port_remove_reply
 */

of_bsn_virtual_port_remove_reply_t *
of_bsn_virtual_port_remove_reply_new_from_message(of_message_t msg)
{
    of_bsn_virtual_port_remove_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_bsn_virtual_port_remove_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_bsn_virtual_port_remove_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_bsn_virtual_port_remove_reply.
 * @param obj Pointer to an object of type of_bsn_virtual_port_remove_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_virtual_port_remove_reply_xid_get(
    of_bsn_virtual_port_remove_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_REMOVE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_bsn_virtual_port_remove_reply.
 * @param obj Pointer to an object of type of_bsn_virtual_port_remove_reply.
 * @param xid The value to write into the object
 */
void
of_bsn_virtual_port_remove_reply_xid_set(
    of_bsn_virtual_port_remove_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_REMOVE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get experimenter from an object of type of_bsn_virtual_port_remove_reply.
 * @param obj Pointer to an object of type of_bsn_virtual_port_remove_reply.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_virtual_port_remove_reply_experimenter_get(
    of_bsn_virtual_port_remove_reply_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_REMOVE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set experimenter in an object of type of_bsn_virtual_port_remove_reply.
 * @param obj Pointer to an object of type of_bsn_virtual_port_remove_reply.
 * @param experimenter The value to write into the object
 */
void
of_bsn_virtual_port_remove_reply_experimenter_set(
    of_bsn_virtual_port_remove_reply_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_REMOVE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get subtype from an object of type of_bsn_virtual_port_remove_reply.
 * @param obj Pointer to an object of type of_bsn_virtual_port_remove_reply.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_virtual_port_remove_reply_subtype_get(
    of_bsn_virtual_port_remove_reply_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_REMOVE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set subtype in an object of type of_bsn_virtual_port_remove_reply.
 * @param obj Pointer to an object of type of_bsn_virtual_port_remove_reply.
 * @param subtype The value to write into the object
 */
void
of_bsn_virtual_port_remove_reply_subtype_set(
    of_bsn_virtual_port_remove_reply_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_REMOVE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get status from an object of type of_bsn_virtual_port_remove_reply.
 * @param obj Pointer to an object of type of_bsn_virtual_port_remove_reply.
 * @param status Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_virtual_port_remove_reply_status_get(
    of_bsn_virtual_port_remove_reply_t *obj,
    uint32_t *status)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_REMOVE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, status);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set status in an object of type of_bsn_virtual_port_remove_reply.
 * @param obj Pointer to an object of type of_bsn_virtual_port_remove_reply.
 * @param status The value to write into the object
 */
void
of_bsn_virtual_port_remove_reply_status_set(
    of_bsn_virtual_port_remove_reply_t *obj,
    uint32_t status)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_REMOVE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, status);

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
of_bsn_virtual_port_remove_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x5c16c7); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0x11); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_bsn_virtual_port_remove_request of_bsn_virtual_port_remove_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_bsn_virtual_port_remove_request_push_wire_values(of_bsn_virtual_port_remove_request_t *obj)
{

    of_bsn_virtual_port_remove_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_bsn_virtual_port_remove_request object
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
 * \ingroup of_bsn_virtual_port_remove_request
 */

of_bsn_virtual_port_remove_request_t *
of_bsn_virtual_port_remove_request_new(of_version_t version)
{
    of_bsn_virtual_port_remove_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_BSN_VIRTUAL_PORT_REMOVE_REQUEST] + of_object_extra_len[version][OF_BSN_VIRTUAL_PORT_REMOVE_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_bsn_virtual_port_remove_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_bsn_virtual_port_remove_request_init(obj, version, bytes, 0);

    if (of_bsn_virtual_port_remove_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_bsn_virtual_port_remove_request.
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
of_bsn_virtual_port_remove_request_init(of_bsn_virtual_port_remove_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_BSN_VIRTUAL_PORT_REMOVE_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_BSN_VIRTUAL_PORT_REMOVE_REQUEST] + of_object_extra_len[version][OF_BSN_VIRTUAL_PORT_REMOVE_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_BSN_VIRTUAL_PORT_REMOVE_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_bsn_virtual_port_remove_request_push_wire_types;

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
 * Create a new of_bsn_virtual_port_remove_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_bsn_virtual_port_remove_request
 */

of_bsn_virtual_port_remove_request_t *
of_bsn_virtual_port_remove_request_new_from_message(of_message_t msg)
{
    of_bsn_virtual_port_remove_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_bsn_virtual_port_remove_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_bsn_virtual_port_remove_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_bsn_virtual_port_remove_request.
 * @param obj Pointer to an object of type of_bsn_virtual_port_remove_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_virtual_port_remove_request_xid_get(
    of_bsn_virtual_port_remove_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_REMOVE_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_bsn_virtual_port_remove_request.
 * @param obj Pointer to an object of type of_bsn_virtual_port_remove_request.
 * @param xid The value to write into the object
 */
void
of_bsn_virtual_port_remove_request_xid_set(
    of_bsn_virtual_port_remove_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_REMOVE_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get experimenter from an object of type of_bsn_virtual_port_remove_request.
 * @param obj Pointer to an object of type of_bsn_virtual_port_remove_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_virtual_port_remove_request_experimenter_get(
    of_bsn_virtual_port_remove_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_REMOVE_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set experimenter in an object of type of_bsn_virtual_port_remove_request.
 * @param obj Pointer to an object of type of_bsn_virtual_port_remove_request.
 * @param experimenter The value to write into the object
 */
void
of_bsn_virtual_port_remove_request_experimenter_set(
    of_bsn_virtual_port_remove_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_REMOVE_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get subtype from an object of type of_bsn_virtual_port_remove_request.
 * @param obj Pointer to an object of type of_bsn_virtual_port_remove_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_virtual_port_remove_request_subtype_get(
    of_bsn_virtual_port_remove_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_REMOVE_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set subtype in an object of type of_bsn_virtual_port_remove_request.
 * @param obj Pointer to an object of type of_bsn_virtual_port_remove_request.
 * @param subtype The value to write into the object
 */
void
of_bsn_virtual_port_remove_request_subtype_set(
    of_bsn_virtual_port_remove_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_REMOVE_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get vport_no from an object of type of_bsn_virtual_port_remove_request.
 * @param obj Pointer to an object of type of_bsn_virtual_port_remove_request.
 * @param vport_no Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_bsn_virtual_port_remove_request_vport_no_get(
    of_bsn_virtual_port_remove_request_t *obj,
    uint32_t *vport_no)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_REMOVE_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, vport_no);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vport_no in an object of type of_bsn_virtual_port_remove_request.
 * @param obj Pointer to an object of type of_bsn_virtual_port_remove_request.
 * @param vport_no The value to write into the object
 */
void
of_bsn_virtual_port_remove_request_vport_no_set(
    of_bsn_virtual_port_remove_request_t *obj,
    uint32_t vport_no)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_BSN_VIRTUAL_PORT_REMOVE_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, vport_no);

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
of_desc_stats_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x11; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0x0); /* stats_type */
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0x0); /* stats_type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_desc_stats_reply of_desc_stats_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_desc_stats_reply_push_wire_values(of_desc_stats_reply_t *obj)
{

    of_desc_stats_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_desc_stats_reply object
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
 * \ingroup of_desc_stats_reply
 */

of_desc_stats_reply_t *
of_desc_stats_reply_new(of_version_t version)
{
    of_desc_stats_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_DESC_STATS_REPLY] + of_object_extra_len[version][OF_DESC_STATS_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_desc_stats_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_desc_stats_reply_init(obj, version, bytes, 0);

    if (of_desc_stats_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_desc_stats_reply.
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
of_desc_stats_reply_init(of_desc_stats_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_DESC_STATS_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_DESC_STATS_REPLY] + of_object_extra_len[version][OF_DESC_STATS_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_DESC_STATS_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_desc_stats_reply_push_wire_types;

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
 * Create a new of_desc_stats_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_desc_stats_reply
 */

of_desc_stats_reply_t *
of_desc_stats_reply_new_from_message(of_message_t msg)
{
    of_desc_stats_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_desc_stats_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_desc_stats_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_desc_stats_reply.
 * @param obj Pointer to an object of type of_desc_stats_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_desc_stats_reply_xid_get(
    of_desc_stats_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_DESC_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_desc_stats_reply.
 * @param obj Pointer to an object of type of_desc_stats_reply.
 * @param xid The value to write into the object
 */
void
of_desc_stats_reply_xid_set(
    of_desc_stats_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_DESC_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get flags from an object of type of_desc_stats_reply.
 * @param obj Pointer to an object of type of_desc_stats_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_desc_stats_reply_flags_get(
    of_desc_stats_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_DESC_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set flags in an object of type of_desc_stats_reply.
 * @param obj Pointer to an object of type of_desc_stats_reply.
 * @param flags The value to write into the object
 */
void
of_desc_stats_reply_flags_set(
    of_desc_stats_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_DESC_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get mfr_desc from an object of type of_desc_stats_reply.
 * @param obj Pointer to an object of type of_desc_stats_reply.
 * @param mfr_desc Pointer to the child object of type
 * of_desc_str_t to be filled out.
 *
 */
void
of_desc_stats_reply_mfr_desc_get(
    of_desc_stats_reply_t *obj,
    of_desc_str_t *mfr_desc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_DESC_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 12;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_desc_str_get(wbuf, abs_offset, mfr_desc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mfr_desc in an object of type of_desc_stats_reply.
 * @param obj Pointer to an object of type of_desc_stats_reply.
 * @param mfr_desc The value to write into the object
 */
void
of_desc_stats_reply_mfr_desc_set(
    of_desc_stats_reply_t *obj,
    of_desc_str_t mfr_desc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_DESC_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 12;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_desc_str_set(wbuf, abs_offset, mfr_desc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get hw_desc from an object of type of_desc_stats_reply.
 * @param obj Pointer to an object of type of_desc_stats_reply.
 * @param hw_desc Pointer to the child object of type
 * of_desc_str_t to be filled out.
 *
 */
void
of_desc_stats_reply_hw_desc_get(
    of_desc_stats_reply_t *obj,
    of_desc_str_t *hw_desc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_DESC_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 268;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 272;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_desc_str_get(wbuf, abs_offset, hw_desc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set hw_desc in an object of type of_desc_stats_reply.
 * @param obj Pointer to an object of type of_desc_stats_reply.
 * @param hw_desc The value to write into the object
 */
void
of_desc_stats_reply_hw_desc_set(
    of_desc_stats_reply_t *obj,
    of_desc_str_t hw_desc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_DESC_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 268;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 272;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_desc_str_set(wbuf, abs_offset, hw_desc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get sw_desc from an object of type of_desc_stats_reply.
 * @param obj Pointer to an object of type of_desc_stats_reply.
 * @param sw_desc Pointer to the child object of type
 * of_desc_str_t to be filled out.
 *
 */
void
of_desc_stats_reply_sw_desc_get(
    of_desc_stats_reply_t *obj,
    of_desc_str_t *sw_desc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_DESC_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 524;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 528;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_desc_str_get(wbuf, abs_offset, sw_desc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set sw_desc in an object of type of_desc_stats_reply.
 * @param obj Pointer to an object of type of_desc_stats_reply.
 * @param sw_desc The value to write into the object
 */
void
of_desc_stats_reply_sw_desc_set(
    of_desc_stats_reply_t *obj,
    of_desc_str_t sw_desc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_DESC_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 524;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 528;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_desc_str_set(wbuf, abs_offset, sw_desc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get serial_num from an object of type of_desc_stats_reply.
 * @param obj Pointer to an object of type of_desc_stats_reply.
 * @param serial_num Pointer to the child object of type
 * of_serial_num_t to be filled out.
 *
 */
void
of_desc_stats_reply_serial_num_get(
    of_desc_stats_reply_t *obj,
    of_serial_num_t *serial_num)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_DESC_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 780;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 784;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_ser_num_get(wbuf, abs_offset, serial_num);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set serial_num in an object of type of_desc_stats_reply.
 * @param obj Pointer to an object of type of_desc_stats_reply.
 * @param serial_num The value to write into the object
 */
void
of_desc_stats_reply_serial_num_set(
    of_desc_stats_reply_t *obj,
    of_serial_num_t serial_num)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_DESC_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 780;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 784;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_ser_num_set(wbuf, abs_offset, serial_num);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get dp_desc from an object of type of_desc_stats_reply.
 * @param obj Pointer to an object of type of_desc_stats_reply.
 * @param dp_desc Pointer to the child object of type
 * of_desc_str_t to be filled out.
 *
 */
void
of_desc_stats_reply_dp_desc_get(
    of_desc_stats_reply_t *obj,
    of_desc_str_t *dp_desc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_DESC_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 812;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 816;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_desc_str_get(wbuf, abs_offset, dp_desc);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set dp_desc in an object of type of_desc_stats_reply.
 * @param obj Pointer to an object of type of_desc_stats_reply.
 * @param dp_desc The value to write into the object
 */
void
of_desc_stats_reply_dp_desc_set(
    of_desc_stats_reply_t *obj,
    of_desc_str_t dp_desc)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_DESC_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 812;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 816;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_desc_str_set(wbuf, abs_offset, dp_desc);

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
of_desc_stats_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x10; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0x0); /* stats_type */
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0x0); /* stats_type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_desc_stats_request of_desc_stats_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_desc_stats_request_push_wire_values(of_desc_stats_request_t *obj)
{

    of_desc_stats_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_desc_stats_request object
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
 * \ingroup of_desc_stats_request
 */

of_desc_stats_request_t *
of_desc_stats_request_new(of_version_t version)
{
    of_desc_stats_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_DESC_STATS_REQUEST] + of_object_extra_len[version][OF_DESC_STATS_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_desc_stats_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_desc_stats_request_init(obj, version, bytes, 0);

    if (of_desc_stats_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_desc_stats_request.
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
of_desc_stats_request_init(of_desc_stats_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_DESC_STATS_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_DESC_STATS_REQUEST] + of_object_extra_len[version][OF_DESC_STATS_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_DESC_STATS_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_desc_stats_request_push_wire_types;

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
 * Create a new of_desc_stats_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_desc_stats_request
 */

of_desc_stats_request_t *
of_desc_stats_request_new_from_message(of_message_t msg)
{
    of_desc_stats_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_desc_stats_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_desc_stats_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_desc_stats_request.
 * @param obj Pointer to an object of type of_desc_stats_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_desc_stats_request_xid_get(
    of_desc_stats_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_DESC_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_desc_stats_request.
 * @param obj Pointer to an object of type of_desc_stats_request.
 * @param xid The value to write into the object
 */
void
of_desc_stats_request_xid_set(
    of_desc_stats_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_DESC_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get flags from an object of type of_desc_stats_request.
 * @param obj Pointer to an object of type of_desc_stats_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_desc_stats_request_flags_get(
    of_desc_stats_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_DESC_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set flags in an object of type of_desc_stats_request.
 * @param obj Pointer to an object of type of_desc_stats_request.
 * @param flags The value to write into the object
 */
void
of_desc_stats_request_flags_set(
    of_desc_stats_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_DESC_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
of_echo_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x3; /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_echo_reply of_echo_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_echo_reply_push_wire_values(of_echo_reply_t *obj)
{

    of_echo_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_echo_reply object
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
 * \ingroup of_echo_reply
 */

of_echo_reply_t *
of_echo_reply_new(of_version_t version)
{
    of_echo_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ECHO_REPLY] + of_object_extra_len[version][OF_ECHO_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_echo_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_echo_reply_init(obj, version, bytes, 0);

    if (of_echo_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_echo_reply.
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
of_echo_reply_init(of_echo_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ECHO_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ECHO_REPLY] + of_object_extra_len[version][OF_ECHO_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ECHO_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_echo_reply_push_wire_types;

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
 * Create a new of_echo_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_echo_reply
 */

of_echo_reply_t *
of_echo_reply_new_from_message(of_message_t msg)
{
    of_echo_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_echo_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_echo_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_echo_reply.
 * @param obj Pointer to an object of type of_echo_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_echo_reply_xid_get(
    of_echo_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ECHO_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_echo_reply.
 * @param obj Pointer to an object of type of_echo_reply.
 * @param xid The value to write into the object
 */
void
of_echo_reply_xid_set(
    of_echo_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ECHO_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get data from an object of type of_echo_reply.
 * @param obj Pointer to an object of type of_echo_reply.
 * @param data Pointer to the child object of type
 * of_octets_t to be filled out.
 *
 */
void
of_echo_reply_data_get(
    of_echo_reply_t *obj,
    of_octets_t *data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_ECHO_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
    LOCI_ASSERT(cur_len + abs_offset <= WBUF_CURRENT_BYTES(wbuf));
    data->bytes = cur_len;
    data->data = OF_WIRE_BUFFER_INDEX(wbuf, abs_offset);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set data in an object of type of_echo_reply.
 * @param obj Pointer to an object of type of_echo_reply.
 * @param data The value to write into the object
 */
int WARN_UNUSED_RESULT
of_echo_reply_data_set(
    of_echo_reply_t *obj,
    of_octets_t *data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_ECHO_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
of_echo_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x2; /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_echo_request of_echo_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_echo_request_push_wire_values(of_echo_request_t *obj)
{

    of_echo_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_echo_request object
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
 * \ingroup of_echo_request
 */

of_echo_request_t *
of_echo_request_new(of_version_t version)
{
    of_echo_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_ECHO_REQUEST] + of_object_extra_len[version][OF_ECHO_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_echo_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_echo_request_init(obj, version, bytes, 0);

    if (of_echo_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_echo_request.
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
of_echo_request_init(of_echo_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_ECHO_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_ECHO_REQUEST] + of_object_extra_len[version][OF_ECHO_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_ECHO_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_echo_request_push_wire_types;

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
 * Create a new of_echo_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_echo_request
 */

of_echo_request_t *
of_echo_request_new_from_message(of_message_t msg)
{
    of_echo_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_echo_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_echo_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_echo_request.
 * @param obj Pointer to an object of type of_echo_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_echo_request_xid_get(
    of_echo_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ECHO_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_echo_request.
 * @param obj Pointer to an object of type of_echo_request.
 * @param xid The value to write into the object
 */
void
of_echo_request_xid_set(
    of_echo_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_ECHO_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get data from an object of type of_echo_request.
 * @param obj Pointer to an object of type of_echo_request.
 * @param data Pointer to the child object of type
 * of_octets_t to be filled out.
 *
 */
void
of_echo_request_data_get(
    of_echo_request_t *obj,
    of_octets_t *data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_ECHO_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
    LOCI_ASSERT(cur_len + abs_offset <= WBUF_CURRENT_BYTES(wbuf));
    data->bytes = cur_len;
    data->data = OF_WIRE_BUFFER_INDEX(wbuf, abs_offset);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set data in an object of type of_echo_request.
 * @param obj Pointer to an object of type of_echo_request.
 * @param data The value to write into the object
 */
int WARN_UNUSED_RESULT
of_echo_request_data_set(
    of_echo_request_t *obj,
    of_octets_t *data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_ECHO_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
of_features_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x6; /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_features_reply of_features_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_features_reply_push_wire_values(of_features_reply_t *obj)
{

    of_features_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_features_reply object
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
 * \ingroup of_features_reply
 */

of_features_reply_t *
of_features_reply_new(of_version_t version)
{
    of_features_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_FEATURES_REPLY] + of_object_extra_len[version][OF_FEATURES_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_features_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_features_reply_init(obj, version, bytes, 0);

    if (of_features_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_features_reply.
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
of_features_reply_init(of_features_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_FEATURES_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_FEATURES_REPLY] + of_object_extra_len[version][OF_FEATURES_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_FEATURES_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_features_reply_push_wire_types;

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
 * Create a new of_features_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_features_reply
 */

of_features_reply_t *
of_features_reply_new_from_message(of_message_t msg)
{
    of_features_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_features_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_features_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_features_reply.
 * @param obj Pointer to an object of type of_features_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_features_reply_xid_get(
    of_features_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FEATURES_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_features_reply.
 * @param obj Pointer to an object of type of_features_reply.
 * @param xid The value to write into the object
 */
void
of_features_reply_xid_set(
    of_features_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FEATURES_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get datapath_id from an object of type of_features_reply.
 * @param obj Pointer to an object of type of_features_reply.
 * @param datapath_id Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_features_reply_datapath_id_get(
    of_features_reply_t *obj,
    uint64_t *datapath_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FEATURES_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, datapath_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set datapath_id in an object of type of_features_reply.
 * @param obj Pointer to an object of type of_features_reply.
 * @param datapath_id The value to write into the object
 */
void
of_features_reply_datapath_id_set(
    of_features_reply_t *obj,
    uint64_t datapath_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FEATURES_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, datapath_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get n_buffers from an object of type of_features_reply.
 * @param obj Pointer to an object of type of_features_reply.
 * @param n_buffers Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_features_reply_n_buffers_get(
    of_features_reply_t *obj,
    uint32_t *n_buffers)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FEATURES_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, n_buffers);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set n_buffers in an object of type of_features_reply.
 * @param obj Pointer to an object of type of_features_reply.
 * @param n_buffers The value to write into the object
 */
void
of_features_reply_n_buffers_set(
    of_features_reply_t *obj,
    uint32_t n_buffers)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FEATURES_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, n_buffers);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get n_tables from an object of type of_features_reply.
 * @param obj Pointer to an object of type of_features_reply.
 * @param n_tables Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_features_reply_n_tables_get(
    of_features_reply_t *obj,
    uint8_t *n_tables)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FEATURES_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, n_tables);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set n_tables in an object of type of_features_reply.
 * @param obj Pointer to an object of type of_features_reply.
 * @param n_tables The value to write into the object
 */
void
of_features_reply_n_tables_set(
    of_features_reply_t *obj,
    uint8_t n_tables)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FEATURES_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, n_tables);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get auxiliary_id from an object of type of_features_reply.
 * @param obj Pointer to an object of type of_features_reply.
 * @param auxiliary_id Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_features_reply_auxiliary_id_get(
    of_features_reply_t *obj,
    uint8_t *auxiliary_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FEATURES_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 21;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, auxiliary_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set auxiliary_id in an object of type of_features_reply.
 * @param obj Pointer to an object of type of_features_reply.
 * @param auxiliary_id The value to write into the object
 */
void
of_features_reply_auxiliary_id_set(
    of_features_reply_t *obj,
    uint8_t auxiliary_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FEATURES_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 21;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, auxiliary_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get capabilities from an object of type of_features_reply.
 * @param obj Pointer to an object of type of_features_reply.
 * @param capabilities Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_features_reply_capabilities_get(
    of_features_reply_t *obj,
    uint32_t *capabilities)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FEATURES_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
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
 * Set capabilities in an object of type of_features_reply.
 * @param obj Pointer to an object of type of_features_reply.
 * @param capabilities The value to write into the object
 */
void
of_features_reply_capabilities_set(
    of_features_reply_t *obj,
    uint32_t capabilities)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FEATURES_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
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
 * Get reserved from an object of type of_features_reply.
 * @param obj Pointer to an object of type of_features_reply.
 * @param reserved Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_features_reply_reserved_get(
    of_features_reply_t *obj,
    uint32_t *reserved)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FEATURES_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, reserved);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set reserved in an object of type of_features_reply.
 * @param obj Pointer to an object of type of_features_reply.
 * @param reserved The value to write into the object
 */
void
of_features_reply_reserved_set(
    of_features_reply_t *obj,
    uint32_t reserved)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FEATURES_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, reserved);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Bind an object of type of_list_port_desc_t to the parent of type of_features_reply for
 * member ports
 * @param obj Pointer to an object of type of_features_reply.
 * @param ports Pointer to the child object of type
 * of_list_port_desc_t to be filled out.
 * \ingroup of_features_reply
 *
 * The parameter ports is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_features_reply_ports_bind(
    of_features_reply_t *obj,
    of_list_port_desc_t *ports)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_FEATURES_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_port_desc_init(ports, obj->version, 0, 1);
    /* Attach to parent */
    ports->parent = (of_object_t *)obj;
    ports->wire_object.wbuf = obj->wire_object.wbuf;
    ports->wire_object.obj_offset = abs_offset;
    ports->wire_object.owned = 0;
    ports->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of ports into a new variable of type of_list_port_desc_t from
 * a of_features_reply instance.
 *
 * @param obj Pointer to the source of type of_features_reply_t
 * @returns A pointer to a new instance of type of_list_port_desc_t whose contents
 * match that of ports from source
 * @returns NULL if an error occurs
 */
of_list_port_desc_t *
of_features_reply_ports_get(of_features_reply_t *obj) {
    of_list_port_desc_t _ports;
    of_list_port_desc_t *_ports_ptr;

    of_features_reply_ports_bind(obj, &_ports);
    _ports_ptr = (of_list_port_desc_t *)of_object_dup(&_ports);
    return _ports_ptr;
}

/**
 * Set ports in an object of type of_features_reply.
 * @param obj Pointer to an object of type of_features_reply.
 * @param ports Pointer to the child of type of_list_port_desc_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_features_reply_ports_set(
    of_features_reply_t *obj,
    of_list_port_desc_t *ports)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_FEATURES_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = ports->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == ports->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(ports, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(ports, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (ports->wire_length_set != NULL) {
        ports->wire_length_set((of_object_t *)ports, ports->length);
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

/**
 * Get actions from an object of type of_features_reply.
 * @param obj Pointer to an object of type of_features_reply.
 * @param actions Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_features_reply_actions_get(
    of_features_reply_t *obj,
    uint32_t *actions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FEATURES_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, actions);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set actions in an object of type of_features_reply.
 * @param obj Pointer to an object of type of_features_reply.
 * @param actions The value to write into the object
 */
void
of_features_reply_actions_set(
    of_features_reply_t *obj,
    uint32_t actions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FEATURES_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, actions);

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
of_features_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x5; /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_features_request of_features_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_features_request_push_wire_values(of_features_request_t *obj)
{

    of_features_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_features_request object
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
 * \ingroup of_features_request
 */

of_features_request_t *
of_features_request_new(of_version_t version)
{
    of_features_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_FEATURES_REQUEST] + of_object_extra_len[version][OF_FEATURES_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_features_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_features_request_init(obj, version, bytes, 0);

    if (of_features_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_features_request.
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
of_features_request_init(of_features_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_FEATURES_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_FEATURES_REQUEST] + of_object_extra_len[version][OF_FEATURES_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_FEATURES_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_features_request_push_wire_types;

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
 * Create a new of_features_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_features_request
 */

of_features_request_t *
of_features_request_new_from_message(of_message_t msg)
{
    of_features_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_features_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_features_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_features_request.
 * @param obj Pointer to an object of type of_features_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_features_request_xid_get(
    of_features_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FEATURES_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_features_request.
 * @param obj Pointer to an object of type of_features_request.
 * @param xid The value to write into the object
 */
void
of_features_request_xid_set(
    of_features_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FEATURES_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
of_flow_mod_wire_object_id_get(of_object_t *obj, of_object_id_t *id)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0: {
        uint16_t value = U16_NTOH(*(uint16_t *)(buf + 56)); /* _command */
        switch (value) {
        case 0x0:
            *id = OF_FLOW_ADD;
            break;
        case 0x1:
            *id = OF_FLOW_MODIFY;
            break;
        case 0x2:
            *id = OF_FLOW_MODIFY_STRICT;
            break;
        case 0x3:
            *id = OF_FLOW_DELETE;
            break;
        case 0x4:
            *id = OF_FLOW_DELETE_STRICT;
            break;
        default:
            *id = OF_FLOW_MOD;
            break;
        }
        break;
    }
    case OF_VERSION_1_1: {
        uint8_t value = *(uint8_t *)(buf + 25); /* _command */
        switch (value) {
        case 0x0:
            *id = OF_FLOW_ADD;
            break;
        case 0x1:
            *id = OF_FLOW_MODIFY;
            break;
        case 0x2:
            *id = OF_FLOW_MODIFY_STRICT;
            break;
        case 0x3:
            *id = OF_FLOW_DELETE;
            break;
        case 0x4:
            *id = OF_FLOW_DELETE_STRICT;
            break;
        default:
            *id = OF_FLOW_MOD;
            break;
        }
        break;
    }
    case OF_VERSION_1_2: {
        uint8_t value = *(uint8_t *)(buf + 25); /* _command */
        switch (value) {
        case 0x0:
            *id = OF_FLOW_ADD;
            break;
        case 0x1:
            *id = OF_FLOW_MODIFY;
            break;
        case 0x2:
            *id = OF_FLOW_MODIFY_STRICT;
            break;
        case 0x3:
            *id = OF_FLOW_DELETE;
            break;
        case 0x4:
            *id = OF_FLOW_DELETE_STRICT;
            break;
        default:
            *id = OF_FLOW_MOD;
            break;
        }
        break;
    }
    case OF_VERSION_1_3: {
        uint8_t value = *(uint8_t *)(buf + 25); /* _command */
        switch (value) {
        case 0x0:
            *id = OF_FLOW_ADD;
            break;
        case 0x1:
            *id = OF_FLOW_MODIFY;
            break;
        case 0x2:
            *id = OF_FLOW_MODIFY_STRICT;
            break;
        case 0x3:
            *id = OF_FLOW_DELETE;
            break;
        case 0x4:
            *id = OF_FLOW_DELETE_STRICT;
            break;
        default:
            *id = OF_FLOW_MOD;
            break;
        }
        break;
    }
    default:
        LOCI_ASSERT(0);
    }
}


/**
 * \defgroup of_flow_mod of_flow_mod
 */

/**
 * Create a new of_flow_mod object
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
 * \ingroup of_flow_mod
 */

of_flow_mod_t *
of_flow_mod_new(of_version_t version)
{
    of_flow_mod_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_FLOW_MOD] + of_object_extra_len[version][OF_FLOW_MOD];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_flow_mod_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_flow_mod_init(obj, version, bytes, 0);

    /* Initialize match TLV for 1.2 */
    /* FIXME: Check 1.3 below */
    if ((version == OF_VERSION_1_2) || (version == OF_VERSION_1_3)) {
        of_object_u16_set((of_object_t *)obj, 48 + 2, 4);
    }

    return obj;
}

/**
 * Initialize an object of type of_flow_mod.
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
of_flow_mod_init(of_flow_mod_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_FLOW_MOD] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_FLOW_MOD] + of_object_extra_len[version][OF_FLOW_MOD];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_FLOW_MOD;

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
 * Create a new of_flow_mod object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_flow_mod
 */

of_flow_mod_t *
of_flow_mod_new_from_message(of_message_t msg)
{
    of_flow_mod_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_flow_mod_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_flow_mod_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_mod_xid_get(
    of_flow_mod_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param xid The value to write into the object
 */
void
of_flow_mod_xid_set(
    of_flow_mod_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get cookie from an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param cookie Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_flow_mod_cookie_get(
    of_flow_mod_t *obj,
    uint64_t *cookie)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 48;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, cookie);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set cookie in an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param cookie The value to write into the object
 */
void
of_flow_mod_cookie_set(
    of_flow_mod_t *obj,
    uint64_t cookie)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 48;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, cookie);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get cookie_mask from an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param cookie_mask Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_flow_mod_cookie_mask_get(
    of_flow_mod_t *obj,
    uint64_t *cookie_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, cookie_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set cookie_mask in an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param cookie_mask The value to write into the object
 */
void
of_flow_mod_cookie_mask_set(
    of_flow_mod_t *obj,
    uint64_t cookie_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, cookie_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get table_id from an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param table_id Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_flow_mod_table_id_get(
    of_flow_mod_t *obj,
    uint8_t *table_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
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
 * Set table_id in an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param table_id The value to write into the object
 */
void
of_flow_mod_table_id_set(
    of_flow_mod_t *obj,
    uint8_t table_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
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
 * Get idle_timeout from an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param idle_timeout Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_mod_idle_timeout_get(
    of_flow_mod_t *obj,
    uint16_t *idle_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 58;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, idle_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set idle_timeout in an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param idle_timeout The value to write into the object
 */
void
of_flow_mod_idle_timeout_set(
    of_flow_mod_t *obj,
    uint16_t idle_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 58;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, idle_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get hard_timeout from an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param hard_timeout Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_mod_hard_timeout_get(
    of_flow_mod_t *obj,
    uint16_t *hard_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 60;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, hard_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set hard_timeout in an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param hard_timeout The value to write into the object
 */
void
of_flow_mod_hard_timeout_set(
    of_flow_mod_t *obj,
    uint16_t hard_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 60;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, hard_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get priority from an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param priority Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_mod_priority_get(
    of_flow_mod_t *obj,
    uint16_t *priority)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 62;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, priority);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set priority in an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param priority The value to write into the object
 */
void
of_flow_mod_priority_set(
    of_flow_mod_t *obj,
    uint16_t priority)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 62;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, priority);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get buffer_id from an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param buffer_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_mod_buffer_id_get(
    of_flow_mod_t *obj,
    uint32_t *buffer_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 64;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, buffer_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set buffer_id in an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param buffer_id The value to write into the object
 */
void
of_flow_mod_buffer_id_set(
    of_flow_mod_t *obj,
    uint32_t buffer_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 64;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, buffer_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get out_port from an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param out_port Pointer to the child object of type
 * of_port_no_t to be filled out.
 *
 */
void
of_flow_mod_out_port_get(
    of_flow_mod_t *obj,
    of_port_no_t *out_port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 68;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_port_no_get(ver, wbuf, abs_offset, out_port);
    OF_PORT_NO_VALUE_CHECK(*out_port, ver);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set out_port in an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param out_port The value to write into the object
 */
void
of_flow_mod_out_port_set(
    of_flow_mod_t *obj,
    of_port_no_t out_port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 68;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_port_no_set(ver, wbuf, abs_offset, out_port);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get out_group from an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param out_group Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_mod_out_group_get(
    of_flow_mod_t *obj,
    uint32_t *out_group)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, out_group);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set out_group in an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param out_group The value to write into the object
 */
void
of_flow_mod_out_group_set(
    of_flow_mod_t *obj,
    uint32_t out_group)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, out_group);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_mod_flags_get(
    of_flow_mod_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 70;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 44;
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
 * Set flags in an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param flags The value to write into the object
 */
void
of_flow_mod_flags_set(
    of_flow_mod_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 70;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 44;
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
 * Get match from an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param match Pointer to the child object of type
 * of_match_t to be filled out.
 *
 */
int WARN_UNUSED_RESULT
of_flow_mod_match_get(
    of_flow_mod_t *obj,
    of_match_t *match)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    of_octets_t match_octets; /* Serialized string for match */

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 8;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    LOCI_ASSERT(cur_len + abs_offset <= WBUF_CURRENT_BYTES(wbuf));
    match_octets.bytes = cur_len;
    match_octets.data = OF_OBJECT_BUFFER_INDEX(obj, offset);
    OF_TRY(of_match_deserialize(ver, match, &match_octets));

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}

/**
 * Set match in an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param match Pointer to the child of type of_match_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_mod_match_set(
    of_flow_mod_t *obj,
    of_match_t *match)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */
    of_octets_t match_octets; /* Serialized string for match */

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 8;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Match object */
    OF_TRY(of_match_serialize(ver, match, &match_octets));
    new_len = match_octets.bytes;
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        match_octets.data, new_len);
    /* Free match serialized octets */
    FREE(match_octets.data);

    /* Not scalar, update lengths if needed */
    delta = new_len - cur_len;
    if (delta != 0) {
        /* Update parent(s) */
        of_object_parent_length_update((of_object_t *)obj, delta);
    }

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}

/**
 * Bind an object of type of_list_instruction_t to the parent of type of_flow_mod for
 * member instructions
 * @param obj Pointer to an object of type of_flow_mod.
 * @param instructions Pointer to the child object of type
 * of_list_instruction_t to be filled out.
 * \ingroup of_flow_mod
 *
 * The parameter instructions is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_flow_mod_instructions_bind(
    of_flow_mod_t *obj,
    of_list_instruction_t *instructions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
        offset = 136;
        cur_len = _END_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = _FLOW_MOD_INSTRUCTIONS_OFFSET(obj);
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_instruction_init(instructions, obj->version, 0, 1);
    /* Attach to parent */
    instructions->parent = (of_object_t *)obj;
    instructions->wire_object.wbuf = obj->wire_object.wbuf;
    instructions->wire_object.obj_offset = abs_offset;
    instructions->wire_object.owned = 0;
    instructions->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of instructions into a new variable of type of_list_instruction_t from
 * a of_flow_mod instance.
 *
 * @param obj Pointer to the source of type of_flow_mod_t
 * @returns A pointer to a new instance of type of_list_instruction_t whose contents
 * match that of instructions from source
 * @returns NULL if an error occurs
 */
of_list_instruction_t *
of_flow_mod_instructions_get(of_flow_mod_t *obj) {
    of_list_instruction_t _instructions;
    of_list_instruction_t *_instructions_ptr;

    of_flow_mod_instructions_bind(obj, &_instructions);
    _instructions_ptr = (of_list_instruction_t *)of_object_dup(&_instructions);
    return _instructions_ptr;
}

/**
 * Set instructions in an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param instructions Pointer to the child of type of_list_instruction_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_mod_instructions_set(
    of_flow_mod_t *obj,
    of_list_instruction_t *instructions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
        offset = 136;
        cur_len = _END_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = _FLOW_MOD_INSTRUCTIONS_OFFSET(obj);
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = instructions->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == instructions->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(instructions, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(instructions, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (instructions->wire_length_set != NULL) {
        instructions->wire_length_set((of_object_t *)instructions, instructions->length);
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

/**
 * Bind an object of type of_list_action_t to the parent of type of_flow_mod for
 * member actions
 * @param obj Pointer to an object of type of_flow_mod.
 * @param actions Pointer to the child object of type
 * of_list_action_t to be filled out.
 * \ingroup of_flow_mod
 *
 * The parameter actions is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_flow_mod_actions_bind(
    of_flow_mod_t *obj,
    of_list_action_t *actions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 72;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_action_init(actions, obj->version, 0, 1);
    /* Attach to parent */
    actions->parent = (of_object_t *)obj;
    actions->wire_object.wbuf = obj->wire_object.wbuf;
    actions->wire_object.obj_offset = abs_offset;
    actions->wire_object.owned = 0;
    actions->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of actions into a new variable of type of_list_action_t from
 * a of_flow_mod instance.
 *
 * @param obj Pointer to the source of type of_flow_mod_t
 * @returns A pointer to a new instance of type of_list_action_t whose contents
 * match that of actions from source
 * @returns NULL if an error occurs
 */
of_list_action_t *
of_flow_mod_actions_get(of_flow_mod_t *obj) {
    of_list_action_t _actions;
    of_list_action_t *_actions_ptr;

    of_flow_mod_actions_bind(obj, &_actions);
    _actions_ptr = (of_list_action_t *)of_object_dup(&_actions);
    return _actions_ptr;
}

/**
 * Set actions in an object of type of_flow_mod.
 * @param obj Pointer to an object of type of_flow_mod.
 * @param actions Pointer to the child of type of_list_action_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_mod_actions_set(
    of_flow_mod_t *obj,
    of_list_action_t *actions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 72;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = actions->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == actions->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(actions, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(actions, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (actions->wire_length_set != NULL) {
        actions->wire_length_set((of_object_t *)actions, actions->length);
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
of_flow_add_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0xe; /* type */
        *(uint16_t *)(buf + 56) = U16_HTON(0x0); /* _command */
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0xe; /* type */
        *(uint8_t *)(buf + 25) = 0x0; /* _command */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_flow_add of_flow_add
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_flow_add_push_wire_values(of_flow_add_t *obj)
{

    of_flow_add_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_flow_add object
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
 * \ingroup of_flow_add
 */

of_flow_add_t *
of_flow_add_new(of_version_t version)
{
    of_flow_add_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_FLOW_ADD] + of_object_extra_len[version][OF_FLOW_ADD];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_flow_add_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_flow_add_init(obj, version, bytes, 0);

    if (of_flow_add_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    /* Initialize match TLV for 1.2 */
    /* FIXME: Check 1.3 below */
    if ((version == OF_VERSION_1_2) || (version == OF_VERSION_1_3)) {
        of_object_u16_set((of_object_t *)obj, 48 + 2, 4);
    }

    return obj;
}

/**
 * Initialize an object of type of_flow_add.
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
of_flow_add_init(of_flow_add_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_FLOW_ADD] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_FLOW_ADD] + of_object_extra_len[version][OF_FLOW_ADD];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_FLOW_ADD;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_flow_add_push_wire_types;

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
 * Create a new of_flow_add object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_flow_add
 */

of_flow_add_t *
of_flow_add_new_from_message(of_message_t msg)
{
    of_flow_add_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_flow_add_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_flow_add_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_add_xid_get(
    of_flow_add_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param xid The value to write into the object
 */
void
of_flow_add_xid_set(
    of_flow_add_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get cookie from an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param cookie Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_flow_add_cookie_get(
    of_flow_add_t *obj,
    uint64_t *cookie)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 48;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, cookie);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set cookie in an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param cookie The value to write into the object
 */
void
of_flow_add_cookie_set(
    of_flow_add_t *obj,
    uint64_t cookie)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 48;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, cookie);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get cookie_mask from an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param cookie_mask Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_flow_add_cookie_mask_get(
    of_flow_add_t *obj,
    uint64_t *cookie_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, cookie_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set cookie_mask in an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param cookie_mask The value to write into the object
 */
void
of_flow_add_cookie_mask_set(
    of_flow_add_t *obj,
    uint64_t cookie_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, cookie_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get table_id from an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param table_id Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_flow_add_table_id_get(
    of_flow_add_t *obj,
    uint8_t *table_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
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
 * Set table_id in an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param table_id The value to write into the object
 */
void
of_flow_add_table_id_set(
    of_flow_add_t *obj,
    uint8_t table_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
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
 * Get idle_timeout from an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param idle_timeout Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_add_idle_timeout_get(
    of_flow_add_t *obj,
    uint16_t *idle_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 58;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, idle_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set idle_timeout in an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param idle_timeout The value to write into the object
 */
void
of_flow_add_idle_timeout_set(
    of_flow_add_t *obj,
    uint16_t idle_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 58;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, idle_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get hard_timeout from an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param hard_timeout Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_add_hard_timeout_get(
    of_flow_add_t *obj,
    uint16_t *hard_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 60;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, hard_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set hard_timeout in an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param hard_timeout The value to write into the object
 */
void
of_flow_add_hard_timeout_set(
    of_flow_add_t *obj,
    uint16_t hard_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 60;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, hard_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get priority from an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param priority Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_add_priority_get(
    of_flow_add_t *obj,
    uint16_t *priority)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 62;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, priority);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set priority in an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param priority The value to write into the object
 */
void
of_flow_add_priority_set(
    of_flow_add_t *obj,
    uint16_t priority)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 62;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, priority);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get buffer_id from an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param buffer_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_add_buffer_id_get(
    of_flow_add_t *obj,
    uint32_t *buffer_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 64;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, buffer_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set buffer_id in an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param buffer_id The value to write into the object
 */
void
of_flow_add_buffer_id_set(
    of_flow_add_t *obj,
    uint32_t buffer_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 64;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, buffer_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get out_port from an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param out_port Pointer to the child object of type
 * of_port_no_t to be filled out.
 *
 */
void
of_flow_add_out_port_get(
    of_flow_add_t *obj,
    of_port_no_t *out_port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 68;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_port_no_get(ver, wbuf, abs_offset, out_port);
    OF_PORT_NO_VALUE_CHECK(*out_port, ver);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set out_port in an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param out_port The value to write into the object
 */
void
of_flow_add_out_port_set(
    of_flow_add_t *obj,
    of_port_no_t out_port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 68;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_port_no_set(ver, wbuf, abs_offset, out_port);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get out_group from an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param out_group Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_add_out_group_get(
    of_flow_add_t *obj,
    uint32_t *out_group)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, out_group);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set out_group in an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param out_group The value to write into the object
 */
void
of_flow_add_out_group_set(
    of_flow_add_t *obj,
    uint32_t out_group)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, out_group);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_add_flags_get(
    of_flow_add_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 70;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 44;
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
 * Set flags in an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param flags The value to write into the object
 */
void
of_flow_add_flags_set(
    of_flow_add_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 70;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 44;
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
 * Get match from an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param match Pointer to the child object of type
 * of_match_t to be filled out.
 *
 */
int WARN_UNUSED_RESULT
of_flow_add_match_get(
    of_flow_add_t *obj,
    of_match_t *match)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    of_octets_t match_octets; /* Serialized string for match */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 8;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    LOCI_ASSERT(cur_len + abs_offset <= WBUF_CURRENT_BYTES(wbuf));
    match_octets.bytes = cur_len;
    match_octets.data = OF_OBJECT_BUFFER_INDEX(obj, offset);
    OF_TRY(of_match_deserialize(ver, match, &match_octets));

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}

/**
 * Set match in an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param match Pointer to the child of type of_match_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_add_match_set(
    of_flow_add_t *obj,
    of_match_t *match)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */
    of_octets_t match_octets; /* Serialized string for match */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 8;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Match object */
    OF_TRY(of_match_serialize(ver, match, &match_octets));
    new_len = match_octets.bytes;
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        match_octets.data, new_len);
    /* Free match serialized octets */
    FREE(match_octets.data);

    /* Not scalar, update lengths if needed */
    delta = new_len - cur_len;
    if (delta != 0) {
        /* Update parent(s) */
        of_object_parent_length_update((of_object_t *)obj, delta);
    }

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}

/**
 * Bind an object of type of_list_instruction_t to the parent of type of_flow_add for
 * member instructions
 * @param obj Pointer to an object of type of_flow_add.
 * @param instructions Pointer to the child object of type
 * of_list_instruction_t to be filled out.
 * \ingroup of_flow_add
 *
 * The parameter instructions is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_flow_add_instructions_bind(
    of_flow_add_t *obj,
    of_list_instruction_t *instructions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
        offset = 136;
        cur_len = _END_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = _FLOW_ADD_INSTRUCTIONS_OFFSET(obj);
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_instruction_init(instructions, obj->version, 0, 1);
    /* Attach to parent */
    instructions->parent = (of_object_t *)obj;
    instructions->wire_object.wbuf = obj->wire_object.wbuf;
    instructions->wire_object.obj_offset = abs_offset;
    instructions->wire_object.owned = 0;
    instructions->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of instructions into a new variable of type of_list_instruction_t from
 * a of_flow_add instance.
 *
 * @param obj Pointer to the source of type of_flow_add_t
 * @returns A pointer to a new instance of type of_list_instruction_t whose contents
 * match that of instructions from source
 * @returns NULL if an error occurs
 */
of_list_instruction_t *
of_flow_add_instructions_get(of_flow_add_t *obj) {
    of_list_instruction_t _instructions;
    of_list_instruction_t *_instructions_ptr;

    of_flow_add_instructions_bind(obj, &_instructions);
    _instructions_ptr = (of_list_instruction_t *)of_object_dup(&_instructions);
    return _instructions_ptr;
}

/**
 * Set instructions in an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param instructions Pointer to the child of type of_list_instruction_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_add_instructions_set(
    of_flow_add_t *obj,
    of_list_instruction_t *instructions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
        offset = 136;
        cur_len = _END_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = _FLOW_ADD_INSTRUCTIONS_OFFSET(obj);
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = instructions->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == instructions->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(instructions, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(instructions, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (instructions->wire_length_set != NULL) {
        instructions->wire_length_set((of_object_t *)instructions, instructions->length);
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

/**
 * Bind an object of type of_list_action_t to the parent of type of_flow_add for
 * member actions
 * @param obj Pointer to an object of type of_flow_add.
 * @param actions Pointer to the child object of type
 * of_list_action_t to be filled out.
 * \ingroup of_flow_add
 *
 * The parameter actions is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_flow_add_actions_bind(
    of_flow_add_t *obj,
    of_list_action_t *actions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 72;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_action_init(actions, obj->version, 0, 1);
    /* Attach to parent */
    actions->parent = (of_object_t *)obj;
    actions->wire_object.wbuf = obj->wire_object.wbuf;
    actions->wire_object.obj_offset = abs_offset;
    actions->wire_object.owned = 0;
    actions->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of actions into a new variable of type of_list_action_t from
 * a of_flow_add instance.
 *
 * @param obj Pointer to the source of type of_flow_add_t
 * @returns A pointer to a new instance of type of_list_action_t whose contents
 * match that of actions from source
 * @returns NULL if an error occurs
 */
of_list_action_t *
of_flow_add_actions_get(of_flow_add_t *obj) {
    of_list_action_t _actions;
    of_list_action_t *_actions_ptr;

    of_flow_add_actions_bind(obj, &_actions);
    _actions_ptr = (of_list_action_t *)of_object_dup(&_actions);
    return _actions_ptr;
}

/**
 * Set actions in an object of type of_flow_add.
 * @param obj Pointer to an object of type of_flow_add.
 * @param actions Pointer to the child of type of_list_action_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_add_actions_set(
    of_flow_add_t *obj,
    of_list_action_t *actions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 72;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = actions->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == actions->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(actions, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(actions, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (actions->wire_length_set != NULL) {
        actions->wire_length_set((of_object_t *)actions, actions->length);
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
of_flow_delete_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0xe; /* type */
        *(uint16_t *)(buf + 56) = U16_HTON(0x3); /* _command */
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0xe; /* type */
        *(uint8_t *)(buf + 25) = 0x3; /* _command */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_flow_delete of_flow_delete
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_flow_delete_push_wire_values(of_flow_delete_t *obj)
{

    of_flow_delete_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_flow_delete object
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
 * \ingroup of_flow_delete
 */

of_flow_delete_t *
of_flow_delete_new(of_version_t version)
{
    of_flow_delete_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_FLOW_DELETE] + of_object_extra_len[version][OF_FLOW_DELETE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_flow_delete_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_flow_delete_init(obj, version, bytes, 0);

    if (of_flow_delete_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    /* Initialize match TLV for 1.2 */
    /* FIXME: Check 1.3 below */
    if ((version == OF_VERSION_1_2) || (version == OF_VERSION_1_3)) {
        of_object_u16_set((of_object_t *)obj, 48 + 2, 4);
    }

    return obj;
}

/**
 * Initialize an object of type of_flow_delete.
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
of_flow_delete_init(of_flow_delete_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_FLOW_DELETE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_FLOW_DELETE] + of_object_extra_len[version][OF_FLOW_DELETE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_FLOW_DELETE;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_flow_delete_push_wire_types;

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
 * Create a new of_flow_delete object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_flow_delete
 */

of_flow_delete_t *
of_flow_delete_new_from_message(of_message_t msg)
{
    of_flow_delete_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_flow_delete_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_flow_delete_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_delete_xid_get(
    of_flow_delete_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param xid The value to write into the object
 */
void
of_flow_delete_xid_set(
    of_flow_delete_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get cookie from an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param cookie Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_flow_delete_cookie_get(
    of_flow_delete_t *obj,
    uint64_t *cookie)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 48;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, cookie);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set cookie in an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param cookie The value to write into the object
 */
void
of_flow_delete_cookie_set(
    of_flow_delete_t *obj,
    uint64_t cookie)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 48;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, cookie);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get cookie_mask from an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param cookie_mask Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_flow_delete_cookie_mask_get(
    of_flow_delete_t *obj,
    uint64_t *cookie_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, cookie_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set cookie_mask in an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param cookie_mask The value to write into the object
 */
void
of_flow_delete_cookie_mask_set(
    of_flow_delete_t *obj,
    uint64_t cookie_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, cookie_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get table_id from an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param table_id Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_flow_delete_table_id_get(
    of_flow_delete_t *obj,
    uint8_t *table_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
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
 * Set table_id in an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param table_id The value to write into the object
 */
void
of_flow_delete_table_id_set(
    of_flow_delete_t *obj,
    uint8_t table_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
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
 * Get idle_timeout from an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param idle_timeout Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_delete_idle_timeout_get(
    of_flow_delete_t *obj,
    uint16_t *idle_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 58;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, idle_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set idle_timeout in an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param idle_timeout The value to write into the object
 */
void
of_flow_delete_idle_timeout_set(
    of_flow_delete_t *obj,
    uint16_t idle_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 58;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, idle_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get hard_timeout from an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param hard_timeout Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_delete_hard_timeout_get(
    of_flow_delete_t *obj,
    uint16_t *hard_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 60;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, hard_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set hard_timeout in an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param hard_timeout The value to write into the object
 */
void
of_flow_delete_hard_timeout_set(
    of_flow_delete_t *obj,
    uint16_t hard_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 60;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, hard_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get priority from an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param priority Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_delete_priority_get(
    of_flow_delete_t *obj,
    uint16_t *priority)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 62;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, priority);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set priority in an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param priority The value to write into the object
 */
void
of_flow_delete_priority_set(
    of_flow_delete_t *obj,
    uint16_t priority)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 62;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, priority);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get buffer_id from an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param buffer_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_delete_buffer_id_get(
    of_flow_delete_t *obj,
    uint32_t *buffer_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 64;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, buffer_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set buffer_id in an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param buffer_id The value to write into the object
 */
void
of_flow_delete_buffer_id_set(
    of_flow_delete_t *obj,
    uint32_t buffer_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 64;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, buffer_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get out_port from an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param out_port Pointer to the child object of type
 * of_port_no_t to be filled out.
 *
 */
void
of_flow_delete_out_port_get(
    of_flow_delete_t *obj,
    of_port_no_t *out_port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 68;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_port_no_get(ver, wbuf, abs_offset, out_port);
    OF_PORT_NO_VALUE_CHECK(*out_port, ver);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set out_port in an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param out_port The value to write into the object
 */
void
of_flow_delete_out_port_set(
    of_flow_delete_t *obj,
    of_port_no_t out_port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 68;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_port_no_set(ver, wbuf, abs_offset, out_port);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get out_group from an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param out_group Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_delete_out_group_get(
    of_flow_delete_t *obj,
    uint32_t *out_group)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, out_group);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set out_group in an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param out_group The value to write into the object
 */
void
of_flow_delete_out_group_set(
    of_flow_delete_t *obj,
    uint32_t out_group)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, out_group);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_delete_flags_get(
    of_flow_delete_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 70;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 44;
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
 * Set flags in an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param flags The value to write into the object
 */
void
of_flow_delete_flags_set(
    of_flow_delete_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 70;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 44;
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
 * Get match from an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param match Pointer to the child object of type
 * of_match_t to be filled out.
 *
 */
int WARN_UNUSED_RESULT
of_flow_delete_match_get(
    of_flow_delete_t *obj,
    of_match_t *match)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    of_octets_t match_octets; /* Serialized string for match */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 8;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    LOCI_ASSERT(cur_len + abs_offset <= WBUF_CURRENT_BYTES(wbuf));
    match_octets.bytes = cur_len;
    match_octets.data = OF_OBJECT_BUFFER_INDEX(obj, offset);
    OF_TRY(of_match_deserialize(ver, match, &match_octets));

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}

/**
 * Set match in an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param match Pointer to the child of type of_match_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_delete_match_set(
    of_flow_delete_t *obj,
    of_match_t *match)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */
    of_octets_t match_octets; /* Serialized string for match */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 8;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Match object */
    OF_TRY(of_match_serialize(ver, match, &match_octets));
    new_len = match_octets.bytes;
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        match_octets.data, new_len);
    /* Free match serialized octets */
    FREE(match_octets.data);

    /* Not scalar, update lengths if needed */
    delta = new_len - cur_len;
    if (delta != 0) {
        /* Update parent(s) */
        of_object_parent_length_update((of_object_t *)obj, delta);
    }

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}

/**
 * Bind an object of type of_list_instruction_t to the parent of type of_flow_delete for
 * member instructions
 * @param obj Pointer to an object of type of_flow_delete.
 * @param instructions Pointer to the child object of type
 * of_list_instruction_t to be filled out.
 * \ingroup of_flow_delete
 *
 * The parameter instructions is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_flow_delete_instructions_bind(
    of_flow_delete_t *obj,
    of_list_instruction_t *instructions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
        offset = 136;
        cur_len = _END_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = _FLOW_DELETE_INSTRUCTIONS_OFFSET(obj);
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_instruction_init(instructions, obj->version, 0, 1);
    /* Attach to parent */
    instructions->parent = (of_object_t *)obj;
    instructions->wire_object.wbuf = obj->wire_object.wbuf;
    instructions->wire_object.obj_offset = abs_offset;
    instructions->wire_object.owned = 0;
    instructions->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of instructions into a new variable of type of_list_instruction_t from
 * a of_flow_delete instance.
 *
 * @param obj Pointer to the source of type of_flow_delete_t
 * @returns A pointer to a new instance of type of_list_instruction_t whose contents
 * match that of instructions from source
 * @returns NULL if an error occurs
 */
of_list_instruction_t *
of_flow_delete_instructions_get(of_flow_delete_t *obj) {
    of_list_instruction_t _instructions;
    of_list_instruction_t *_instructions_ptr;

    of_flow_delete_instructions_bind(obj, &_instructions);
    _instructions_ptr = (of_list_instruction_t *)of_object_dup(&_instructions);
    return _instructions_ptr;
}

/**
 * Set instructions in an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param instructions Pointer to the child of type of_list_instruction_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_delete_instructions_set(
    of_flow_delete_t *obj,
    of_list_instruction_t *instructions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
        offset = 136;
        cur_len = _END_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = _FLOW_DELETE_INSTRUCTIONS_OFFSET(obj);
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = instructions->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == instructions->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(instructions, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(instructions, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (instructions->wire_length_set != NULL) {
        instructions->wire_length_set((of_object_t *)instructions, instructions->length);
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

/**
 * Bind an object of type of_list_action_t to the parent of type of_flow_delete for
 * member actions
 * @param obj Pointer to an object of type of_flow_delete.
 * @param actions Pointer to the child object of type
 * of_list_action_t to be filled out.
 * \ingroup of_flow_delete
 *
 * The parameter actions is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_flow_delete_actions_bind(
    of_flow_delete_t *obj,
    of_list_action_t *actions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 72;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_action_init(actions, obj->version, 0, 1);
    /* Attach to parent */
    actions->parent = (of_object_t *)obj;
    actions->wire_object.wbuf = obj->wire_object.wbuf;
    actions->wire_object.obj_offset = abs_offset;
    actions->wire_object.owned = 0;
    actions->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of actions into a new variable of type of_list_action_t from
 * a of_flow_delete instance.
 *
 * @param obj Pointer to the source of type of_flow_delete_t
 * @returns A pointer to a new instance of type of_list_action_t whose contents
 * match that of actions from source
 * @returns NULL if an error occurs
 */
of_list_action_t *
of_flow_delete_actions_get(of_flow_delete_t *obj) {
    of_list_action_t _actions;
    of_list_action_t *_actions_ptr;

    of_flow_delete_actions_bind(obj, &_actions);
    _actions_ptr = (of_list_action_t *)of_object_dup(&_actions);
    return _actions_ptr;
}

/**
 * Set actions in an object of type of_flow_delete.
 * @param obj Pointer to an object of type of_flow_delete.
 * @param actions Pointer to the child of type of_list_action_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_delete_actions_set(
    of_flow_delete_t *obj,
    of_list_action_t *actions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 72;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = actions->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == actions->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(actions, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(actions, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (actions->wire_length_set != NULL) {
        actions->wire_length_set((of_object_t *)actions, actions->length);
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
of_flow_delete_strict_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0xe; /* type */
        *(uint16_t *)(buf + 56) = U16_HTON(0x4); /* _command */
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0xe; /* type */
        *(uint8_t *)(buf + 25) = 0x4; /* _command */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_flow_delete_strict of_flow_delete_strict
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_flow_delete_strict_push_wire_values(of_flow_delete_strict_t *obj)
{

    of_flow_delete_strict_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_flow_delete_strict object
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
 * \ingroup of_flow_delete_strict
 */

of_flow_delete_strict_t *
of_flow_delete_strict_new(of_version_t version)
{
    of_flow_delete_strict_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_FLOW_DELETE_STRICT] + of_object_extra_len[version][OF_FLOW_DELETE_STRICT];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_flow_delete_strict_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_flow_delete_strict_init(obj, version, bytes, 0);

    if (of_flow_delete_strict_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    /* Initialize match TLV for 1.2 */
    /* FIXME: Check 1.3 below */
    if ((version == OF_VERSION_1_2) || (version == OF_VERSION_1_3)) {
        of_object_u16_set((of_object_t *)obj, 48 + 2, 4);
    }

    return obj;
}

/**
 * Initialize an object of type of_flow_delete_strict.
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
of_flow_delete_strict_init(of_flow_delete_strict_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_FLOW_DELETE_STRICT] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_FLOW_DELETE_STRICT] + of_object_extra_len[version][OF_FLOW_DELETE_STRICT];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_FLOW_DELETE_STRICT;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_flow_delete_strict_push_wire_types;

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
 * Create a new of_flow_delete_strict object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_flow_delete_strict
 */

of_flow_delete_strict_t *
of_flow_delete_strict_new_from_message(of_message_t msg)
{
    of_flow_delete_strict_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_flow_delete_strict_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_flow_delete_strict_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_delete_strict_xid_get(
    of_flow_delete_strict_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param xid The value to write into the object
 */
void
of_flow_delete_strict_xid_set(
    of_flow_delete_strict_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get cookie from an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param cookie Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_flow_delete_strict_cookie_get(
    of_flow_delete_strict_t *obj,
    uint64_t *cookie)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 48;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, cookie);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set cookie in an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param cookie The value to write into the object
 */
void
of_flow_delete_strict_cookie_set(
    of_flow_delete_strict_t *obj,
    uint64_t cookie)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 48;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, cookie);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get cookie_mask from an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param cookie_mask Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_flow_delete_strict_cookie_mask_get(
    of_flow_delete_strict_t *obj,
    uint64_t *cookie_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, cookie_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set cookie_mask in an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param cookie_mask The value to write into the object
 */
void
of_flow_delete_strict_cookie_mask_set(
    of_flow_delete_strict_t *obj,
    uint64_t cookie_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, cookie_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get table_id from an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param table_id Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_flow_delete_strict_table_id_get(
    of_flow_delete_strict_t *obj,
    uint8_t *table_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
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
 * Set table_id in an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param table_id The value to write into the object
 */
void
of_flow_delete_strict_table_id_set(
    of_flow_delete_strict_t *obj,
    uint8_t table_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
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
 * Get idle_timeout from an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param idle_timeout Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_delete_strict_idle_timeout_get(
    of_flow_delete_strict_t *obj,
    uint16_t *idle_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 58;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, idle_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set idle_timeout in an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param idle_timeout The value to write into the object
 */
void
of_flow_delete_strict_idle_timeout_set(
    of_flow_delete_strict_t *obj,
    uint16_t idle_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 58;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, idle_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get hard_timeout from an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param hard_timeout Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_delete_strict_hard_timeout_get(
    of_flow_delete_strict_t *obj,
    uint16_t *hard_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 60;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, hard_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set hard_timeout in an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param hard_timeout The value to write into the object
 */
void
of_flow_delete_strict_hard_timeout_set(
    of_flow_delete_strict_t *obj,
    uint16_t hard_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 60;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, hard_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get priority from an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param priority Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_delete_strict_priority_get(
    of_flow_delete_strict_t *obj,
    uint16_t *priority)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 62;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, priority);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set priority in an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param priority The value to write into the object
 */
void
of_flow_delete_strict_priority_set(
    of_flow_delete_strict_t *obj,
    uint16_t priority)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 62;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, priority);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get buffer_id from an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param buffer_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_delete_strict_buffer_id_get(
    of_flow_delete_strict_t *obj,
    uint32_t *buffer_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 64;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, buffer_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set buffer_id in an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param buffer_id The value to write into the object
 */
void
of_flow_delete_strict_buffer_id_set(
    of_flow_delete_strict_t *obj,
    uint32_t buffer_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 64;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, buffer_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get out_port from an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param out_port Pointer to the child object of type
 * of_port_no_t to be filled out.
 *
 */
void
of_flow_delete_strict_out_port_get(
    of_flow_delete_strict_t *obj,
    of_port_no_t *out_port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 68;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_port_no_get(ver, wbuf, abs_offset, out_port);
    OF_PORT_NO_VALUE_CHECK(*out_port, ver);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set out_port in an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param out_port The value to write into the object
 */
void
of_flow_delete_strict_out_port_set(
    of_flow_delete_strict_t *obj,
    of_port_no_t out_port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 68;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_port_no_set(ver, wbuf, abs_offset, out_port);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get out_group from an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param out_group Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_delete_strict_out_group_get(
    of_flow_delete_strict_t *obj,
    uint32_t *out_group)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, out_group);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set out_group in an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param out_group The value to write into the object
 */
void
of_flow_delete_strict_out_group_set(
    of_flow_delete_strict_t *obj,
    uint32_t out_group)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, out_group);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_delete_strict_flags_get(
    of_flow_delete_strict_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 70;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 44;
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
 * Set flags in an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param flags The value to write into the object
 */
void
of_flow_delete_strict_flags_set(
    of_flow_delete_strict_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 70;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 44;
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
 * Get match from an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param match Pointer to the child object of type
 * of_match_t to be filled out.
 *
 */
int WARN_UNUSED_RESULT
of_flow_delete_strict_match_get(
    of_flow_delete_strict_t *obj,
    of_match_t *match)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    of_octets_t match_octets; /* Serialized string for match */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 8;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    LOCI_ASSERT(cur_len + abs_offset <= WBUF_CURRENT_BYTES(wbuf));
    match_octets.bytes = cur_len;
    match_octets.data = OF_OBJECT_BUFFER_INDEX(obj, offset);
    OF_TRY(of_match_deserialize(ver, match, &match_octets));

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}

/**
 * Set match in an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param match Pointer to the child of type of_match_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_delete_strict_match_set(
    of_flow_delete_strict_t *obj,
    of_match_t *match)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */
    of_octets_t match_octets; /* Serialized string for match */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 8;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Match object */
    OF_TRY(of_match_serialize(ver, match, &match_octets));
    new_len = match_octets.bytes;
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        match_octets.data, new_len);
    /* Free match serialized octets */
    FREE(match_octets.data);

    /* Not scalar, update lengths if needed */
    delta = new_len - cur_len;
    if (delta != 0) {
        /* Update parent(s) */
        of_object_parent_length_update((of_object_t *)obj, delta);
    }

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}

/**
 * Bind an object of type of_list_instruction_t to the parent of type of_flow_delete_strict for
 * member instructions
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param instructions Pointer to the child object of type
 * of_list_instruction_t to be filled out.
 * \ingroup of_flow_delete_strict
 *
 * The parameter instructions is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_flow_delete_strict_instructions_bind(
    of_flow_delete_strict_t *obj,
    of_list_instruction_t *instructions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
        offset = 136;
        cur_len = _END_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = _FLOW_DELETE_STRICT_INSTRUCTIONS_OFFSET(obj);
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_instruction_init(instructions, obj->version, 0, 1);
    /* Attach to parent */
    instructions->parent = (of_object_t *)obj;
    instructions->wire_object.wbuf = obj->wire_object.wbuf;
    instructions->wire_object.obj_offset = abs_offset;
    instructions->wire_object.owned = 0;
    instructions->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of instructions into a new variable of type of_list_instruction_t from
 * a of_flow_delete_strict instance.
 *
 * @param obj Pointer to the source of type of_flow_delete_strict_t
 * @returns A pointer to a new instance of type of_list_instruction_t whose contents
 * match that of instructions from source
 * @returns NULL if an error occurs
 */
of_list_instruction_t *
of_flow_delete_strict_instructions_get(of_flow_delete_strict_t *obj) {
    of_list_instruction_t _instructions;
    of_list_instruction_t *_instructions_ptr;

    of_flow_delete_strict_instructions_bind(obj, &_instructions);
    _instructions_ptr = (of_list_instruction_t *)of_object_dup(&_instructions);
    return _instructions_ptr;
}

/**
 * Set instructions in an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param instructions Pointer to the child of type of_list_instruction_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_delete_strict_instructions_set(
    of_flow_delete_strict_t *obj,
    of_list_instruction_t *instructions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
        offset = 136;
        cur_len = _END_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = _FLOW_DELETE_STRICT_INSTRUCTIONS_OFFSET(obj);
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = instructions->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == instructions->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(instructions, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(instructions, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (instructions->wire_length_set != NULL) {
        instructions->wire_length_set((of_object_t *)instructions, instructions->length);
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

/**
 * Bind an object of type of_list_action_t to the parent of type of_flow_delete_strict for
 * member actions
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param actions Pointer to the child object of type
 * of_list_action_t to be filled out.
 * \ingroup of_flow_delete_strict
 *
 * The parameter actions is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_flow_delete_strict_actions_bind(
    of_flow_delete_strict_t *obj,
    of_list_action_t *actions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 72;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_action_init(actions, obj->version, 0, 1);
    /* Attach to parent */
    actions->parent = (of_object_t *)obj;
    actions->wire_object.wbuf = obj->wire_object.wbuf;
    actions->wire_object.obj_offset = abs_offset;
    actions->wire_object.owned = 0;
    actions->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of actions into a new variable of type of_list_action_t from
 * a of_flow_delete_strict instance.
 *
 * @param obj Pointer to the source of type of_flow_delete_strict_t
 * @returns A pointer to a new instance of type of_list_action_t whose contents
 * match that of actions from source
 * @returns NULL if an error occurs
 */
of_list_action_t *
of_flow_delete_strict_actions_get(of_flow_delete_strict_t *obj) {
    of_list_action_t _actions;
    of_list_action_t *_actions_ptr;

    of_flow_delete_strict_actions_bind(obj, &_actions);
    _actions_ptr = (of_list_action_t *)of_object_dup(&_actions);
    return _actions_ptr;
}

/**
 * Set actions in an object of type of_flow_delete_strict.
 * @param obj Pointer to an object of type of_flow_delete_strict.
 * @param actions Pointer to the child of type of_list_action_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_delete_strict_actions_set(
    of_flow_delete_strict_t *obj,
    of_list_action_t *actions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 72;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = actions->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == actions->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(actions, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(actions, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (actions->wire_length_set != NULL) {
        actions->wire_length_set((of_object_t *)actions, actions->length);
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
of_flow_mod_failed_error_msg_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x1; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0x3); /* err_type */
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x1; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0x5); /* err_type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_flow_mod_failed_error_msg of_flow_mod_failed_error_msg
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_flow_mod_failed_error_msg_push_wire_values(of_flow_mod_failed_error_msg_t *obj)
{

    of_flow_mod_failed_error_msg_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_flow_mod_failed_error_msg object
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
 * \ingroup of_flow_mod_failed_error_msg
 */

of_flow_mod_failed_error_msg_t *
of_flow_mod_failed_error_msg_new(of_version_t version)
{
    of_flow_mod_failed_error_msg_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_FLOW_MOD_FAILED_ERROR_MSG] + of_object_extra_len[version][OF_FLOW_MOD_FAILED_ERROR_MSG];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_flow_mod_failed_error_msg_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_flow_mod_failed_error_msg_init(obj, version, bytes, 0);

    if (of_flow_mod_failed_error_msg_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_flow_mod_failed_error_msg.
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
of_flow_mod_failed_error_msg_init(of_flow_mod_failed_error_msg_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_FLOW_MOD_FAILED_ERROR_MSG] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_FLOW_MOD_FAILED_ERROR_MSG] + of_object_extra_len[version][OF_FLOW_MOD_FAILED_ERROR_MSG];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_FLOW_MOD_FAILED_ERROR_MSG;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_flow_mod_failed_error_msg_push_wire_types;

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
 * Create a new of_flow_mod_failed_error_msg object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_flow_mod_failed_error_msg
 */

of_flow_mod_failed_error_msg_t *
of_flow_mod_failed_error_msg_new_from_message(of_message_t msg)
{
    of_flow_mod_failed_error_msg_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_flow_mod_failed_error_msg_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_flow_mod_failed_error_msg_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_flow_mod_failed_error_msg.
 * @param obj Pointer to an object of type of_flow_mod_failed_error_msg.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_mod_failed_error_msg_xid_get(
    of_flow_mod_failed_error_msg_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD_FAILED_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_flow_mod_failed_error_msg.
 * @param obj Pointer to an object of type of_flow_mod_failed_error_msg.
 * @param xid The value to write into the object
 */
void
of_flow_mod_failed_error_msg_xid_set(
    of_flow_mod_failed_error_msg_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD_FAILED_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get code from an object of type of_flow_mod_failed_error_msg.
 * @param obj Pointer to an object of type of_flow_mod_failed_error_msg.
 * @param code Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_mod_failed_error_msg_code_get(
    of_flow_mod_failed_error_msg_t *obj,
    uint16_t *code)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD_FAILED_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set code in an object of type of_flow_mod_failed_error_msg.
 * @param obj Pointer to an object of type of_flow_mod_failed_error_msg.
 * @param code The value to write into the object
 */
void
of_flow_mod_failed_error_msg_code_set(
    of_flow_mod_failed_error_msg_t *obj,
    uint16_t code)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD_FAILED_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get data from an object of type of_flow_mod_failed_error_msg.
 * @param obj Pointer to an object of type of_flow_mod_failed_error_msg.
 * @param data Pointer to the child object of type
 * of_octets_t to be filled out.
 *
 */
void
of_flow_mod_failed_error_msg_data_get(
    of_flow_mod_failed_error_msg_t *obj,
    of_octets_t *data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD_FAILED_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set data in an object of type of_flow_mod_failed_error_msg.
 * @param obj Pointer to an object of type of_flow_mod_failed_error_msg.
 * @param data The value to write into the object
 */
int WARN_UNUSED_RESULT
of_flow_mod_failed_error_msg_data_set(
    of_flow_mod_failed_error_msg_t *obj,
    of_octets_t *data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_FLOW_MOD_FAILED_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
of_flow_modify_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0xe; /* type */
        *(uint16_t *)(buf + 56) = U16_HTON(0x1); /* _command */
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0xe; /* type */
        *(uint8_t *)(buf + 25) = 0x1; /* _command */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_flow_modify of_flow_modify
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_flow_modify_push_wire_values(of_flow_modify_t *obj)
{

    of_flow_modify_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_flow_modify object
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
 * \ingroup of_flow_modify
 */

of_flow_modify_t *
of_flow_modify_new(of_version_t version)
{
    of_flow_modify_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_FLOW_MODIFY] + of_object_extra_len[version][OF_FLOW_MODIFY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_flow_modify_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_flow_modify_init(obj, version, bytes, 0);

    if (of_flow_modify_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    /* Initialize match TLV for 1.2 */
    /* FIXME: Check 1.3 below */
    if ((version == OF_VERSION_1_2) || (version == OF_VERSION_1_3)) {
        of_object_u16_set((of_object_t *)obj, 48 + 2, 4);
    }

    return obj;
}

/**
 * Initialize an object of type of_flow_modify.
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
of_flow_modify_init(of_flow_modify_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_FLOW_MODIFY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_FLOW_MODIFY] + of_object_extra_len[version][OF_FLOW_MODIFY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_FLOW_MODIFY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_flow_modify_push_wire_types;

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
 * Create a new of_flow_modify object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_flow_modify
 */

of_flow_modify_t *
of_flow_modify_new_from_message(of_message_t msg)
{
    of_flow_modify_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_flow_modify_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_flow_modify_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_modify_xid_get(
    of_flow_modify_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param xid The value to write into the object
 */
void
of_flow_modify_xid_set(
    of_flow_modify_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get cookie from an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param cookie Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_flow_modify_cookie_get(
    of_flow_modify_t *obj,
    uint64_t *cookie)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 48;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, cookie);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set cookie in an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param cookie The value to write into the object
 */
void
of_flow_modify_cookie_set(
    of_flow_modify_t *obj,
    uint64_t cookie)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 48;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, cookie);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get cookie_mask from an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param cookie_mask Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_flow_modify_cookie_mask_get(
    of_flow_modify_t *obj,
    uint64_t *cookie_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, cookie_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set cookie_mask in an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param cookie_mask The value to write into the object
 */
void
of_flow_modify_cookie_mask_set(
    of_flow_modify_t *obj,
    uint64_t cookie_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, cookie_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get table_id from an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param table_id Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_flow_modify_table_id_get(
    of_flow_modify_t *obj,
    uint8_t *table_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
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
 * Set table_id in an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param table_id The value to write into the object
 */
void
of_flow_modify_table_id_set(
    of_flow_modify_t *obj,
    uint8_t table_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
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
 * Get idle_timeout from an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param idle_timeout Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_modify_idle_timeout_get(
    of_flow_modify_t *obj,
    uint16_t *idle_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 58;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, idle_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set idle_timeout in an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param idle_timeout The value to write into the object
 */
void
of_flow_modify_idle_timeout_set(
    of_flow_modify_t *obj,
    uint16_t idle_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 58;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, idle_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get hard_timeout from an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param hard_timeout Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_modify_hard_timeout_get(
    of_flow_modify_t *obj,
    uint16_t *hard_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 60;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, hard_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set hard_timeout in an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param hard_timeout The value to write into the object
 */
void
of_flow_modify_hard_timeout_set(
    of_flow_modify_t *obj,
    uint16_t hard_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 60;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, hard_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get priority from an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param priority Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_modify_priority_get(
    of_flow_modify_t *obj,
    uint16_t *priority)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 62;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, priority);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set priority in an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param priority The value to write into the object
 */
void
of_flow_modify_priority_set(
    of_flow_modify_t *obj,
    uint16_t priority)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 62;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, priority);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get buffer_id from an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param buffer_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_modify_buffer_id_get(
    of_flow_modify_t *obj,
    uint32_t *buffer_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 64;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, buffer_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set buffer_id in an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param buffer_id The value to write into the object
 */
void
of_flow_modify_buffer_id_set(
    of_flow_modify_t *obj,
    uint32_t buffer_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 64;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, buffer_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get out_port from an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param out_port Pointer to the child object of type
 * of_port_no_t to be filled out.
 *
 */
void
of_flow_modify_out_port_get(
    of_flow_modify_t *obj,
    of_port_no_t *out_port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 68;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_port_no_get(ver, wbuf, abs_offset, out_port);
    OF_PORT_NO_VALUE_CHECK(*out_port, ver);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set out_port in an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param out_port The value to write into the object
 */
void
of_flow_modify_out_port_set(
    of_flow_modify_t *obj,
    of_port_no_t out_port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 68;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_port_no_set(ver, wbuf, abs_offset, out_port);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get out_group from an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param out_group Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_modify_out_group_get(
    of_flow_modify_t *obj,
    uint32_t *out_group)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, out_group);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set out_group in an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param out_group The value to write into the object
 */
void
of_flow_modify_out_group_set(
    of_flow_modify_t *obj,
    uint32_t out_group)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, out_group);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_modify_flags_get(
    of_flow_modify_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 70;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 44;
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
 * Set flags in an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param flags The value to write into the object
 */
void
of_flow_modify_flags_set(
    of_flow_modify_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 70;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 44;
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
 * Get match from an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param match Pointer to the child object of type
 * of_match_t to be filled out.
 *
 */
int WARN_UNUSED_RESULT
of_flow_modify_match_get(
    of_flow_modify_t *obj,
    of_match_t *match)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    of_octets_t match_octets; /* Serialized string for match */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 8;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    LOCI_ASSERT(cur_len + abs_offset <= WBUF_CURRENT_BYTES(wbuf));
    match_octets.bytes = cur_len;
    match_octets.data = OF_OBJECT_BUFFER_INDEX(obj, offset);
    OF_TRY(of_match_deserialize(ver, match, &match_octets));

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}

/**
 * Set match in an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param match Pointer to the child of type of_match_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_modify_match_set(
    of_flow_modify_t *obj,
    of_match_t *match)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */
    of_octets_t match_octets; /* Serialized string for match */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 8;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Match object */
    OF_TRY(of_match_serialize(ver, match, &match_octets));
    new_len = match_octets.bytes;
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        match_octets.data, new_len);
    /* Free match serialized octets */
    FREE(match_octets.data);

    /* Not scalar, update lengths if needed */
    delta = new_len - cur_len;
    if (delta != 0) {
        /* Update parent(s) */
        of_object_parent_length_update((of_object_t *)obj, delta);
    }

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}

/**
 * Bind an object of type of_list_instruction_t to the parent of type of_flow_modify for
 * member instructions
 * @param obj Pointer to an object of type of_flow_modify.
 * @param instructions Pointer to the child object of type
 * of_list_instruction_t to be filled out.
 * \ingroup of_flow_modify
 *
 * The parameter instructions is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_flow_modify_instructions_bind(
    of_flow_modify_t *obj,
    of_list_instruction_t *instructions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
        offset = 136;
        cur_len = _END_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = _FLOW_MODIFY_INSTRUCTIONS_OFFSET(obj);
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_instruction_init(instructions, obj->version, 0, 1);
    /* Attach to parent */
    instructions->parent = (of_object_t *)obj;
    instructions->wire_object.wbuf = obj->wire_object.wbuf;
    instructions->wire_object.obj_offset = abs_offset;
    instructions->wire_object.owned = 0;
    instructions->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of instructions into a new variable of type of_list_instruction_t from
 * a of_flow_modify instance.
 *
 * @param obj Pointer to the source of type of_flow_modify_t
 * @returns A pointer to a new instance of type of_list_instruction_t whose contents
 * match that of instructions from source
 * @returns NULL if an error occurs
 */
of_list_instruction_t *
of_flow_modify_instructions_get(of_flow_modify_t *obj) {
    of_list_instruction_t _instructions;
    of_list_instruction_t *_instructions_ptr;

    of_flow_modify_instructions_bind(obj, &_instructions);
    _instructions_ptr = (of_list_instruction_t *)of_object_dup(&_instructions);
    return _instructions_ptr;
}

/**
 * Set instructions in an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param instructions Pointer to the child of type of_list_instruction_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_modify_instructions_set(
    of_flow_modify_t *obj,
    of_list_instruction_t *instructions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
        offset = 136;
        cur_len = _END_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = _FLOW_MODIFY_INSTRUCTIONS_OFFSET(obj);
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = instructions->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == instructions->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(instructions, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(instructions, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (instructions->wire_length_set != NULL) {
        instructions->wire_length_set((of_object_t *)instructions, instructions->length);
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

/**
 * Bind an object of type of_list_action_t to the parent of type of_flow_modify for
 * member actions
 * @param obj Pointer to an object of type of_flow_modify.
 * @param actions Pointer to the child object of type
 * of_list_action_t to be filled out.
 * \ingroup of_flow_modify
 *
 * The parameter actions is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_flow_modify_actions_bind(
    of_flow_modify_t *obj,
    of_list_action_t *actions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 72;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_action_init(actions, obj->version, 0, 1);
    /* Attach to parent */
    actions->parent = (of_object_t *)obj;
    actions->wire_object.wbuf = obj->wire_object.wbuf;
    actions->wire_object.obj_offset = abs_offset;
    actions->wire_object.owned = 0;
    actions->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of actions into a new variable of type of_list_action_t from
 * a of_flow_modify instance.
 *
 * @param obj Pointer to the source of type of_flow_modify_t
 * @returns A pointer to a new instance of type of_list_action_t whose contents
 * match that of actions from source
 * @returns NULL if an error occurs
 */
of_list_action_t *
of_flow_modify_actions_get(of_flow_modify_t *obj) {
    of_list_action_t _actions;
    of_list_action_t *_actions_ptr;

    of_flow_modify_actions_bind(obj, &_actions);
    _actions_ptr = (of_list_action_t *)of_object_dup(&_actions);
    return _actions_ptr;
}

/**
 * Set actions in an object of type of_flow_modify.
 * @param obj Pointer to an object of type of_flow_modify.
 * @param actions Pointer to the child of type of_list_action_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_modify_actions_set(
    of_flow_modify_t *obj,
    of_list_action_t *actions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 72;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = actions->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == actions->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(actions, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(actions, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (actions->wire_length_set != NULL) {
        actions->wire_length_set((of_object_t *)actions, actions->length);
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
of_flow_modify_strict_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0xe; /* type */
        *(uint16_t *)(buf + 56) = U16_HTON(0x2); /* _command */
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0xe; /* type */
        *(uint8_t *)(buf + 25) = 0x2; /* _command */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_flow_modify_strict of_flow_modify_strict
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_flow_modify_strict_push_wire_values(of_flow_modify_strict_t *obj)
{

    of_flow_modify_strict_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_flow_modify_strict object
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
 * \ingroup of_flow_modify_strict
 */

of_flow_modify_strict_t *
of_flow_modify_strict_new(of_version_t version)
{
    of_flow_modify_strict_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_FLOW_MODIFY_STRICT] + of_object_extra_len[version][OF_FLOW_MODIFY_STRICT];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_flow_modify_strict_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_flow_modify_strict_init(obj, version, bytes, 0);

    if (of_flow_modify_strict_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    /* Initialize match TLV for 1.2 */
    /* FIXME: Check 1.3 below */
    if ((version == OF_VERSION_1_2) || (version == OF_VERSION_1_3)) {
        of_object_u16_set((of_object_t *)obj, 48 + 2, 4);
    }

    return obj;
}

/**
 * Initialize an object of type of_flow_modify_strict.
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
of_flow_modify_strict_init(of_flow_modify_strict_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_FLOW_MODIFY_STRICT] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_FLOW_MODIFY_STRICT] + of_object_extra_len[version][OF_FLOW_MODIFY_STRICT];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_FLOW_MODIFY_STRICT;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_flow_modify_strict_push_wire_types;

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
 * Create a new of_flow_modify_strict object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_flow_modify_strict
 */

of_flow_modify_strict_t *
of_flow_modify_strict_new_from_message(of_message_t msg)
{
    of_flow_modify_strict_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_flow_modify_strict_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_flow_modify_strict_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_modify_strict_xid_get(
    of_flow_modify_strict_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param xid The value to write into the object
 */
void
of_flow_modify_strict_xid_set(
    of_flow_modify_strict_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get cookie from an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param cookie Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_flow_modify_strict_cookie_get(
    of_flow_modify_strict_t *obj,
    uint64_t *cookie)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 48;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, cookie);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set cookie in an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param cookie The value to write into the object
 */
void
of_flow_modify_strict_cookie_set(
    of_flow_modify_strict_t *obj,
    uint64_t cookie)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 48;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, cookie);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get cookie_mask from an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param cookie_mask Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_flow_modify_strict_cookie_mask_get(
    of_flow_modify_strict_t *obj,
    uint64_t *cookie_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, cookie_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set cookie_mask in an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param cookie_mask The value to write into the object
 */
void
of_flow_modify_strict_cookie_mask_set(
    of_flow_modify_strict_t *obj,
    uint64_t cookie_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, cookie_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get table_id from an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param table_id Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_flow_modify_strict_table_id_get(
    of_flow_modify_strict_t *obj,
    uint8_t *table_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
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
 * Set table_id in an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param table_id The value to write into the object
 */
void
of_flow_modify_strict_table_id_set(
    of_flow_modify_strict_t *obj,
    uint8_t table_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
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
 * Get idle_timeout from an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param idle_timeout Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_modify_strict_idle_timeout_get(
    of_flow_modify_strict_t *obj,
    uint16_t *idle_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 58;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, idle_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set idle_timeout in an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param idle_timeout The value to write into the object
 */
void
of_flow_modify_strict_idle_timeout_set(
    of_flow_modify_strict_t *obj,
    uint16_t idle_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 58;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 26;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, idle_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get hard_timeout from an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param hard_timeout Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_modify_strict_hard_timeout_get(
    of_flow_modify_strict_t *obj,
    uint16_t *hard_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 60;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, hard_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set hard_timeout in an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param hard_timeout The value to write into the object
 */
void
of_flow_modify_strict_hard_timeout_set(
    of_flow_modify_strict_t *obj,
    uint16_t hard_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 60;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, hard_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get priority from an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param priority Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_modify_strict_priority_get(
    of_flow_modify_strict_t *obj,
    uint16_t *priority)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 62;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, priority);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set priority in an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param priority The value to write into the object
 */
void
of_flow_modify_strict_priority_set(
    of_flow_modify_strict_t *obj,
    uint16_t priority)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 62;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, priority);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get buffer_id from an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param buffer_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_modify_strict_buffer_id_get(
    of_flow_modify_strict_t *obj,
    uint32_t *buffer_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 64;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, buffer_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set buffer_id in an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param buffer_id The value to write into the object
 */
void
of_flow_modify_strict_buffer_id_set(
    of_flow_modify_strict_t *obj,
    uint32_t buffer_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 64;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, buffer_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get out_port from an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param out_port Pointer to the child object of type
 * of_port_no_t to be filled out.
 *
 */
void
of_flow_modify_strict_out_port_get(
    of_flow_modify_strict_t *obj,
    of_port_no_t *out_port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 68;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_port_no_get(ver, wbuf, abs_offset, out_port);
    OF_PORT_NO_VALUE_CHECK(*out_port, ver);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set out_port in an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param out_port The value to write into the object
 */
void
of_flow_modify_strict_out_port_set(
    of_flow_modify_strict_t *obj,
    of_port_no_t out_port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 68;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_port_no_set(ver, wbuf, abs_offset, out_port);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get out_group from an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param out_group Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_modify_strict_out_group_get(
    of_flow_modify_strict_t *obj,
    uint32_t *out_group)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, out_group);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set out_group in an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param out_group The value to write into the object
 */
void
of_flow_modify_strict_out_group_set(
    of_flow_modify_strict_t *obj,
    uint32_t out_group)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, out_group);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_modify_strict_flags_get(
    of_flow_modify_strict_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 70;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 44;
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
 * Set flags in an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param flags The value to write into the object
 */
void
of_flow_modify_strict_flags_set(
    of_flow_modify_strict_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 70;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 44;
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
 * Get match from an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param match Pointer to the child object of type
 * of_match_t to be filled out.
 *
 */
int WARN_UNUSED_RESULT
of_flow_modify_strict_match_get(
    of_flow_modify_strict_t *obj,
    of_match_t *match)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    of_octets_t match_octets; /* Serialized string for match */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 8;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    LOCI_ASSERT(cur_len + abs_offset <= WBUF_CURRENT_BYTES(wbuf));
    match_octets.bytes = cur_len;
    match_octets.data = OF_OBJECT_BUFFER_INDEX(obj, offset);
    OF_TRY(of_match_deserialize(ver, match, &match_octets));

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}

/**
 * Set match in an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param match Pointer to the child of type of_match_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_modify_strict_match_set(
    of_flow_modify_strict_t *obj,
    of_match_t *match)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */
    of_octets_t match_octets; /* Serialized string for match */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 8;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Match object */
    OF_TRY(of_match_serialize(ver, match, &match_octets));
    new_len = match_octets.bytes;
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        match_octets.data, new_len);
    /* Free match serialized octets */
    FREE(match_octets.data);

    /* Not scalar, update lengths if needed */
    delta = new_len - cur_len;
    if (delta != 0) {
        /* Update parent(s) */
        of_object_parent_length_update((of_object_t *)obj, delta);
    }

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}

/**
 * Bind an object of type of_list_instruction_t to the parent of type of_flow_modify_strict for
 * member instructions
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param instructions Pointer to the child object of type
 * of_list_instruction_t to be filled out.
 * \ingroup of_flow_modify_strict
 *
 * The parameter instructions is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_flow_modify_strict_instructions_bind(
    of_flow_modify_strict_t *obj,
    of_list_instruction_t *instructions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
        offset = 136;
        cur_len = _END_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = _FLOW_MODIFY_STRICT_INSTRUCTIONS_OFFSET(obj);
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_instruction_init(instructions, obj->version, 0, 1);
    /* Attach to parent */
    instructions->parent = (of_object_t *)obj;
    instructions->wire_object.wbuf = obj->wire_object.wbuf;
    instructions->wire_object.obj_offset = abs_offset;
    instructions->wire_object.owned = 0;
    instructions->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of instructions into a new variable of type of_list_instruction_t from
 * a of_flow_modify_strict instance.
 *
 * @param obj Pointer to the source of type of_flow_modify_strict_t
 * @returns A pointer to a new instance of type of_list_instruction_t whose contents
 * match that of instructions from source
 * @returns NULL if an error occurs
 */
of_list_instruction_t *
of_flow_modify_strict_instructions_get(of_flow_modify_strict_t *obj) {
    of_list_instruction_t _instructions;
    of_list_instruction_t *_instructions_ptr;

    of_flow_modify_strict_instructions_bind(obj, &_instructions);
    _instructions_ptr = (of_list_instruction_t *)of_object_dup(&_instructions);
    return _instructions_ptr;
}

/**
 * Set instructions in an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param instructions Pointer to the child of type of_list_instruction_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_modify_strict_instructions_set(
    of_flow_modify_strict_t *obj,
    of_list_instruction_t *instructions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
        offset = 136;
        cur_len = _END_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = _FLOW_MODIFY_STRICT_INSTRUCTIONS_OFFSET(obj);
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = instructions->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == instructions->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(instructions, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(instructions, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (instructions->wire_length_set != NULL) {
        instructions->wire_length_set((of_object_t *)instructions, instructions->length);
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

/**
 * Bind an object of type of_list_action_t to the parent of type of_flow_modify_strict for
 * member actions
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param actions Pointer to the child object of type
 * of_list_action_t to be filled out.
 * \ingroup of_flow_modify_strict
 *
 * The parameter actions is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_flow_modify_strict_actions_bind(
    of_flow_modify_strict_t *obj,
    of_list_action_t *actions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 72;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_action_init(actions, obj->version, 0, 1);
    /* Attach to parent */
    actions->parent = (of_object_t *)obj;
    actions->wire_object.wbuf = obj->wire_object.wbuf;
    actions->wire_object.obj_offset = abs_offset;
    actions->wire_object.owned = 0;
    actions->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of actions into a new variable of type of_list_action_t from
 * a of_flow_modify_strict instance.
 *
 * @param obj Pointer to the source of type of_flow_modify_strict_t
 * @returns A pointer to a new instance of type of_list_action_t whose contents
 * match that of actions from source
 * @returns NULL if an error occurs
 */
of_list_action_t *
of_flow_modify_strict_actions_get(of_flow_modify_strict_t *obj) {
    of_list_action_t _actions;
    of_list_action_t *_actions_ptr;

    of_flow_modify_strict_actions_bind(obj, &_actions);
    _actions_ptr = (of_list_action_t *)of_object_dup(&_actions);
    return _actions_ptr;
}

/**
 * Set actions in an object of type of_flow_modify_strict.
 * @param obj Pointer to an object of type of_flow_modify_strict.
 * @param actions Pointer to the child of type of_list_action_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_modify_strict_actions_set(
    of_flow_modify_strict_t *obj,
    of_list_action_t *actions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(IS_FLOW_MOD_SUBTYPE(obj->object_id));
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 72;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = actions->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == actions->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(actions, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(actions, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (actions->wire_length_set != NULL) {
        actions->wire_length_set((of_object_t *)actions, actions->length);
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
of_flow_removed_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0xb; /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_flow_removed of_flow_removed
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_flow_removed_push_wire_values(of_flow_removed_t *obj)
{

    of_flow_removed_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_flow_removed object
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
 * \ingroup of_flow_removed
 */

of_flow_removed_t *
of_flow_removed_new(of_version_t version)
{
    of_flow_removed_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_FLOW_REMOVED] + of_object_extra_len[version][OF_FLOW_REMOVED];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_flow_removed_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_flow_removed_init(obj, version, bytes, 0);

    if (of_flow_removed_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    /* Initialize match TLV for 1.2 */
    /* FIXME: Check 1.3 below */
    if ((version == OF_VERSION_1_2) || (version == OF_VERSION_1_3)) {
        of_object_u16_set((of_object_t *)obj, 48 + 2, 4);
    }

    return obj;
}

/**
 * Initialize an object of type of_flow_removed.
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
of_flow_removed_init(of_flow_removed_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_FLOW_REMOVED] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_FLOW_REMOVED] + of_object_extra_len[version][OF_FLOW_REMOVED];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_FLOW_REMOVED;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_flow_removed_push_wire_types;

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
 * Create a new of_flow_removed object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_flow_removed
 */

of_flow_removed_t *
of_flow_removed_new_from_message(of_message_t msg)
{
    of_flow_removed_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_flow_removed_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_flow_removed_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_removed_xid_get(
    of_flow_removed_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param xid The value to write into the object
 */
void
of_flow_removed_xid_set(
    of_flow_removed_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get cookie from an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param cookie Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_flow_removed_cookie_get(
    of_flow_removed_t *obj,
    uint64_t *cookie)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 48;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, cookie);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set cookie in an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param cookie The value to write into the object
 */
void
of_flow_removed_cookie_set(
    of_flow_removed_t *obj,
    uint64_t cookie)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 48;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, cookie);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get priority from an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param priority Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_removed_priority_get(
    of_flow_removed_t *obj,
    uint16_t *priority)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 56;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, priority);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set priority in an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param priority The value to write into the object
 */
void
of_flow_removed_priority_set(
    of_flow_removed_t *obj,
    uint16_t priority)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 56;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, priority);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get reason from an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param reason Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_flow_removed_reason_get(
    of_flow_removed_t *obj,
    uint8_t *reason)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 58;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 18;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, reason);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set reason in an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param reason The value to write into the object
 */
void
of_flow_removed_reason_set(
    of_flow_removed_t *obj,
    uint8_t reason)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 58;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 18;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, reason);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get table_id from an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param table_id Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_flow_removed_table_id_get(
    of_flow_removed_t *obj,
    uint8_t *table_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 19;
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
 * Set table_id in an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param table_id The value to write into the object
 */
void
of_flow_removed_table_id_set(
    of_flow_removed_t *obj,
    uint8_t table_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 19;
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
 * Get duration_sec from an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param duration_sec Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_removed_duration_sec_get(
    of_flow_removed_t *obj,
    uint32_t *duration_sec)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 60;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 20;
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
 * Set duration_sec in an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param duration_sec The value to write into the object
 */
void
of_flow_removed_duration_sec_set(
    of_flow_removed_t *obj,
    uint32_t duration_sec)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 60;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 20;
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
 * Get duration_nsec from an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param duration_nsec Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_removed_duration_nsec_get(
    of_flow_removed_t *obj,
    uint32_t *duration_nsec)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 64;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
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
 * Set duration_nsec in an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param duration_nsec The value to write into the object
 */
void
of_flow_removed_duration_nsec_set(
    of_flow_removed_t *obj,
    uint32_t duration_nsec)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 64;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
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
 * Get idle_timeout from an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param idle_timeout Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_removed_idle_timeout_get(
    of_flow_removed_t *obj,
    uint16_t *idle_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 68;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, idle_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set idle_timeout in an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param idle_timeout The value to write into the object
 */
void
of_flow_removed_idle_timeout_set(
    of_flow_removed_t *obj,
    uint16_t idle_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 68;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, idle_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get hard_timeout from an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param hard_timeout Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_removed_hard_timeout_get(
    of_flow_removed_t *obj,
    uint16_t *hard_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, hard_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set hard_timeout in an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param hard_timeout The value to write into the object
 */
void
of_flow_removed_hard_timeout_set(
    of_flow_removed_t *obj,
    uint16_t hard_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 30;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, hard_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get packet_count from an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param packet_count Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_flow_removed_packet_count_get(
    of_flow_removed_t *obj,
    uint64_t *packet_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 72;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, packet_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set packet_count in an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param packet_count The value to write into the object
 */
void
of_flow_removed_packet_count_set(
    of_flow_removed_t *obj,
    uint64_t packet_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 72;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, packet_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get byte_count from an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param byte_count Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_flow_removed_byte_count_get(
    of_flow_removed_t *obj,
    uint64_t *byte_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 80;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, byte_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set byte_count in an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param byte_count The value to write into the object
 */
void
of_flow_removed_byte_count_set(
    of_flow_removed_t *obj,
    uint64_t byte_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 80;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, byte_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get match from an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param match Pointer to the child object of type
 * of_match_t to be filled out.
 *
 */
int WARN_UNUSED_RESULT
of_flow_removed_match_get(
    of_flow_removed_t *obj,
    of_match_t *match)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    of_octets_t match_octets; /* Serialized string for match */

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 8;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    LOCI_ASSERT(cur_len + abs_offset <= WBUF_CURRENT_BYTES(wbuf));
    match_octets.bytes = cur_len;
    match_octets.data = OF_OBJECT_BUFFER_INDEX(obj, offset);
    OF_TRY(of_match_deserialize(ver, match, &match_octets));

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}

/**
 * Set match in an object of type of_flow_removed.
 * @param obj Pointer to an object of type of_flow_removed.
 * @param match Pointer to the child of type of_match_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_removed_match_set(
    of_flow_removed_t *obj,
    of_match_t *match)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */
    of_octets_t match_octets; /* Serialized string for match */

    LOCI_ASSERT(obj->object_id == OF_FLOW_REMOVED);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 8;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Match object */
    OF_TRY(of_match_serialize(ver, match, &match_octets));
    new_len = match_octets.bytes;
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        match_octets.data, new_len);
    /* Free match serialized octets */
    FREE(match_octets.data);

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
 * \defgroup of_flow_stats_entry of_flow_stats_entry
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_flow_stats_entry_push_wire_values(of_flow_stats_entry_t *obj)
{

    obj->wire_length_set((of_object_t *)obj, obj->length);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_flow_stats_entry object
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
 * \ingroup of_flow_stats_entry
 */

of_flow_stats_entry_t *
of_flow_stats_entry_new(of_version_t version)
{
    of_flow_stats_entry_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_FLOW_STATS_ENTRY] + of_object_extra_len[version][OF_FLOW_STATS_ENTRY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_flow_stats_entry_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_flow_stats_entry_init(obj, version, bytes, 0);

    if (of_flow_stats_entry_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    /* Initialize match TLV for 1.2 */
    /* FIXME: Check 1.3 below */
    if ((version == OF_VERSION_1_2) || (version == OF_VERSION_1_3)) {
        of_object_u16_set((of_object_t *)obj, 48 + 2, 4);
    }

    return obj;
}

/**
 * Initialize an object of type of_flow_stats_entry.
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
of_flow_stats_entry_init(of_flow_stats_entry_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_FLOW_STATS_ENTRY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_FLOW_STATS_ENTRY] + of_object_extra_len[version][OF_FLOW_STATS_ENTRY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_FLOW_STATS_ENTRY;

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
 * Get table_id from an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param table_id Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_flow_stats_entry_table_id_get(
    of_flow_stats_entry_t *obj,
    uint8_t *table_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set table_id in an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param table_id The value to write into the object
 */
void
of_flow_stats_entry_table_id_set(
    of_flow_stats_entry_t *obj,
    uint8_t table_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get duration_sec from an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param duration_sec Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_stats_entry_duration_sec_get(
    of_flow_stats_entry_t *obj,
    uint32_t *duration_sec)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 44;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
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
 * Set duration_sec in an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param duration_sec The value to write into the object
 */
void
of_flow_stats_entry_duration_sec_set(
    of_flow_stats_entry_t *obj,
    uint32_t duration_sec)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 44;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 4;
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
 * Get duration_nsec from an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param duration_nsec Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_stats_entry_duration_nsec_get(
    of_flow_stats_entry_t *obj,
    uint32_t *duration_nsec)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 48;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
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
 * Set duration_nsec in an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param duration_nsec The value to write into the object
 */
void
of_flow_stats_entry_duration_nsec_set(
    of_flow_stats_entry_t *obj,
    uint32_t duration_nsec)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 48;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
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
 * Get priority from an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param priority Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_stats_entry_priority_get(
    of_flow_stats_entry_t *obj,
    uint16_t *priority)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 52;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, priority);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set priority in an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param priority The value to write into the object
 */
void
of_flow_stats_entry_priority_set(
    of_flow_stats_entry_t *obj,
    uint16_t priority)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 52;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, priority);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get idle_timeout from an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param idle_timeout Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_stats_entry_idle_timeout_get(
    of_flow_stats_entry_t *obj,
    uint16_t *idle_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 54;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 14;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, idle_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set idle_timeout in an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param idle_timeout The value to write into the object
 */
void
of_flow_stats_entry_idle_timeout_set(
    of_flow_stats_entry_t *obj,
    uint16_t idle_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 54;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 14;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, idle_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get hard_timeout from an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param hard_timeout Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_stats_entry_hard_timeout_get(
    of_flow_stats_entry_t *obj,
    uint16_t *hard_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 56;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, hard_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set hard_timeout in an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param hard_timeout The value to write into the object
 */
void
of_flow_stats_entry_hard_timeout_set(
    of_flow_stats_entry_t *obj,
    uint16_t hard_timeout)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 56;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, hard_timeout);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get flags from an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_stats_entry_flags_get(
    of_flow_stats_entry_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set flags in an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param flags The value to write into the object
 */
void
of_flow_stats_entry_flags_set(
    of_flow_stats_entry_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, flags);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get cookie from an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param cookie Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_flow_stats_entry_cookie_get(
    of_flow_stats_entry_t *obj,
    uint64_t *cookie)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 64;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, cookie);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set cookie in an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param cookie The value to write into the object
 */
void
of_flow_stats_entry_cookie_set(
    of_flow_stats_entry_t *obj,
    uint64_t cookie)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 64;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, cookie);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get packet_count from an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param packet_count Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_flow_stats_entry_packet_count_get(
    of_flow_stats_entry_t *obj,
    uint64_t *packet_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 72;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, packet_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set packet_count in an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param packet_count The value to write into the object
 */
void
of_flow_stats_entry_packet_count_set(
    of_flow_stats_entry_t *obj,
    uint64_t packet_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 72;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, packet_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get byte_count from an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param byte_count Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_flow_stats_entry_byte_count_get(
    of_flow_stats_entry_t *obj,
    uint64_t *byte_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 80;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, byte_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set byte_count in an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param byte_count The value to write into the object
 */
void
of_flow_stats_entry_byte_count_set(
    of_flow_stats_entry_t *obj,
    uint64_t byte_count)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 80;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, byte_count);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get match from an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param match Pointer to the child object of type
 * of_match_t to be filled out.
 *
 */
int WARN_UNUSED_RESULT
of_flow_stats_entry_match_get(
    of_flow_stats_entry_t *obj,
    of_match_t *match)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    of_octets_t match_octets; /* Serialized string for match */

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 4;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    LOCI_ASSERT(cur_len + abs_offset <= WBUF_CURRENT_BYTES(wbuf));
    match_octets.bytes = cur_len;
    match_octets.data = OF_OBJECT_BUFFER_INDEX(obj, offset);
    OF_TRY(of_match_deserialize(ver, match, &match_octets));

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}

/**
 * Set match in an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param match Pointer to the child of type of_match_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_stats_entry_match_set(
    of_flow_stats_entry_t *obj,
    of_match_t *match)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */
    of_octets_t match_octets; /* Serialized string for match */

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 4;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Match object */
    OF_TRY(of_match_serialize(ver, match, &match_octets));
    new_len = match_octets.bytes;
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        match_octets.data, new_len);
    /* Free match serialized octets */
    FREE(match_octets.data);

    /* Not scalar, update lengths if needed */
    delta = new_len - cur_len;
    if (delta != 0) {
        /* Update parent(s) */
        of_object_parent_length_update((of_object_t *)obj, delta);
    }

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}

/**
 * Bind an object of type of_list_instruction_t to the parent of type of_flow_stats_entry for
 * member instructions
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param instructions Pointer to the child object of type
 * of_list_instruction_t to be filled out.
 * \ingroup of_flow_stats_entry
 *
 * The parameter instructions is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_flow_stats_entry_instructions_bind(
    of_flow_stats_entry_t *obj,
    of_list_instruction_t *instructions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
        offset = 136;
        cur_len = _END_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = _FLOW_STATS_ENTRY_INSTRUCTIONS_OFFSET(obj);
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_instruction_init(instructions, obj->version, 0, 1);
    /* Attach to parent */
    instructions->parent = (of_object_t *)obj;
    instructions->wire_object.wbuf = obj->wire_object.wbuf;
    instructions->wire_object.obj_offset = abs_offset;
    instructions->wire_object.owned = 0;
    instructions->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of instructions into a new variable of type of_list_instruction_t from
 * a of_flow_stats_entry instance.
 *
 * @param obj Pointer to the source of type of_flow_stats_entry_t
 * @returns A pointer to a new instance of type of_list_instruction_t whose contents
 * match that of instructions from source
 * @returns NULL if an error occurs
 */
of_list_instruction_t *
of_flow_stats_entry_instructions_get(of_flow_stats_entry_t *obj) {
    of_list_instruction_t _instructions;
    of_list_instruction_t *_instructions_ptr;

    of_flow_stats_entry_instructions_bind(obj, &_instructions);
    _instructions_ptr = (of_list_instruction_t *)of_object_dup(&_instructions);
    return _instructions_ptr;
}

/**
 * Set instructions in an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param instructions Pointer to the child of type of_list_instruction_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_stats_entry_instructions_set(
    of_flow_stats_entry_t *obj,
    of_list_instruction_t *instructions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
        offset = 136;
        cur_len = _END_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = _FLOW_STATS_ENTRY_INSTRUCTIONS_OFFSET(obj);
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = instructions->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == instructions->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(instructions, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(instructions, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (instructions->wire_length_set != NULL) {
        instructions->wire_length_set((of_object_t *)instructions, instructions->length);
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

/**
 * Bind an object of type of_list_action_t to the parent of type of_flow_stats_entry for
 * member actions
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param actions Pointer to the child object of type
 * of_list_action_t to be filled out.
 * \ingroup of_flow_stats_entry
 *
 * The parameter actions is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_flow_stats_entry_actions_bind(
    of_flow_stats_entry_t *obj,
    of_list_action_t *actions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 88;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Initialize child */
    of_list_action_init(actions, obj->version, 0, 1);
    /* Attach to parent */
    actions->parent = (of_object_t *)obj;
    actions->wire_object.wbuf = obj->wire_object.wbuf;
    actions->wire_object.obj_offset = abs_offset;
    actions->wire_object.owned = 0;
    actions->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of actions into a new variable of type of_list_action_t from
 * a of_flow_stats_entry instance.
 *
 * @param obj Pointer to the source of type of_flow_stats_entry_t
 * @returns A pointer to a new instance of type of_list_action_t whose contents
 * match that of actions from source
 * @returns NULL if an error occurs
 */
of_list_action_t *
of_flow_stats_entry_actions_get(of_flow_stats_entry_t *obj) {
    of_list_action_t _actions;
    of_list_action_t *_actions_ptr;

    of_flow_stats_entry_actions_bind(obj, &_actions);
    _actions_ptr = (of_list_action_t *)of_object_dup(&_actions);
    return _actions_ptr;
}

/**
 * Set actions in an object of type of_flow_stats_entry.
 * @param obj Pointer to an object of type of_flow_stats_entry.
 * @param actions Pointer to the child of type of_list_action_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_stats_entry_actions_set(
    of_flow_stats_entry_t *obj,
    of_list_action_t *actions)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_ENTRY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 88;
        cur_len = _END_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* LOCI object type */
    new_len = actions->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == actions->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(actions, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(actions, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (actions->wire_length_set != NULL) {
        actions->wire_length_set((of_object_t *)actions, actions->length);
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
of_flow_stats_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x11; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0x1); /* stats_type */
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0x1); /* stats_type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_flow_stats_reply of_flow_stats_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_flow_stats_reply_push_wire_values(of_flow_stats_reply_t *obj)
{

    of_flow_stats_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_flow_stats_reply object
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
 * \ingroup of_flow_stats_reply
 */

of_flow_stats_reply_t *
of_flow_stats_reply_new(of_version_t version)
{
    of_flow_stats_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_FLOW_STATS_REPLY] + of_object_extra_len[version][OF_FLOW_STATS_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_flow_stats_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_flow_stats_reply_init(obj, version, bytes, 0);

    if (of_flow_stats_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_flow_stats_reply.
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
of_flow_stats_reply_init(of_flow_stats_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_FLOW_STATS_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_FLOW_STATS_REPLY] + of_object_extra_len[version][OF_FLOW_STATS_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_FLOW_STATS_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_flow_stats_reply_push_wire_types;

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
 * Create a new of_flow_stats_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_flow_stats_reply
 */

of_flow_stats_reply_t *
of_flow_stats_reply_new_from_message(of_message_t msg)
{
    of_flow_stats_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_flow_stats_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_flow_stats_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_flow_stats_reply.
 * @param obj Pointer to an object of type of_flow_stats_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_stats_reply_xid_get(
    of_flow_stats_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_flow_stats_reply.
 * @param obj Pointer to an object of type of_flow_stats_reply.
 * @param xid The value to write into the object
 */
void
of_flow_stats_reply_xid_set(
    of_flow_stats_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get flags from an object of type of_flow_stats_reply.
 * @param obj Pointer to an object of type of_flow_stats_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_stats_reply_flags_get(
    of_flow_stats_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set flags in an object of type of_flow_stats_reply.
 * @param obj Pointer to an object of type of_flow_stats_reply.
 * @param flags The value to write into the object
 */
void
of_flow_stats_reply_flags_set(
    of_flow_stats_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Bind an object of type of_list_flow_stats_entry_t to the parent of type of_flow_stats_reply for
 * member entries
 * @param obj Pointer to an object of type of_flow_stats_reply.
 * @param entries Pointer to the child object of type
 * of_list_flow_stats_entry_t to be filled out.
 * \ingroup of_flow_stats_reply
 *
 * The parameter entries is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_flow_stats_reply_entries_bind(
    of_flow_stats_reply_t *obj,
    of_list_flow_stats_entry_t *entries)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 12;
        cur_len = _END_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
    of_list_flow_stats_entry_init(entries, obj->version, 0, 1);
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
 * Create a copy of entries into a new variable of type of_list_flow_stats_entry_t from
 * a of_flow_stats_reply instance.
 *
 * @param obj Pointer to the source of type of_flow_stats_reply_t
 * @returns A pointer to a new instance of type of_list_flow_stats_entry_t whose contents
 * match that of entries from source
 * @returns NULL if an error occurs
 */
of_list_flow_stats_entry_t *
of_flow_stats_reply_entries_get(of_flow_stats_reply_t *obj) {
    of_list_flow_stats_entry_t _entries;
    of_list_flow_stats_entry_t *_entries_ptr;

    of_flow_stats_reply_entries_bind(obj, &_entries);
    _entries_ptr = (of_list_flow_stats_entry_t *)of_object_dup(&_entries);
    return _entries_ptr;
}

/**
 * Set entries in an object of type of_flow_stats_reply.
 * @param obj Pointer to an object of type of_flow_stats_reply.
 * @param entries Pointer to the child of type of_list_flow_stats_entry_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_stats_reply_entries_set(
    of_flow_stats_reply_t *obj,
    of_list_flow_stats_entry_t *entries)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 12;
        cur_len = _END_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
of_flow_stats_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x10; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0x1); /* stats_type */
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0x1); /* stats_type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_flow_stats_request of_flow_stats_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_flow_stats_request_push_wire_values(of_flow_stats_request_t *obj)
{

    of_flow_stats_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_flow_stats_request object
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
 * \ingroup of_flow_stats_request
 */

of_flow_stats_request_t *
of_flow_stats_request_new(of_version_t version)
{
    of_flow_stats_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_FLOW_STATS_REQUEST] + of_object_extra_len[version][OF_FLOW_STATS_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_flow_stats_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_flow_stats_request_init(obj, version, bytes, 0);

    if (of_flow_stats_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    /* Initialize match TLV for 1.2 */
    /* FIXME: Check 1.3 below */
    if ((version == OF_VERSION_1_2) || (version == OF_VERSION_1_3)) {
        of_object_u16_set((of_object_t *)obj, 48 + 2, 4);
    }

    return obj;
}

/**
 * Initialize an object of type of_flow_stats_request.
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
of_flow_stats_request_init(of_flow_stats_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_FLOW_STATS_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_FLOW_STATS_REQUEST] + of_object_extra_len[version][OF_FLOW_STATS_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_FLOW_STATS_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_flow_stats_request_push_wire_types;

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
 * Create a new of_flow_stats_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_flow_stats_request
 */

of_flow_stats_request_t *
of_flow_stats_request_new_from_message(of_message_t msg)
{
    of_flow_stats_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_flow_stats_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_flow_stats_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_flow_stats_request.
 * @param obj Pointer to an object of type of_flow_stats_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_stats_request_xid_get(
    of_flow_stats_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_flow_stats_request.
 * @param obj Pointer to an object of type of_flow_stats_request.
 * @param xid The value to write into the object
 */
void
of_flow_stats_request_xid_set(
    of_flow_stats_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get flags from an object of type of_flow_stats_request.
 * @param obj Pointer to an object of type of_flow_stats_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_flow_stats_request_flags_get(
    of_flow_stats_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set flags in an object of type of_flow_stats_request.
 * @param obj Pointer to an object of type of_flow_stats_request.
 * @param flags The value to write into the object
 */
void
of_flow_stats_request_flags_set(
    of_flow_stats_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get table_id from an object of type of_flow_stats_request.
 * @param obj Pointer to an object of type of_flow_stats_request.
 * @param table_id Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_flow_stats_request_table_id_get(
    of_flow_stats_request_t *obj,
    uint8_t *table_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 52;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
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
 * Set table_id in an object of type of_flow_stats_request.
 * @param obj Pointer to an object of type of_flow_stats_request.
 * @param table_id The value to write into the object
 */
void
of_flow_stats_request_table_id_set(
    of_flow_stats_request_t *obj,
    uint8_t table_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 52;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 16;
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
 * Get out_port from an object of type of_flow_stats_request.
 * @param obj Pointer to an object of type of_flow_stats_request.
 * @param out_port Pointer to the child object of type
 * of_port_no_t to be filled out.
 *
 */
void
of_flow_stats_request_out_port_get(
    of_flow_stats_request_t *obj,
    of_port_no_t *out_port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 54;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_port_no_get(ver, wbuf, abs_offset, out_port);
    OF_PORT_NO_VALUE_CHECK(*out_port, ver);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set out_port in an object of type of_flow_stats_request.
 * @param obj Pointer to an object of type of_flow_stats_request.
 * @param out_port The value to write into the object
 */
void
of_flow_stats_request_out_port_set(
    of_flow_stats_request_t *obj,
    of_port_no_t out_port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 54;
        break;
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 20;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_port_no_set(ver, wbuf, abs_offset, out_port);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get out_group from an object of type of_flow_stats_request.
 * @param obj Pointer to an object of type of_flow_stats_request.
 * @param out_group Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_flow_stats_request_out_group_get(
    of_flow_stats_request_t *obj,
    uint32_t *out_group)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, out_group);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set out_group in an object of type of_flow_stats_request.
 * @param obj Pointer to an object of type of_flow_stats_request.
 * @param out_group The value to write into the object
 */
void
of_flow_stats_request_out_group_set(
    of_flow_stats_request_t *obj,
    uint32_t out_group)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, out_group);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get cookie from an object of type of_flow_stats_request.
 * @param obj Pointer to an object of type of_flow_stats_request.
 * @param cookie Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_flow_stats_request_cookie_get(
    of_flow_stats_request_t *obj,
    uint64_t *cookie)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, cookie);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set cookie in an object of type of_flow_stats_request.
 * @param obj Pointer to an object of type of_flow_stats_request.
 * @param cookie The value to write into the object
 */
void
of_flow_stats_request_cookie_set(
    of_flow_stats_request_t *obj,
    uint64_t cookie)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, cookie);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get cookie_mask from an object of type of_flow_stats_request.
 * @param obj Pointer to an object of type of_flow_stats_request.
 * @param cookie_mask Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_flow_stats_request_cookie_mask_get(
    of_flow_stats_request_t *obj,
    uint64_t *cookie_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_get(wbuf, abs_offset, cookie_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set cookie_mask in an object of type of_flow_stats_request.
 * @param obj Pointer to an object of type of_flow_stats_request.
 * @param cookie_mask The value to write into the object
 */
void
of_flow_stats_request_cookie_mask_set(
    of_flow_stats_request_t *obj,
    uint64_t cookie_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 40;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u64_set(wbuf, abs_offset, cookie_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get match from an object of type of_flow_stats_request.
 * @param obj Pointer to an object of type of_flow_stats_request.
 * @param match Pointer to the child object of type
 * of_match_t to be filled out.
 *
 */
int WARN_UNUSED_RESULT
of_flow_stats_request_match_get(
    of_flow_stats_request_t *obj,
    of_match_t *match)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    of_octets_t match_octets; /* Serialized string for match */

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 12;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    LOCI_ASSERT(cur_len + abs_offset <= WBUF_CURRENT_BYTES(wbuf));
    match_octets.bytes = cur_len;
    match_octets.data = OF_OBJECT_BUFFER_INDEX(obj, offset);
    OF_TRY(of_match_deserialize(ver, match, &match_octets));

    OF_LENGTH_CHECK_ASSERT(obj);

    return OF_ERROR_NONE;
}

/**
 * Set match in an object of type of_flow_stats_request.
 * @param obj Pointer to an object of type of_flow_stats_request.
 * @param match Pointer to the child of type of_match_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_flow_stats_request_match_set(
    of_flow_stats_request_t *obj,
    of_match_t *match)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */
    of_octets_t match_octets; /* Serialized string for match */

    LOCI_ASSERT(obj->object_id == OF_FLOW_STATS_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 12;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_1:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 48;
        cur_len = _WIRE_MATCH_PADDED_LEN(obj, offset);
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    LOCI_ASSERT(cur_len >= 0 && cur_len < 64 * 1024);

    /* Match object */
    OF_TRY(of_match_serialize(ver, match, &match_octets));
    new_len = match_octets.bytes;
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        match_octets.data, new_len);
    /* Free match serialized octets */
    FREE(match_octets.data);

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
of_get_config_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x8; /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_get_config_reply of_get_config_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_get_config_reply_push_wire_values(of_get_config_reply_t *obj)
{

    of_get_config_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_get_config_reply object
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
 * \ingroup of_get_config_reply
 */

of_get_config_reply_t *
of_get_config_reply_new(of_version_t version)
{
    of_get_config_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_GET_CONFIG_REPLY] + of_object_extra_len[version][OF_GET_CONFIG_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_get_config_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_get_config_reply_init(obj, version, bytes, 0);

    if (of_get_config_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_get_config_reply.
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
of_get_config_reply_init(of_get_config_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_GET_CONFIG_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_GET_CONFIG_REPLY] + of_object_extra_len[version][OF_GET_CONFIG_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_GET_CONFIG_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_get_config_reply_push_wire_types;

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
 * Create a new of_get_config_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_get_config_reply
 */

of_get_config_reply_t *
of_get_config_reply_new_from_message(of_message_t msg)
{
    of_get_config_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_get_config_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_get_config_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_get_config_reply.
 * @param obj Pointer to an object of type of_get_config_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_get_config_reply_xid_get(
    of_get_config_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GET_CONFIG_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_get_config_reply.
 * @param obj Pointer to an object of type of_get_config_reply.
 * @param xid The value to write into the object
 */
void
of_get_config_reply_xid_set(
    of_get_config_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GET_CONFIG_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get flags from an object of type of_get_config_reply.
 * @param obj Pointer to an object of type of_get_config_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_get_config_reply_flags_get(
    of_get_config_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GET_CONFIG_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
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
 * Set flags in an object of type of_get_config_reply.
 * @param obj Pointer to an object of type of_get_config_reply.
 * @param flags The value to write into the object
 */
void
of_get_config_reply_flags_set(
    of_get_config_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GET_CONFIG_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 8;
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
 * Get miss_send_len from an object of type of_get_config_reply.
 * @param obj Pointer to an object of type of_get_config_reply.
 * @param miss_send_len Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_get_config_reply_miss_send_len_get(
    of_get_config_reply_t *obj,
    uint16_t *miss_send_len)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GET_CONFIG_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, miss_send_len);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set miss_send_len in an object of type of_get_config_reply.
 * @param obj Pointer to an object of type of_get_config_reply.
 * @param miss_send_len The value to write into the object
 */
void
of_get_config_reply_miss_send_len_set(
    of_get_config_reply_t *obj,
    uint16_t miss_send_len)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GET_CONFIG_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        offset = 10;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, miss_send_len);

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
of_get_config_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x7; /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_get_config_request of_get_config_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_get_config_request_push_wire_values(of_get_config_request_t *obj)
{

    of_get_config_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_get_config_request object
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
 * \ingroup of_get_config_request
 */

of_get_config_request_t *
of_get_config_request_new(of_version_t version)
{
    of_get_config_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_GET_CONFIG_REQUEST] + of_object_extra_len[version][OF_GET_CONFIG_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_get_config_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_get_config_request_init(obj, version, bytes, 0);

    if (of_get_config_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_get_config_request.
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
of_get_config_request_init(of_get_config_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_GET_CONFIG_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_GET_CONFIG_REQUEST] + of_object_extra_len[version][OF_GET_CONFIG_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_GET_CONFIG_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_get_config_request_push_wire_types;

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
 * Create a new of_get_config_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_get_config_request
 */

of_get_config_request_t *
of_get_config_request_new_from_message(of_message_t msg)
{
    of_get_config_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_get_config_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_get_config_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_get_config_request.
 * @param obj Pointer to an object of type of_get_config_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_get_config_request_xid_get(
    of_get_config_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GET_CONFIG_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_get_config_request.
 * @param obj Pointer to an object of type of_get_config_request.
 * @param xid The value to write into the object
 */
void
of_get_config_request_xid_set(
    of_get_config_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_GET_CONFIG_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
of_hello_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x0; /* type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_hello of_hello
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_hello_push_wire_values(of_hello_t *obj)
{

    of_hello_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_hello object
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
 * \ingroup of_hello
 */

of_hello_t *
of_hello_new(of_version_t version)
{
    of_hello_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_HELLO] + of_object_extra_len[version][OF_HELLO];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_hello_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_hello_init(obj, version, bytes, 0);

    if (of_hello_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_hello.
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
of_hello_init(of_hello_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_HELLO] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_HELLO] + of_object_extra_len[version][OF_HELLO];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_HELLO;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_hello_push_wire_types;

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
 * Create a new of_hello object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_hello
 */

of_hello_t *
of_hello_new_from_message(of_message_t msg)
{
    of_hello_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_hello_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_hello_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_hello.
 * @param obj Pointer to an object of type of_hello.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_hello_xid_get(
    of_hello_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_HELLO);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_hello.
 * @param obj Pointer to an object of type of_hello.
 * @param xid The value to write into the object
 */
void
of_hello_xid_set(
    of_hello_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_HELLO);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Bind an object of type of_list_hello_elem_t to the parent of type of_hello for
 * member elements
 * @param obj Pointer to an object of type of_hello.
 * @param elements Pointer to the child object of type
 * of_list_hello_elem_t to be filled out.
 * \ingroup of_hello
 *
 * The parameter elements is filled out to point to the same underlying
 * wire buffer as its parent.
 *
 */
void
of_hello_elements_bind(
    of_hello_t *obj,
    of_list_hello_elem_t *elements)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_HELLO);
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
    of_list_hello_elem_init(elements, obj->version, 0, 1);
    /* Attach to parent */
    elements->parent = (of_object_t *)obj;
    elements->wire_object.wbuf = obj->wire_object.wbuf;
    elements->wire_object.obj_offset = abs_offset;
    elements->wire_object.owned = 0;
    elements->length = cur_len;

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Create a copy of elements into a new variable of type of_list_hello_elem_t from
 * a of_hello instance.
 *
 * @param obj Pointer to the source of type of_hello_t
 * @returns A pointer to a new instance of type of_list_hello_elem_t whose contents
 * match that of elements from source
 * @returns NULL if an error occurs
 */
of_list_hello_elem_t *
of_hello_elements_get(of_hello_t *obj) {
    of_list_hello_elem_t _elements;
    of_list_hello_elem_t *_elements_ptr;

    of_hello_elements_bind(obj, &_elements);
    _elements_ptr = (of_list_hello_elem_t *)of_object_dup(&_elements);
    return _elements_ptr;
}

/**
 * Set elements in an object of type of_hello.
 * @param obj Pointer to an object of type of_hello.
 * @param elements Pointer to the child of type of_list_hello_elem_t.
 *
 * If the child's wire buffer is the same as the parent's, then
 * nothing is done as the changes have already been registered in the
 * parent.  Otherwise, the data in the child's wire buffer is inserted
 * into the parent's and the appropriate lengths are updated.
 */
int WARN_UNUSED_RESULT
of_hello_elements_set(
    of_hello_t *obj,
    of_list_hello_elem_t *elements)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_HELLO);
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
    new_len = elements->length;
    /* If underlying buffer already shared; nothing to do */
    if (obj->wire_object.wbuf == elements->wire_object.wbuf) {
        of_wire_buffer_grow(wbuf, abs_offset + new_len);
        /* Verify that the offsets are correct */
        LOCI_ASSERT(abs_offset == OF_OBJECT_ABSOLUTE_OFFSET(elements, 0));
        /* LOCI_ASSERT(new_len == cur_len); */ /* fixme: may fail for OXM lists */
        return OF_ERROR_NONE;
    }

    /* Otherwise, replace existing object in data buffer */
    of_wire_buffer_replace_data(wbuf, abs_offset, cur_len,
        OF_OBJECT_BUFFER_INDEX(elements, 0), new_len);

    /* @fixme Shouldn't this precede copying value's data to buffer? */
    if (elements->wire_length_set != NULL) {
        elements->wire_length_set((of_object_t *)elements, elements->length);
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
of_hello_failed_error_msg_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x1; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0x0); /* err_type */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_hello_failed_error_msg of_hello_failed_error_msg
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_hello_failed_error_msg_push_wire_values(of_hello_failed_error_msg_t *obj)
{

    of_hello_failed_error_msg_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_hello_failed_error_msg object
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
 * \ingroup of_hello_failed_error_msg
 */

of_hello_failed_error_msg_t *
of_hello_failed_error_msg_new(of_version_t version)
{
    of_hello_failed_error_msg_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_HELLO_FAILED_ERROR_MSG] + of_object_extra_len[version][OF_HELLO_FAILED_ERROR_MSG];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_hello_failed_error_msg_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_hello_failed_error_msg_init(obj, version, bytes, 0);

    if (of_hello_failed_error_msg_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_hello_failed_error_msg.
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
of_hello_failed_error_msg_init(of_hello_failed_error_msg_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_HELLO_FAILED_ERROR_MSG] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_HELLO_FAILED_ERROR_MSG] + of_object_extra_len[version][OF_HELLO_FAILED_ERROR_MSG];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_HELLO_FAILED_ERROR_MSG;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_hello_failed_error_msg_push_wire_types;

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
 * Create a new of_hello_failed_error_msg object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_hello_failed_error_msg
 */

of_hello_failed_error_msg_t *
of_hello_failed_error_msg_new_from_message(of_message_t msg)
{
    of_hello_failed_error_msg_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_hello_failed_error_msg_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_hello_failed_error_msg_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_hello_failed_error_msg.
 * @param obj Pointer to an object of type of_hello_failed_error_msg.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_hello_failed_error_msg_xid_get(
    of_hello_failed_error_msg_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_HELLO_FAILED_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_hello_failed_error_msg.
 * @param obj Pointer to an object of type of_hello_failed_error_msg.
 * @param xid The value to write into the object
 */
void
of_hello_failed_error_msg_xid_set(
    of_hello_failed_error_msg_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_HELLO_FAILED_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get code from an object of type of_hello_failed_error_msg.
 * @param obj Pointer to an object of type of_hello_failed_error_msg.
 * @param code Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_hello_failed_error_msg_code_get(
    of_hello_failed_error_msg_t *obj,
    uint16_t *code)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_HELLO_FAILED_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set code in an object of type of_hello_failed_error_msg.
 * @param obj Pointer to an object of type of_hello_failed_error_msg.
 * @param code The value to write into the object
 */
void
of_hello_failed_error_msg_code_set(
    of_hello_failed_error_msg_t *obj,
    uint16_t code)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_HELLO_FAILED_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get data from an object of type of_hello_failed_error_msg.
 * @param obj Pointer to an object of type of_hello_failed_error_msg.
 * @param data Pointer to the child object of type
 * of_octets_t to be filled out.
 *
 */
void
of_hello_failed_error_msg_data_get(
    of_hello_failed_error_msg_t *obj,
    of_octets_t *data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */

    LOCI_ASSERT(obj->object_id == OF_HELLO_FAILED_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set data in an object of type of_hello_failed_error_msg.
 * @param obj Pointer to an object of type of_hello_failed_error_msg.
 * @param data The value to write into the object
 */
int WARN_UNUSED_RESULT
of_hello_failed_error_msg_data_set(
    of_hello_failed_error_msg_t *obj,
    of_octets_t *data)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;
    int cur_len = 0; /* Current length of object data */
    int new_len, delta; /* For set, need new length and delta */

    LOCI_ASSERT(obj->object_id == OF_HELLO_FAILED_ERROR_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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



/**
 * \defgroup of_match_v1 of_match_v1
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_match_v1_push_wire_values(of_match_v1_t *obj)
{

    return OF_ERROR_NONE;
}

/**
 * Create a new of_match_v1 object
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
 * \ingroup of_match_v1
 */

of_match_v1_t *
of_match_v1_new(of_version_t version)
{
    of_match_v1_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_MATCH_V1] + of_object_extra_len[version][OF_MATCH_V1];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_match_v1_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_match_v1_init(obj, version, bytes, 0);

    if (of_match_v1_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_match_v1.
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
of_match_v1_init(of_match_v1_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_MATCH_V1] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_MATCH_V1] + of_object_extra_len[version][OF_MATCH_V1];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_MATCH_V1;

    /* Set up the object's function pointers */

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Get wildcards from an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param wildcards Pointer to the child object of type
 * of_wc_bmap_t to be filled out.
 *
 */
void
of_match_v1_wildcards_get(
    of_match_v1_t *obj,
    of_wc_bmap_t *wildcards)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 0;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_wc_bmap_get(ver, wbuf, abs_offset, wildcards);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set wildcards in an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param wildcards The value to write into the object
 */
void
of_match_v1_wildcards_set(
    of_match_v1_t *obj,
    of_wc_bmap_t wildcards)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 0;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_wc_bmap_set(ver, wbuf, abs_offset, wildcards);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get in_port from an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param in_port Pointer to the child object of type
 * of_port_no_t to be filled out.
 *
 */
void
of_match_v1_in_port_get(
    of_match_v1_t *obj,
    of_port_no_t *in_port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_port_no_get(ver, wbuf, abs_offset, in_port);
    OF_PORT_NO_VALUE_CHECK(*in_port, ver);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set in_port in an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param in_port The value to write into the object
 */
void
of_match_v1_in_port_set(
    of_match_v1_t *obj,
    of_port_no_t in_port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 4;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_port_no_set(ver, wbuf, abs_offset, in_port);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get eth_src from an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param eth_src Pointer to the child object of type
 * of_mac_addr_t to be filled out.
 *
 */
void
of_match_v1_eth_src_get(
    of_match_v1_t *obj,
    of_mac_addr_t *eth_src)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 6;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_mac_get(wbuf, abs_offset, eth_src);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set eth_src in an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param eth_src The value to write into the object
 */
void
of_match_v1_eth_src_set(
    of_match_v1_t *obj,
    of_mac_addr_t eth_src)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 6;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_mac_set(wbuf, abs_offset, eth_src);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get eth_dst from an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param eth_dst Pointer to the child object of type
 * of_mac_addr_t to be filled out.
 *
 */
void
of_match_v1_eth_dst_get(
    of_match_v1_t *obj,
    of_mac_addr_t *eth_dst)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_mac_get(wbuf, abs_offset, eth_dst);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set eth_dst in an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param eth_dst The value to write into the object
 */
void
of_match_v1_eth_dst_set(
    of_match_v1_t *obj,
    of_mac_addr_t eth_dst)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 12;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_mac_set(wbuf, abs_offset, eth_dst);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_vid from an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param vlan_vid Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_match_v1_vlan_vid_get(
    of_match_v1_t *obj,
    uint16_t *vlan_vid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 18;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, vlan_vid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set vlan_vid in an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param vlan_vid The value to write into the object
 */
void
of_match_v1_vlan_vid_set(
    of_match_v1_t *obj,
    uint16_t vlan_vid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 18;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, vlan_vid);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get vlan_pcp from an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param vlan_pcp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_match_v1_vlan_pcp_get(
    of_match_v1_t *obj,
    uint8_t *vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 20;
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
 * Set vlan_pcp in an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param vlan_pcp The value to write into the object
 */
void
of_match_v1_vlan_pcp_set(
    of_match_v1_t *obj,
    uint8_t vlan_pcp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 20;
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
 * Get eth_type from an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param eth_type Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_match_v1_eth_type_get(
    of_match_v1_t *obj,
    uint16_t *eth_type)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 22;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, eth_type);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set eth_type in an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param eth_type The value to write into the object
 */
void
of_match_v1_eth_type_set(
    of_match_v1_t *obj,
    uint16_t eth_type)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 22;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, eth_type);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get ip_dscp from an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param ip_dscp Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_match_v1_ip_dscp_get(
    of_match_v1_t *obj,
    uint8_t *ip_dscp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, ip_dscp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set ip_dscp in an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param ip_dscp The value to write into the object
 */
void
of_match_v1_ip_dscp_set(
    of_match_v1_t *obj,
    uint8_t ip_dscp)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 24;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, ip_dscp);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get ip_proto from an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param ip_proto Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_match_v1_ip_proto_get(
    of_match_v1_t *obj,
    uint8_t *ip_proto)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 25;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_get(wbuf, abs_offset, ip_proto);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set ip_proto in an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param ip_proto The value to write into the object
 */
void
of_match_v1_ip_proto_set(
    of_match_v1_t *obj,
    uint8_t ip_proto)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 25;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u8_set(wbuf, abs_offset, ip_proto);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get ipv4_src from an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param ipv4_src Pointer to the child object of type
 * of_ipv4_t to be filled out.
 *
 */
void
of_match_v1_ipv4_src_get(
    of_match_v1_t *obj,
    of_ipv4_t *ipv4_src)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_ipv4_get(wbuf, abs_offset, ipv4_src);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set ipv4_src in an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param ipv4_src The value to write into the object
 */
void
of_match_v1_ipv4_src_set(
    of_match_v1_t *obj,
    of_ipv4_t ipv4_src)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 28;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_ipv4_set(wbuf, abs_offset, ipv4_src);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get ipv4_dst from an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param ipv4_dst Pointer to the child object of type
 * of_ipv4_t to be filled out.
 *
 */
void
of_match_v1_ipv4_dst_get(
    of_match_v1_t *obj,
    of_ipv4_t *ipv4_dst)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_ipv4_get(wbuf, abs_offset, ipv4_dst);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set ipv4_dst in an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param ipv4_dst The value to write into the object
 */
void
of_match_v1_ipv4_dst_set(
    of_match_v1_t *obj,
    of_ipv4_t ipv4_dst)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 32;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_ipv4_set(wbuf, abs_offset, ipv4_dst);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get tcp_src from an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param tcp_src Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_match_v1_tcp_src_get(
    of_match_v1_t *obj,
    uint16_t *tcp_src)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, tcp_src);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set tcp_src in an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param tcp_src The value to write into the object
 */
void
of_match_v1_tcp_src_set(
    of_match_v1_t *obj,
    uint16_t tcp_src)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 36;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, tcp_src);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get tcp_dst from an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param tcp_dst Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_match_v1_tcp_dst_get(
    of_match_v1_t *obj,
    uint16_t *tcp_dst)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 38;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, tcp_dst);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set tcp_dst in an object of type of_match_v1.
 * @param obj Pointer to an object of type of_match_v1.
 * @param tcp_dst The value to write into the object
 */
void
of_match_v1_tcp_dst_set(
    of_match_v1_t *obj,
    uint16_t tcp_dst)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_MATCH_V1);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 38;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, tcp_dst);

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
of_nicira_header_wire_object_id_get(of_object_t *obj, of_object_id_t *id)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 12)); /* subtype */
        switch (value) {
        case 0xa:
            *id = OF_NICIRA_CONTROLLER_ROLE_REQUEST;
            break;
        case 0xb:
            *id = OF_NICIRA_CONTROLLER_ROLE_REPLY;
            break;
        default:
            *id = OF_NICIRA_HEADER;
            break;
        }
        break;
    }
    case OF_VERSION_1_1: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 12)); /* subtype */
        switch (value) {
        default:
            *id = OF_NICIRA_HEADER;
            break;
        }
        break;
    }
    case OF_VERSION_1_2: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 12)); /* subtype */
        switch (value) {
        default:
            *id = OF_NICIRA_HEADER;
            break;
        }
        break;
    }
    case OF_VERSION_1_3: {
        uint32_t value = U32_NTOH(*(uint32_t *)(buf + 12)); /* subtype */
        switch (value) {
        default:
            *id = OF_NICIRA_HEADER;
            break;
        }
        break;
    }
    default:
        LOCI_ASSERT(0);
    }
}


/**
 * \defgroup of_nicira_header of_nicira_header
 */

/**
 * Create a new of_nicira_header object
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
 * \ingroup of_nicira_header
 */

of_nicira_header_t *
of_nicira_header_new(of_version_t version)
{
    of_nicira_header_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_NICIRA_HEADER] + of_object_extra_len[version][OF_NICIRA_HEADER];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_nicira_header_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_nicira_header_init(obj, version, bytes, 0);

    return obj;
}

/**
 * Initialize an object of type of_nicira_header.
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
of_nicira_header_init(of_nicira_header_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_NICIRA_HEADER] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_NICIRA_HEADER] + of_object_extra_len[version][OF_NICIRA_HEADER];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_NICIRA_HEADER;

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
 * Create a new of_nicira_header object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_nicira_header
 */

of_nicira_header_t *
of_nicira_header_new_from_message(of_message_t msg)
{
    of_nicira_header_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_nicira_header_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_nicira_header_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_nicira_header.
 * @param obj Pointer to an object of type of_nicira_header.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_nicira_header_xid_get(
    of_nicira_header_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_NICIRA_HEADER);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set xid in an object of type of_nicira_header.
 * @param obj Pointer to an object of type of_nicira_header.
 * @param xid The value to write into the object
 */
void
of_nicira_header_xid_set(
    of_nicira_header_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_NICIRA_HEADER);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get experimenter from an object of type of_nicira_header.
 * @param obj Pointer to an object of type of_nicira_header.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_nicira_header_experimenter_get(
    of_nicira_header_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_NICIRA_HEADER);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set experimenter in an object of type of_nicira_header.
 * @param obj Pointer to an object of type of_nicira_header.
 * @param experimenter The value to write into the object
 */
void
of_nicira_header_experimenter_set(
    of_nicira_header_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_NICIRA_HEADER);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Get subtype from an object of type of_nicira_header.
 * @param obj Pointer to an object of type of_nicira_header.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_nicira_header_subtype_get(
    of_nicira_header_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_NICIRA_HEADER);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
 * Set subtype in an object of type of_nicira_header.
 * @param obj Pointer to an object of type of_nicira_header.
 * @param subtype The value to write into the object
 */
void
of_nicira_header_subtype_set(
    of_nicira_header_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_NICIRA_HEADER);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
    case OF_VERSION_1_1:
    case OF_VERSION_1_2:
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
of_nicira_controller_role_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_0:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x2320); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0xb); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_nicira_controller_role_reply of_nicira_controller_role_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_nicira_controller_role_reply_push_wire_values(of_nicira_controller_role_reply_t *obj)
{

    of_nicira_controller_role_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_nicira_controller_role_reply object
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
 * \ingroup of_nicira_controller_role_reply
 */

of_nicira_controller_role_reply_t *
of_nicira_controller_role_reply_new(of_version_t version)
{
    of_nicira_controller_role_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_NICIRA_CONTROLLER_ROLE_REPLY] + of_object_extra_len[version][OF_NICIRA_CONTROLLER_ROLE_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_nicira_controller_role_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_nicira_controller_role_reply_init(obj, version, bytes, 0);

    if (of_nicira_controller_role_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_nicira_controller_role_reply.
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
of_nicira_controller_role_reply_init(of_nicira_controller_role_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_NICIRA_CONTROLLER_ROLE_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_NICIRA_CONTROLLER_ROLE_REPLY] + of_object_extra_len[version][OF_NICIRA_CONTROLLER_ROLE_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_NICIRA_CONTROLLER_ROLE_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_nicira_controller_role_reply_push_wire_types;

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
 * Create a new of_nicira_controller_role_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_nicira_controller_role_reply
 */

of_nicira_controller_role_reply_t *
of_nicira_controller_role_reply_new_from_message(of_message_t msg)
{
    of_nicira_controller_role_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_nicira_controller_role_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_nicira_controller_role_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_nicira_controller_role_reply.
 * @param obj Pointer to an object of type of_nicira_controller_role_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_nicira_controller_role_reply_xid_get(
    of_nicira_controller_role_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_NICIRA_CONTROLLER_ROLE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
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
 * Set xid in an object of type of_nicira_controller_role_reply.
 * @param obj Pointer to an object of type of_nicira_controller_role_reply.
 * @param xid The value to write into the object
 */
void
of_nicira_controller_role_reply_xid_set(
    of_nicira_controller_role_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_NICIRA_CONTROLLER_ROLE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
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
 * Get experimenter from an object of type of_nicira_controller_role_reply.
 * @param obj Pointer to an object of type of_nicira_controller_role_reply.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_nicira_controller_role_reply_experimenter_get(
    of_nicira_controller_role_reply_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_NICIRA_CONTROLLER_ROLE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
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
 * Set experimenter in an object of type of_nicira_controller_role_reply.
 * @param obj Pointer to an object of type of_nicira_controller_role_reply.
 * @param experimenter The value to write into the object
 */
void
of_nicira_controller_role_reply_experimenter_set(
    of_nicira_controller_role_reply_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_NICIRA_CONTROLLER_ROLE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
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
 * Get subtype from an object of type of_nicira_controller_role_reply.
 * @param obj Pointer to an object of type of_nicira_controller_role_reply.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_nicira_controller_role_reply_subtype_get(
    of_nicira_controller_role_reply_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_NICIRA_CONTROLLER_ROLE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
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
 * Set subtype in an object of type of_nicira_controller_role_reply.
 * @param obj Pointer to an object of type of_nicira_controller_role_reply.
 * @param subtype The value to write into the object
 */
void
of_nicira_controller_role_reply_subtype_set(
    of_nicira_controller_role_reply_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_NICIRA_CONTROLLER_ROLE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
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
 * Get role from an object of type of_nicira_controller_role_reply.
 * @param obj Pointer to an object of type of_nicira_controller_role_reply.
 * @param role Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_nicira_controller_role_reply_role_get(
    of_nicira_controller_role_reply_t *obj,
    uint32_t *role)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_NICIRA_CONTROLLER_ROLE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, role);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set role in an object of type of_nicira_controller_role_reply.
 * @param obj Pointer to an object of type of_nicira_controller_role_reply.
 * @param role The value to write into the object
 */
void
of_nicira_controller_role_reply_role_set(
    of_nicira_controller_role_reply_t *obj,
    uint32_t role)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_NICIRA_CONTROLLER_ROLE_REPLY);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_0:
        offset = 16;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, role);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
