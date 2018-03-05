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
of_ofdpa_port_experimenter_prop_scheduling_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint16_t *)(buf + 32) = U16_HTON(0xffff); /* pm_type */
        *(uint32_t *)(buf + 36) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 40) = U32_HTON(0x10); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_port_experimenter_prop_scheduling of_ofdpa_port_experimenter_prop_scheduling
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_port_experimenter_prop_scheduling_push_wire_values(of_ofdpa_port_experimenter_prop_scheduling_t *obj)
{

    of_ofdpa_port_experimenter_prop_scheduling_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_port_experimenter_prop_scheduling object
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
 * \ingroup of_ofdpa_port_experimenter_prop_scheduling
 */

of_ofdpa_port_experimenter_prop_scheduling_t *
of_ofdpa_port_experimenter_prop_scheduling_new(of_version_t version)
{
    of_ofdpa_port_experimenter_prop_scheduling_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING] + of_object_extra_len[version][OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_port_experimenter_prop_scheduling_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_port_experimenter_prop_scheduling_init(obj, version, bytes, 0);

    if (of_ofdpa_port_experimenter_prop_scheduling_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_port_experimenter_prop_scheduling.
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
of_ofdpa_port_experimenter_prop_scheduling_init(of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING] + of_object_extra_len[version][OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_port_experimenter_prop_scheduling_push_wire_types;

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
 * Create a new of_ofdpa_port_experimenter_prop_scheduling object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_port_experimenter_prop_scheduling
 */

of_ofdpa_port_experimenter_prop_scheduling_t *
of_ofdpa_port_experimenter_prop_scheduling_new_from_message(of_message_t msg)
{
    of_ofdpa_port_experimenter_prop_scheduling_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_port_experimenter_prop_scheduling_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_port_experimenter_prop_scheduling_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_port_experimenter_prop_scheduling_xid_get(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
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
 * Set xid in an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param xid The value to write into the object
 */
void
of_ofdpa_port_experimenter_prop_scheduling_xid_set(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
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
 * Get port_no from an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param port_no Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_port_experimenter_prop_scheduling_port_no_get(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t *port_no)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, port_no);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set port_no in an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param port_no The value to write into the object
 */
void
of_ofdpa_port_experimenter_prop_scheduling_port_no_set(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t port_no)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, port_no);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get hw_addr from an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param hw_addr Pointer to the child object of type
 * of_mac_addr_t to be filled out.
 *
 */
void
of_ofdpa_port_experimenter_prop_scheduling_hw_addr_get(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    of_mac_addr_t *hw_addr)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
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
    of_wire_buffer_mac_get(wbuf, abs_offset, hw_addr);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set hw_addr in an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param hw_addr The value to write into the object
 */
void
of_ofdpa_port_experimenter_prop_scheduling_hw_addr_set(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    of_mac_addr_t hw_addr)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
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
    of_wire_buffer_mac_set(wbuf, abs_offset, hw_addr);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get config from an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param config Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_port_experimenter_prop_scheduling_config_get(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t *config)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, config);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set config in an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param config The value to write into the object
 */
void
of_ofdpa_port_experimenter_prop_scheduling_config_set(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t config)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, config);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mask from an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param mask Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_port_experimenter_prop_scheduling_mask_get(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t *mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mask in an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param mask The value to write into the object
 */
void
of_ofdpa_port_experimenter_prop_scheduling_mask_set(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get pm_type from an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param pm_type Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_port_experimenter_prop_scheduling_pm_type_get(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint16_t *pm_type)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, pm_type);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set pm_type in an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param pm_type The value to write into the object
 */
void
of_ofdpa_port_experimenter_prop_scheduling_pm_type_set(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint16_t pm_type)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, pm_type);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get experimenter from an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_port_experimenter_prop_scheduling_experimenter_get(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set experimenter in an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_port_experimenter_prop_scheduling_experimenter_set(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, experimenter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get subtype from an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_port_experimenter_prop_scheduling_subtype_get(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set subtype in an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_port_experimenter_prop_scheduling_subtype_set(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, subtype);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mode from an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param mode Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_port_experimenter_prop_scheduling_mode_get(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint16_t *mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mode in an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param mode The value to write into the object
 */
void
of_ofdpa_port_experimenter_prop_scheduling_mode_set(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint16_t mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get parameter from an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param parameter Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_port_experimenter_prop_scheduling_parameter_get(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint16_t *parameter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 46;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, parameter);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set parameter in an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param obj Pointer to an object of type of_ofdpa_port_experimenter_prop_scheduling.
 * @param parameter The value to write into the object
 */
void
of_ofdpa_port_experimenter_prop_scheduling_parameter_set(
    of_ofdpa_port_experimenter_prop_scheduling_t *obj,
    uint16_t parameter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_PORT_EXPERIMENTER_PROP_SCHEDULING);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 46;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, parameter);

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
of_ofdpa_queue_desc_prop_congestion_multipart_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x13); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_queue_desc_prop_congestion_multipart_reply of_ofdpa_queue_desc_prop_congestion_multipart_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_queue_desc_prop_congestion_multipart_reply_push_wire_values(of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj)
{

    of_ofdpa_queue_desc_prop_congestion_multipart_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_queue_desc_prop_congestion_multipart_reply object
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
 * \ingroup of_ofdpa_queue_desc_prop_congestion_multipart_reply
 */

of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *
of_ofdpa_queue_desc_prop_congestion_multipart_reply_new(of_version_t version)
{
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY] + of_object_extra_len[version][OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_queue_desc_prop_congestion_multipart_reply_init(obj, version, bytes, 0);

    if (of_ofdpa_queue_desc_prop_congestion_multipart_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply.
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
of_ofdpa_queue_desc_prop_congestion_multipart_reply_init(of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY] + of_object_extra_len[version][OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_queue_desc_prop_congestion_multipart_reply_push_wire_types;

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
 * Create a new of_ofdpa_queue_desc_prop_congestion_multipart_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_queue_desc_prop_congestion_multipart_reply
 */

of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *
of_ofdpa_queue_desc_prop_congestion_multipart_reply_new_from_message(of_message_t msg)
{
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_queue_desc_prop_congestion_multipart_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_queue_desc_prop_congestion_multipart_reply_xid_get(
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY);
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
 * Set xid in an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param xid The value to write into the object
 */
void
of_ofdpa_queue_desc_prop_congestion_multipart_reply_xid_set(
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY);
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
 * Get flags from an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_queue_desc_prop_congestion_multipart_reply_flags_get(
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY);
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
 * Set flags in an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param flags The value to write into the object
 */
void
of_ofdpa_queue_desc_prop_congestion_multipart_reply_flags_set(
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY);
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
 * Get experimenter from an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_queue_desc_prop_congestion_multipart_reply_experimenter_get(
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY);
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
 * Set experimenter in an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_queue_desc_prop_congestion_multipart_reply_experimenter_set(
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY);
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
 * Get subtype from an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_queue_desc_prop_congestion_multipart_reply_subtype_get(
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY);
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
 * Set subtype in an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_queue_desc_prop_congestion_multipart_reply_subtype_set(
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY);
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
 * Get mode from an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param mode Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_queue_desc_prop_congestion_multipart_reply_mode_get(
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint16_t *mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mode in an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_congestion_multipart_reply.
 * @param mode The value to write into the object
 */
void
of_ofdpa_queue_desc_prop_congestion_multipart_reply_mode_set(
    of_ofdpa_queue_desc_prop_congestion_multipart_reply_t *obj,
    uint16_t mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REPLY);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, mode);

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
of_ofdpa_queue_desc_prop_congestion_multipart_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x13); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_queue_desc_prop_congestion_multipart_request of_ofdpa_queue_desc_prop_congestion_multipart_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_queue_desc_prop_congestion_multipart_request_push_wire_values(of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj)
{

    of_ofdpa_queue_desc_prop_congestion_multipart_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_queue_desc_prop_congestion_multipart_request object
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
 * \ingroup of_ofdpa_queue_desc_prop_congestion_multipart_request
 */

of_ofdpa_queue_desc_prop_congestion_multipart_request_t *
of_ofdpa_queue_desc_prop_congestion_multipart_request_new(of_version_t version)
{
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST] + of_object_extra_len[version][OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_queue_desc_prop_congestion_multipart_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_queue_desc_prop_congestion_multipart_request_init(obj, version, bytes, 0);

    if (of_ofdpa_queue_desc_prop_congestion_multipart_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request.
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
of_ofdpa_queue_desc_prop_congestion_multipart_request_init(of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST] + of_object_extra_len[version][OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_queue_desc_prop_congestion_multipart_request_push_wire_types;

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
 * Create a new of_ofdpa_queue_desc_prop_congestion_multipart_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_queue_desc_prop_congestion_multipart_request
 */

of_ofdpa_queue_desc_prop_congestion_multipart_request_t *
of_ofdpa_queue_desc_prop_congestion_multipart_request_new_from_message(of_message_t msg)
{
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_queue_desc_prop_congestion_multipart_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_queue_desc_prop_congestion_multipart_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_queue_desc_prop_congestion_multipart_request_xid_get(
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST);
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
 * Set xid in an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param xid The value to write into the object
 */
void
of_ofdpa_queue_desc_prop_congestion_multipart_request_xid_set(
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST);
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
 * Get flags from an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_queue_desc_prop_congestion_multipart_request_flags_get(
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST);
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
 * Set flags in an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param flags The value to write into the object
 */
void
of_ofdpa_queue_desc_prop_congestion_multipart_request_flags_set(
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST);
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
 * Get experimenter from an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_queue_desc_prop_congestion_multipart_request_experimenter_get(
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST);
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
 * Set experimenter in an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_queue_desc_prop_congestion_multipart_request_experimenter_set(
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST);
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
 * Get subtype from an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_queue_desc_prop_congestion_multipart_request_subtype_get(
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST);
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
 * Set subtype in an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_queue_desc_prop_congestion_multipart_request_subtype_set(
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST);
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
 * Get mode from an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param mode Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_queue_desc_prop_congestion_multipart_request_mode_get(
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint16_t *mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mode in an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_congestion_multipart_request.
 * @param mode The value to write into the object
 */
void
of_ofdpa_queue_desc_prop_congestion_multipart_request_mode_set(
    of_ofdpa_queue_desc_prop_congestion_multipart_request_t *obj,
    uint16_t mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_CONGESTION_MULTIPART_REQUEST);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, mode);

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
of_ofdpa_queue_desc_prop_parent_multipart_reply_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x12); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_queue_desc_prop_parent_multipart_reply of_ofdpa_queue_desc_prop_parent_multipart_reply
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_queue_desc_prop_parent_multipart_reply_push_wire_values(of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj)
{

    of_ofdpa_queue_desc_prop_parent_multipart_reply_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_queue_desc_prop_parent_multipart_reply object
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
 * \ingroup of_ofdpa_queue_desc_prop_parent_multipart_reply
 */

of_ofdpa_queue_desc_prop_parent_multipart_reply_t *
of_ofdpa_queue_desc_prop_parent_multipart_reply_new(of_version_t version)
{
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY] + of_object_extra_len[version][OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_queue_desc_prop_parent_multipart_reply_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_queue_desc_prop_parent_multipart_reply_init(obj, version, bytes, 0);

    if (of_ofdpa_queue_desc_prop_parent_multipart_reply_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply.
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
of_ofdpa_queue_desc_prop_parent_multipart_reply_init(of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY] + of_object_extra_len[version][OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_queue_desc_prop_parent_multipart_reply_push_wire_types;

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
 * Create a new of_ofdpa_queue_desc_prop_parent_multipart_reply object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_queue_desc_prop_parent_multipart_reply
 */

of_ofdpa_queue_desc_prop_parent_multipart_reply_t *
of_ofdpa_queue_desc_prop_parent_multipart_reply_new_from_message(of_message_t msg)
{
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_queue_desc_prop_parent_multipart_reply_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_queue_desc_prop_parent_multipart_reply_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_queue_desc_prop_parent_multipart_reply_xid_get(
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY);
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
 * Set xid in an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param xid The value to write into the object
 */
void
of_ofdpa_queue_desc_prop_parent_multipart_reply_xid_set(
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY);
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
 * Get flags from an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_queue_desc_prop_parent_multipart_reply_flags_get(
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY);
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
 * Set flags in an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param flags The value to write into the object
 */
void
of_ofdpa_queue_desc_prop_parent_multipart_reply_flags_set(
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY);
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
 * Get experimenter from an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_queue_desc_prop_parent_multipart_reply_experimenter_get(
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY);
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
 * Set experimenter in an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_queue_desc_prop_parent_multipart_reply_experimenter_set(
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY);
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
 * Get subtype from an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_queue_desc_prop_parent_multipart_reply_subtype_get(
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY);
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
 * Set subtype in an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_queue_desc_prop_parent_multipart_reply_subtype_set(
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY);
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
 * Get scheduler_level from an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param scheduler_level Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_queue_desc_prop_parent_multipart_reply_scheduler_level_get(
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint16_t *scheduler_level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, scheduler_level);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set scheduler_level in an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_parent_multipart_reply.
 * @param scheduler_level The value to write into the object
 */
void
of_ofdpa_queue_desc_prop_parent_multipart_reply_scheduler_level_set(
    of_ofdpa_queue_desc_prop_parent_multipart_reply_t *obj,
    uint16_t scheduler_level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REPLY);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, scheduler_level);

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
of_ofdpa_queue_desc_prop_parent_multipart_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x12); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_queue_desc_prop_parent_multipart_request of_ofdpa_queue_desc_prop_parent_multipart_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_queue_desc_prop_parent_multipart_request_push_wire_values(of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj)
{

    of_ofdpa_queue_desc_prop_parent_multipart_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_queue_desc_prop_parent_multipart_request object
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
 * \ingroup of_ofdpa_queue_desc_prop_parent_multipart_request
 */

of_ofdpa_queue_desc_prop_parent_multipart_request_t *
of_ofdpa_queue_desc_prop_parent_multipart_request_new(of_version_t version)
{
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST] + of_object_extra_len[version][OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_queue_desc_prop_parent_multipart_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_queue_desc_prop_parent_multipart_request_init(obj, version, bytes, 0);

    if (of_ofdpa_queue_desc_prop_parent_multipart_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_queue_desc_prop_parent_multipart_request.
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
of_ofdpa_queue_desc_prop_parent_multipart_request_init(of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST] + of_object_extra_len[version][OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_queue_desc_prop_parent_multipart_request_push_wire_types;

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
 * Create a new of_ofdpa_queue_desc_prop_parent_multipart_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_queue_desc_prop_parent_multipart_request
 */

of_ofdpa_queue_desc_prop_parent_multipart_request_t *
of_ofdpa_queue_desc_prop_parent_multipart_request_new_from_message(of_message_t msg)
{
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_queue_desc_prop_parent_multipart_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_queue_desc_prop_parent_multipart_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_queue_desc_prop_parent_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_parent_multipart_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_queue_desc_prop_parent_multipart_request_xid_get(
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST);
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
 * Set xid in an object of type of_ofdpa_queue_desc_prop_parent_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_parent_multipart_request.
 * @param xid The value to write into the object
 */
void
of_ofdpa_queue_desc_prop_parent_multipart_request_xid_set(
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST);
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
 * Get flags from an object of type of_ofdpa_queue_desc_prop_parent_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_parent_multipart_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_queue_desc_prop_parent_multipart_request_flags_get(
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST);
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
 * Set flags in an object of type of_ofdpa_queue_desc_prop_parent_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_parent_multipart_request.
 * @param flags The value to write into the object
 */
void
of_ofdpa_queue_desc_prop_parent_multipart_request_flags_set(
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST);
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
 * Get experimenter from an object of type of_ofdpa_queue_desc_prop_parent_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_parent_multipart_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_queue_desc_prop_parent_multipart_request_experimenter_get(
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST);
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
 * Set experimenter in an object of type of_ofdpa_queue_desc_prop_parent_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_parent_multipart_request.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_queue_desc_prop_parent_multipart_request_experimenter_set(
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST);
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
 * Get subtype from an object of type of_ofdpa_queue_desc_prop_parent_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_parent_multipart_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_queue_desc_prop_parent_multipart_request_subtype_get(
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST);
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
 * Set subtype in an object of type of_ofdpa_queue_desc_prop_parent_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_parent_multipart_request.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_queue_desc_prop_parent_multipart_request_subtype_set(
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST);
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
 * Get scheduler_level from an object of type of_ofdpa_queue_desc_prop_parent_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_parent_multipart_request.
 * @param scheduler_level Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_queue_desc_prop_parent_multipart_request_scheduler_level_get(
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint16_t *scheduler_level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, scheduler_level);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set scheduler_level in an object of type of_ofdpa_queue_desc_prop_parent_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_queue_desc_prop_parent_multipart_request.
 * @param scheduler_level The value to write into the object
 */
void
of_ofdpa_queue_desc_prop_parent_multipart_request_scheduler_level_set(
    of_ofdpa_queue_desc_prop_parent_multipart_request_t *obj,
    uint16_t scheduler_level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_DESC_PROP_PARENT_MULTIPART_REQUEST);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, scheduler_level);

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
of_ofdpa_queue_mod_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0x11); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_queue_mod of_ofdpa_queue_mod
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_queue_mod_push_wire_values(of_ofdpa_queue_mod_t *obj)
{

    of_ofdpa_queue_mod_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_queue_mod object
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
 * \ingroup of_ofdpa_queue_mod
 */

of_ofdpa_queue_mod_t *
of_ofdpa_queue_mod_new(of_version_t version)
{
    of_ofdpa_queue_mod_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_QUEUE_MOD] + of_object_extra_len[version][OF_OFDPA_QUEUE_MOD];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_queue_mod_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_queue_mod_init(obj, version, bytes, 0);

    if (of_ofdpa_queue_mod_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_queue_mod.
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
of_ofdpa_queue_mod_init(of_ofdpa_queue_mod_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_QUEUE_MOD] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_QUEUE_MOD] + of_object_extra_len[version][OF_OFDPA_QUEUE_MOD];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_QUEUE_MOD;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_queue_mod_push_wire_types;

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
 * Create a new of_ofdpa_queue_mod object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_queue_mod
 */

of_ofdpa_queue_mod_t *
of_ofdpa_queue_mod_new_from_message(of_message_t msg)
{
    of_ofdpa_queue_mod_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_queue_mod_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_queue_mod_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_queue_mod.
 * @param obj Pointer to an object of type of_ofdpa_queue_mod.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_queue_mod_xid_get(
    of_ofdpa_queue_mod_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_MOD);
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
 * Set xid in an object of type of_ofdpa_queue_mod.
 * @param obj Pointer to an object of type of_ofdpa_queue_mod.
 * @param xid The value to write into the object
 */
void
of_ofdpa_queue_mod_xid_set(
    of_ofdpa_queue_mod_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_MOD);
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
 * Get experimenter from an object of type of_ofdpa_queue_mod.
 * @param obj Pointer to an object of type of_ofdpa_queue_mod.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_queue_mod_experimenter_get(
    of_ofdpa_queue_mod_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_MOD);
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
 * Set experimenter in an object of type of_ofdpa_queue_mod.
 * @param obj Pointer to an object of type of_ofdpa_queue_mod.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_queue_mod_experimenter_set(
    of_ofdpa_queue_mod_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_MOD);
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
 * Get subtype from an object of type of_ofdpa_queue_mod.
 * @param obj Pointer to an object of type of_ofdpa_queue_mod.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_queue_mod_subtype_get(
    of_ofdpa_queue_mod_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_MOD);
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
 * Set subtype in an object of type of_ofdpa_queue_mod.
 * @param obj Pointer to an object of type of_ofdpa_queue_mod.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_queue_mod_subtype_set(
    of_ofdpa_queue_mod_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_MOD);
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
 * Get mod_command from an object of type of_ofdpa_queue_mod.
 * @param obj Pointer to an object of type of_ofdpa_queue_mod.
 * @param mod_command Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_queue_mod_mod_command_get(
    of_ofdpa_queue_mod_t *obj,
    uint32_t *mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_MOD);
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
 * Set mod_command in an object of type of_ofdpa_queue_mod.
 * @param obj Pointer to an object of type of_ofdpa_queue_mod.
 * @param mod_command The value to write into the object
 */
void
of_ofdpa_queue_mod_mod_command_set(
    of_ofdpa_queue_mod_t *obj,
    uint32_t mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_MOD);
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
 * Get port_no from an object of type of_ofdpa_queue_mod.
 * @param obj Pointer to an object of type of_ofdpa_queue_mod.
 * @param port_no Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_queue_mod_port_no_get(
    of_ofdpa_queue_mod_t *obj,
    uint32_t *port_no)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_MOD);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, port_no);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set port_no in an object of type of_ofdpa_queue_mod.
 * @param obj Pointer to an object of type of_ofdpa_queue_mod.
 * @param port_no The value to write into the object
 */
void
of_ofdpa_queue_mod_port_no_set(
    of_ofdpa_queue_mod_t *obj,
    uint32_t port_no)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_MOD);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, port_no);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get queue_id from an object of type of_ofdpa_queue_mod.
 * @param obj Pointer to an object of type of_ofdpa_queue_mod.
 * @param queue_id Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_queue_mod_queue_id_get(
    of_ofdpa_queue_mod_t *obj,
    uint32_t *queue_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_MOD);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, queue_id);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set queue_id in an object of type of_ofdpa_queue_mod.
 * @param obj Pointer to an object of type of_ofdpa_queue_mod.
 * @param queue_id The value to write into the object
 */
void
of_ofdpa_queue_mod_queue_id_set(
    of_ofdpa_queue_mod_t *obj,
    uint32_t queue_id)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_QUEUE_MOD);
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
of_ofdpa_scheduler_node_mod_msg_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x4; /* type */
        *(uint32_t *)(buf + 8) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 12) = U32_HTON(0x14); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_scheduler_node_mod_msg of_ofdpa_scheduler_node_mod_msg
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_scheduler_node_mod_msg_push_wire_values(of_ofdpa_scheduler_node_mod_msg_t *obj)
{

    of_ofdpa_scheduler_node_mod_msg_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_scheduler_node_mod_msg object
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
 * \ingroup of_ofdpa_scheduler_node_mod_msg
 */

of_ofdpa_scheduler_node_mod_msg_t *
of_ofdpa_scheduler_node_mod_msg_new(of_version_t version)
{
    of_ofdpa_scheduler_node_mod_msg_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_SCHEDULER_NODE_MOD_MSG] + of_object_extra_len[version][OF_OFDPA_SCHEDULER_NODE_MOD_MSG];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_scheduler_node_mod_msg_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_scheduler_node_mod_msg_init(obj, version, bytes, 0);

    if (of_ofdpa_scheduler_node_mod_msg_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_scheduler_node_mod_msg.
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
of_ofdpa_scheduler_node_mod_msg_init(of_ofdpa_scheduler_node_mod_msg_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_SCHEDULER_NODE_MOD_MSG] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_SCHEDULER_NODE_MOD_MSG] + of_object_extra_len[version][OF_OFDPA_SCHEDULER_NODE_MOD_MSG];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_SCHEDULER_NODE_MOD_MSG;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_scheduler_node_mod_msg_push_wire_types;

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
 * Create a new of_ofdpa_scheduler_node_mod_msg object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_scheduler_node_mod_msg
 */

of_ofdpa_scheduler_node_mod_msg_t *
of_ofdpa_scheduler_node_mod_msg_new_from_message(of_message_t msg)
{
    of_ofdpa_scheduler_node_mod_msg_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_scheduler_node_mod_msg_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_scheduler_node_mod_msg_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_mod_msg_xid_get(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MOD_MSG);
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
 * Set xid in an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param xid The value to write into the object
 */
void
of_ofdpa_scheduler_node_mod_msg_xid_set(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MOD_MSG);
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
 * Get experimenter from an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_mod_msg_experimenter_get(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MOD_MSG);
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
 * Set experimenter in an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_scheduler_node_mod_msg_experimenter_set(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MOD_MSG);
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
 * Get subtype from an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_mod_msg_subtype_get(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MOD_MSG);
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
 * Set subtype in an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_scheduler_node_mod_msg_subtype_set(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MOD_MSG);
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
 * Get mod_command from an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param mod_command Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_mod_msg_mod_command_get(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t *mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MOD_MSG);
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
 * Set mod_command in an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param mod_command The value to write into the object
 */
void
of_ofdpa_scheduler_node_mod_msg_mod_command_set(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t mod_command)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MOD_MSG);
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
 * Get level from an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param level Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_mod_msg_level_get(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t *level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MOD_MSG);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, level);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set level in an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param level The value to write into the object
 */
void
of_ofdpa_scheduler_node_mod_msg_level_set(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MOD_MSG);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, level);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get port from an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param port Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_mod_msg_port_get(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t *port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 22;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_get(wbuf, abs_offset, port);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set port in an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param port The value to write into the object
 */
void
of_ofdpa_scheduler_node_mod_msg_port_set(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint32_t port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MOD_MSG);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 22;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u32_set(wbuf, abs_offset, port);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get max_rate from an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param max_rate Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_mod_msg_max_rate_get(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t *max_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MOD_MSG);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, max_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set max_rate in an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param max_rate The value to write into the object
 */
void
of_ofdpa_scheduler_node_mod_msg_max_rate_set(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t max_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MOD_MSG);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, max_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get min_rate from an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param min_rate Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_mod_msg_min_rate_get(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t *min_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MOD_MSG);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, min_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set min_rate in an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param min_rate The value to write into the object
 */
void
of_ofdpa_scheduler_node_mod_msg_min_rate_set(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t min_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MOD_MSG);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, min_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mode from an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param mode Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_mod_msg_mode_get(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t *mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MOD_MSG);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mode in an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param mode The value to write into the object
 */
void
of_ofdpa_scheduler_node_mod_msg_mode_set(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MOD_MSG);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get weight from an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param weight Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_mod_msg_weight_get(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t *weight)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MOD_MSG);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, weight);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set weight in an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_mod_msg.
 * @param weight The value to write into the object
 */
void
of_ofdpa_scheduler_node_mod_msg_weight_set(
    of_ofdpa_scheduler_node_mod_msg_t *obj,
    uint16_t weight)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MOD_MSG);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, weight);

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
of_ofdpa_scheduler_node_multipart_request_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x12; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x15); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_scheduler_node_multipart_request of_ofdpa_scheduler_node_multipart_request
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_scheduler_node_multipart_request_push_wire_values(of_ofdpa_scheduler_node_multipart_request_t *obj)
{

    of_ofdpa_scheduler_node_multipart_request_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_scheduler_node_multipart_request object
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
 * \ingroup of_ofdpa_scheduler_node_multipart_request
 */

of_ofdpa_scheduler_node_multipart_request_t *
of_ofdpa_scheduler_node_multipart_request_new(of_version_t version)
{
    of_ofdpa_scheduler_node_multipart_request_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST] + of_object_extra_len[version][OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_scheduler_node_multipart_request_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_scheduler_node_multipart_request_init(obj, version, bytes, 0);

    if (of_ofdpa_scheduler_node_multipart_request_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_scheduler_node_multipart_request.
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
of_ofdpa_scheduler_node_multipart_request_init(of_ofdpa_scheduler_node_multipart_request_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST] + of_object_extra_len[version][OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_scheduler_node_multipart_request_push_wire_types;

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
 * Create a new of_ofdpa_scheduler_node_multipart_request object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_scheduler_node_multipart_request
 */

of_ofdpa_scheduler_node_multipart_request_t *
of_ofdpa_scheduler_node_multipart_request_new_from_message(of_message_t msg)
{
    of_ofdpa_scheduler_node_multipart_request_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_scheduler_node_multipart_request_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_scheduler_node_multipart_request_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_multipart_request_xid_get(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
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
 * Set xid in an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param xid The value to write into the object
 */
void
of_ofdpa_scheduler_node_multipart_request_xid_set(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
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
 * Get flags from an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_multipart_request_flags_get(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
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
 * Set flags in an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param flags The value to write into the object
 */
void
of_ofdpa_scheduler_node_multipart_request_flags_set(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
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
 * Get experimenter from an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_multipart_request_experimenter_get(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
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
 * Set experimenter in an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_scheduler_node_multipart_request_experimenter_set(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
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
 * Get subtype from an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_multipart_request_subtype_get(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
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
 * Set subtype in an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_scheduler_node_multipart_request_subtype_set(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
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
 * Get port from an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param port Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_multipart_request_port_get(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t *port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, port);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set port in an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param port The value to write into the object
 */
void
of_ofdpa_scheduler_node_multipart_request_port_set(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint32_t port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, port);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get level from an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param level Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_multipart_request_level_get(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t *level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, level);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set level in an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param level The value to write into the object
 */
void
of_ofdpa_scheduler_node_multipart_request_level_set(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, level);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get max_rate from an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param max_rate Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_multipart_request_max_rate_get(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t *max_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, max_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set max_rate in an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param max_rate The value to write into the object
 */
void
of_ofdpa_scheduler_node_multipart_request_max_rate_set(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t max_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, max_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get min_rate from an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param min_rate Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_multipart_request_min_rate_get(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t *min_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, min_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set min_rate in an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param min_rate The value to write into the object
 */
void
of_ofdpa_scheduler_node_multipart_request_min_rate_set(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t min_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, min_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mode from an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param mode Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_multipart_request_mode_get(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t *mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 34;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mode in an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param mode The value to write into the object
 */
void
of_ofdpa_scheduler_node_multipart_request_mode_set(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 34;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get weight from an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param weight Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_multipart_request_weight_get(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t *weight)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, weight);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set weight in an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_request.
 * @param weight The value to write into the object
 */
void
of_ofdpa_scheduler_node_multipart_request_weight_set(
    of_ofdpa_scheduler_node_multipart_request_t *obj,
    uint16_t weight)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_REQUEST);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, weight);

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
of_ofdpa_scheduler_node_multipart_response_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint8_t *)(buf + 0) = obj->version; /* version */
        *(uint8_t *)(buf + 1) = 0x13; /* type */
        *(uint16_t *)(buf + 8) = U16_HTON(0xffff); /* stats_type */
        *(uint32_t *)(buf + 16) = U32_HTON(0x1018); /* experimenter */
        *(uint32_t *)(buf + 20) = U32_HTON(0x15); /* subtype */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_ofdpa_scheduler_node_multipart_response of_ofdpa_scheduler_node_multipart_response
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_ofdpa_scheduler_node_multipart_response_push_wire_values(of_ofdpa_scheduler_node_multipart_response_t *obj)
{

    of_ofdpa_scheduler_node_multipart_response_push_wire_types(obj);

    /* Message obj; set length */
    of_message_t msg;

    if ((msg = OF_OBJECT_TO_MESSAGE(obj)) != NULL) {
        of_message_length_set(msg, obj->length);
    }

    return OF_ERROR_NONE;
}

/**
 * Create a new of_ofdpa_scheduler_node_multipart_response object
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
 * \ingroup of_ofdpa_scheduler_node_multipart_response
 */

of_ofdpa_scheduler_node_multipart_response_t *
of_ofdpa_scheduler_node_multipart_response_new(of_version_t version)
{
    of_ofdpa_scheduler_node_multipart_response_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE] + of_object_extra_len[version][OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_ofdpa_scheduler_node_multipart_response_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_ofdpa_scheduler_node_multipart_response_init(obj, version, bytes, 0);

    if (of_ofdpa_scheduler_node_multipart_response_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_ofdpa_scheduler_node_multipart_response.
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
of_ofdpa_scheduler_node_multipart_response_init(of_ofdpa_scheduler_node_multipart_response_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE] + of_object_extra_len[version][OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_ofdpa_scheduler_node_multipart_response_push_wire_types;

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
 * Create a new of_ofdpa_scheduler_node_multipart_response object and bind it to an existing message
 *
 * @param msg The message to bind the new object to
 * @return Pointer to the newly create object or NULL on error
 *
 * \ingroup of_ofdpa_scheduler_node_multipart_response
 */

of_ofdpa_scheduler_node_multipart_response_t *
of_ofdpa_scheduler_node_multipart_response_new_from_message(of_message_t msg)
{
    of_ofdpa_scheduler_node_multipart_response_t *obj = NULL;
    of_version_t version;
    int length;

    if (msg == NULL) return NULL;

    version = of_message_version_get(msg);
    if (!OF_VERSION_OKAY(version)) return NULL;

    length = of_message_length_get(msg);

    if ((obj = (of_ofdpa_scheduler_node_multipart_response_t *)of_object_new(-1)) == NULL) {
        return NULL;
    }

    of_ofdpa_scheduler_node_multipart_response_init(obj, version, 0, 0);

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
 * Get xid from an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param xid Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_multipart_response_xid_get(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t *xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
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
 * Set xid in an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param xid The value to write into the object
 */
void
of_ofdpa_scheduler_node_multipart_response_xid_set(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t xid)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
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
 * Get flags from an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param flags Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_multipart_response_flags_get(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t *flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
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
 * Set flags in an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param flags The value to write into the object
 */
void
of_ofdpa_scheduler_node_multipart_response_flags_set(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t flags)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
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
 * Get experimenter from an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_multipart_response_experimenter_get(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
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
 * Set experimenter in an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param experimenter The value to write into the object
 */
void
of_ofdpa_scheduler_node_multipart_response_experimenter_set(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
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
 * Get subtype from an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param subtype Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_multipart_response_subtype_get(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t *subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
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
 * Set subtype in an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param subtype The value to write into the object
 */
void
of_ofdpa_scheduler_node_multipart_response_subtype_set(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t subtype)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
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
 * Get port from an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param port Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_multipart_response_port_get(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t *port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, port);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set port in an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param port The value to write into the object
 */
void
of_ofdpa_scheduler_node_multipart_response_port_set(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint32_t port)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, port);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get level from an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param level Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_multipart_response_level_get(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t *level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, level);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set level in an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param level The value to write into the object
 */
void
of_ofdpa_scheduler_node_multipart_response_level_set(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t level)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, level);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get max_rate from an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param max_rate Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_multipart_response_max_rate_get(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t *max_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, max_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set max_rate in an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param max_rate The value to write into the object
 */
void
of_ofdpa_scheduler_node_multipart_response_max_rate_set(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t max_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, max_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get min_rate from an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param min_rate Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_multipart_response_min_rate_get(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t *min_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, min_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set min_rate in an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param min_rate The value to write into the object
 */
void
of_ofdpa_scheduler_node_multipart_response_min_rate_set(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t min_rate)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, min_rate);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get mode from an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param mode Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_multipart_response_mode_get(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t *mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 34;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_get(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set mode in an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param mode The value to write into the object
 */
void
of_ofdpa_scheduler_node_multipart_response_mode_set(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t mode)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
    ver = obj->version;
    wbuf = OF_OBJECT_TO_WBUF(obj);
    LOCI_ASSERT(wbuf != NULL);

    /* By version, determine offset and current length (where needed) */
    switch (ver) {
    case OF_VERSION_1_3:
        offset = 34;
        break;
    default:
        LOCI_ASSERT(0);
    }

    abs_offset = OF_OBJECT_ABSOLUTE_OFFSET(obj, offset);
    LOCI_ASSERT(abs_offset >= 0);
    of_wire_buffer_u16_set(wbuf, abs_offset, mode);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get weight from an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param weight Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_ofdpa_scheduler_node_multipart_response_weight_get(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t *weight)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, weight);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set weight in an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param obj Pointer to an object of type of_ofdpa_scheduler_node_multipart_response.
 * @param weight The value to write into the object
 */
void
of_ofdpa_scheduler_node_multipart_response_weight_set(
    of_ofdpa_scheduler_node_multipart_response_t *obj,
    uint16_t weight)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OFDPA_SCHEDULER_NODE_MULTIPART_RESPONSE);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, weight);

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
of_oxm_ofdpa_allow_vlan_translation_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff3005); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_allow_vlan_translation of_oxm_ofdpa_allow_vlan_translation
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_allow_vlan_translation_push_wire_values(of_oxm_ofdpa_allow_vlan_translation_t *obj)
{

    of_oxm_ofdpa_allow_vlan_translation_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_allow_vlan_translation object
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
 * \ingroup of_oxm_ofdpa_allow_vlan_translation
 */

of_oxm_ofdpa_allow_vlan_translation_t *
of_oxm_ofdpa_allow_vlan_translation_new(of_version_t version)
{
    of_oxm_ofdpa_allow_vlan_translation_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_ALLOW_VLAN_TRANSLATION] + of_object_extra_len[version][OF_OXM_OFDPA_ALLOW_VLAN_TRANSLATION];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_allow_vlan_translation_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_allow_vlan_translation_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_allow_vlan_translation_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_allow_vlan_translation.
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
of_oxm_ofdpa_allow_vlan_translation_init(of_oxm_ofdpa_allow_vlan_translation_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_ALLOW_VLAN_TRANSLATION] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_ALLOW_VLAN_TRANSLATION] + of_object_extra_len[version][OF_OXM_OFDPA_ALLOW_VLAN_TRANSLATION];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_ALLOW_VLAN_TRANSLATION;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_allow_vlan_translation_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_allow_vlan_translation.
 * @param obj Pointer to an object of type of_oxm_ofdpa_allow_vlan_translation.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_allow_vlan_translation_experimenter_get(
    of_oxm_ofdpa_allow_vlan_translation_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_ALLOW_VLAN_TRANSLATION);
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
 * Set experimenter in an object of type of_oxm_ofdpa_allow_vlan_translation.
 * @param obj Pointer to an object of type of_oxm_ofdpa_allow_vlan_translation.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_allow_vlan_translation_experimenter_set(
    of_oxm_ofdpa_allow_vlan_translation_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_ALLOW_VLAN_TRANSLATION);
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
 * Get value from an object of type of_oxm_ofdpa_allow_vlan_translation.
 * @param obj Pointer to an object of type of_oxm_ofdpa_allow_vlan_translation.
 * @param value Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_oxm_ofdpa_allow_vlan_translation_value_get(
    of_oxm_ofdpa_allow_vlan_translation_t *obj,
    uint8_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_ALLOW_VLAN_TRANSLATION);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_ofdpa_allow_vlan_translation.
 * @param obj Pointer to an object of type of_oxm_ofdpa_allow_vlan_translation.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_allow_vlan_translation_value_set(
    of_oxm_ofdpa_allow_vlan_translation_t *obj,
    uint8_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_ALLOW_VLAN_TRANSLATION);
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

static void
of_oxm_ofdpa_color_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff0605); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_color of_oxm_ofdpa_color
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_color_push_wire_values(of_oxm_ofdpa_color_t *obj)
{

    of_oxm_ofdpa_color_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_color object
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
 * \ingroup of_oxm_ofdpa_color
 */

of_oxm_ofdpa_color_t *
of_oxm_ofdpa_color_new(of_version_t version)
{
    of_oxm_ofdpa_color_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_COLOR] + of_object_extra_len[version][OF_OXM_OFDPA_COLOR];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_color_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_color_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_color_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_color.
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
of_oxm_ofdpa_color_init(of_oxm_ofdpa_color_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_COLOR] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_COLOR] + of_object_extra_len[version][OF_OXM_OFDPA_COLOR];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_COLOR;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_color_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_color.
 * @param obj Pointer to an object of type of_oxm_ofdpa_color.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_color_experimenter_get(
    of_oxm_ofdpa_color_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_COLOR);
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
 * Set experimenter in an object of type of_oxm_ofdpa_color.
 * @param obj Pointer to an object of type of_oxm_ofdpa_color.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_color_experimenter_set(
    of_oxm_ofdpa_color_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_COLOR);
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
 * Get value from an object of type of_oxm_ofdpa_color.
 * @param obj Pointer to an object of type of_oxm_ofdpa_color.
 * @param value Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_oxm_ofdpa_color_value_get(
    of_oxm_ofdpa_color_t *obj,
    uint8_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_COLOR);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_ofdpa_color.
 * @param obj Pointer to an object of type of_oxm_ofdpa_color.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_color_value_set(
    of_oxm_ofdpa_color_t *obj,
    uint8_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_COLOR);
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

static void
of_oxm_ofdpa_color_actions_index_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff2008); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_color_actions_index of_oxm_ofdpa_color_actions_index
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_color_actions_index_push_wire_values(of_oxm_ofdpa_color_actions_index_t *obj)
{

    of_oxm_ofdpa_color_actions_index_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_color_actions_index object
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
 * \ingroup of_oxm_ofdpa_color_actions_index
 */

of_oxm_ofdpa_color_actions_index_t *
of_oxm_ofdpa_color_actions_index_new(of_version_t version)
{
    of_oxm_ofdpa_color_actions_index_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_COLOR_ACTIONS_INDEX] + of_object_extra_len[version][OF_OXM_OFDPA_COLOR_ACTIONS_INDEX];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_color_actions_index_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_color_actions_index_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_color_actions_index_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_color_actions_index.
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
of_oxm_ofdpa_color_actions_index_init(of_oxm_ofdpa_color_actions_index_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_COLOR_ACTIONS_INDEX] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_COLOR_ACTIONS_INDEX] + of_object_extra_len[version][OF_OXM_OFDPA_COLOR_ACTIONS_INDEX];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_COLOR_ACTIONS_INDEX;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_color_actions_index_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_color_actions_index.
 * @param obj Pointer to an object of type of_oxm_ofdpa_color_actions_index.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_color_actions_index_experimenter_get(
    of_oxm_ofdpa_color_actions_index_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_COLOR_ACTIONS_INDEX);
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
 * Set experimenter in an object of type of_oxm_ofdpa_color_actions_index.
 * @param obj Pointer to an object of type of_oxm_ofdpa_color_actions_index.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_color_actions_index_experimenter_set(
    of_oxm_ofdpa_color_actions_index_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_COLOR_ACTIONS_INDEX);
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
 * Get value from an object of type of_oxm_ofdpa_color_actions_index.
 * @param obj Pointer to an object of type of_oxm_ofdpa_color_actions_index.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_color_actions_index_value_get(
    of_oxm_ofdpa_color_actions_index_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_COLOR_ACTIONS_INDEX);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_ofdpa_color_actions_index.
 * @param obj Pointer to an object of type of_oxm_ofdpa_color_actions_index.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_color_actions_index_value_set(
    of_oxm_ofdpa_color_actions_index_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_COLOR_ACTIONS_INDEX);
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

static void
of_oxm_ofdpa_dei_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff0805); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_dei of_oxm_ofdpa_dei
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_dei_push_wire_values(of_oxm_ofdpa_dei_t *obj)
{

    of_oxm_ofdpa_dei_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_dei object
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
 * \ingroup of_oxm_ofdpa_dei
 */

of_oxm_ofdpa_dei_t *
of_oxm_ofdpa_dei_new(of_version_t version)
{
    of_oxm_ofdpa_dei_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_DEI] + of_object_extra_len[version][OF_OXM_OFDPA_DEI];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_dei_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_dei_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_dei_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_dei.
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
of_oxm_ofdpa_dei_init(of_oxm_ofdpa_dei_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_DEI] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_DEI] + of_object_extra_len[version][OF_OXM_OFDPA_DEI];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_DEI;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_dei_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_dei.
 * @param obj Pointer to an object of type of_oxm_ofdpa_dei.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_dei_experimenter_get(
    of_oxm_ofdpa_dei_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_DEI);
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
 * Set experimenter in an object of type of_oxm_ofdpa_dei.
 * @param obj Pointer to an object of type of_oxm_ofdpa_dei.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_dei_experimenter_set(
    of_oxm_ofdpa_dei_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_DEI);
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
 * Get value from an object of type of_oxm_ofdpa_dei.
 * @param obj Pointer to an object of type of_oxm_ofdpa_dei.
 * @param value Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_oxm_ofdpa_dei_value_get(
    of_oxm_ofdpa_dei_t *obj,
    uint8_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_DEI);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_ofdpa_dei.
 * @param obj Pointer to an object of type of_oxm_ofdpa_dei.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_dei_value_set(
    of_oxm_ofdpa_dei_t *obj,
    uint8_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_DEI);
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

static void
of_oxm_ofdpa_l3_in_port_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff1208); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_l3_in_port of_oxm_ofdpa_l3_in_port
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_l3_in_port_push_wire_values(of_oxm_ofdpa_l3_in_port_t *obj)
{

    of_oxm_ofdpa_l3_in_port_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_l3_in_port object
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
 * \ingroup of_oxm_ofdpa_l3_in_port
 */

of_oxm_ofdpa_l3_in_port_t *
of_oxm_ofdpa_l3_in_port_new(of_version_t version)
{
    of_oxm_ofdpa_l3_in_port_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_L3_IN_PORT] + of_object_extra_len[version][OF_OXM_OFDPA_L3_IN_PORT];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_l3_in_port_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_l3_in_port_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_l3_in_port_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_l3_in_port.
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
of_oxm_ofdpa_l3_in_port_init(of_oxm_ofdpa_l3_in_port_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_L3_IN_PORT] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_L3_IN_PORT] + of_object_extra_len[version][OF_OXM_OFDPA_L3_IN_PORT];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_L3_IN_PORT;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_l3_in_port_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_l3_in_port.
 * @param obj Pointer to an object of type of_oxm_ofdpa_l3_in_port.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_l3_in_port_experimenter_get(
    of_oxm_ofdpa_l3_in_port_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_L3_IN_PORT);
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
 * Set experimenter in an object of type of_oxm_ofdpa_l3_in_port.
 * @param obj Pointer to an object of type of_oxm_ofdpa_l3_in_port.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_l3_in_port_experimenter_set(
    of_oxm_ofdpa_l3_in_port_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_L3_IN_PORT);
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
 * Get value from an object of type of_oxm_ofdpa_l3_in_port.
 * @param obj Pointer to an object of type of_oxm_ofdpa_l3_in_port.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_l3_in_port_value_get(
    of_oxm_ofdpa_l3_in_port_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_L3_IN_PORT);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_ofdpa_l3_in_port.
 * @param obj Pointer to an object of type of_oxm_ofdpa_l3_in_port.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_l3_in_port_value_set(
    of_oxm_ofdpa_l3_in_port_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_L3_IN_PORT);
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

static void
of_oxm_ofdpa_lmep_id_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff0c08); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_lmep_id of_oxm_ofdpa_lmep_id
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_lmep_id_push_wire_values(of_oxm_ofdpa_lmep_id_t *obj)
{

    of_oxm_ofdpa_lmep_id_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_lmep_id object
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
 * \ingroup of_oxm_ofdpa_lmep_id
 */

of_oxm_ofdpa_lmep_id_t *
of_oxm_ofdpa_lmep_id_new(of_version_t version)
{
    of_oxm_ofdpa_lmep_id_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_LMEP_ID] + of_object_extra_len[version][OF_OXM_OFDPA_LMEP_ID];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_lmep_id_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_lmep_id_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_lmep_id_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_lmep_id.
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
of_oxm_ofdpa_lmep_id_init(of_oxm_ofdpa_lmep_id_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_LMEP_ID] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_LMEP_ID] + of_object_extra_len[version][OF_OXM_OFDPA_LMEP_ID];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_LMEP_ID;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_lmep_id_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_lmep_id.
 * @param obj Pointer to an object of type of_oxm_ofdpa_lmep_id.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_lmep_id_experimenter_get(
    of_oxm_ofdpa_lmep_id_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_LMEP_ID);
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
 * Set experimenter in an object of type of_oxm_ofdpa_lmep_id.
 * @param obj Pointer to an object of type of_oxm_ofdpa_lmep_id.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_lmep_id_experimenter_set(
    of_oxm_ofdpa_lmep_id_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_LMEP_ID);
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
 * Get value from an object of type of_oxm_ofdpa_lmep_id.
 * @param obj Pointer to an object of type of_oxm_ofdpa_lmep_id.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_lmep_id_value_get(
    of_oxm_ofdpa_lmep_id_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_LMEP_ID);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_ofdpa_lmep_id.
 * @param obj Pointer to an object of type of_oxm_ofdpa_lmep_id.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_lmep_id_value_set(
    of_oxm_ofdpa_lmep_id_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_LMEP_ID);
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

static void
of_oxm_ofdpa_mpls_ach_channel_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff1806); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_mpls_ach_channel of_oxm_ofdpa_mpls_ach_channel
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_mpls_ach_channel_push_wire_values(of_oxm_ofdpa_mpls_ach_channel_t *obj)
{

    of_oxm_ofdpa_mpls_ach_channel_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_mpls_ach_channel object
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
 * \ingroup of_oxm_ofdpa_mpls_ach_channel
 */

of_oxm_ofdpa_mpls_ach_channel_t *
of_oxm_ofdpa_mpls_ach_channel_new(of_version_t version)
{
    of_oxm_ofdpa_mpls_ach_channel_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_MPLS_ACH_CHANNEL] + of_object_extra_len[version][OF_OXM_OFDPA_MPLS_ACH_CHANNEL];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_mpls_ach_channel_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_mpls_ach_channel_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_mpls_ach_channel_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_mpls_ach_channel.
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
of_oxm_ofdpa_mpls_ach_channel_init(of_oxm_ofdpa_mpls_ach_channel_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_MPLS_ACH_CHANNEL] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_MPLS_ACH_CHANNEL] + of_object_extra_len[version][OF_OXM_OFDPA_MPLS_ACH_CHANNEL];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_MPLS_ACH_CHANNEL;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_mpls_ach_channel_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_mpls_ach_channel.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_ach_channel.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_mpls_ach_channel_experimenter_get(
    of_oxm_ofdpa_mpls_ach_channel_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_ACH_CHANNEL);
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
 * Set experimenter in an object of type of_oxm_ofdpa_mpls_ach_channel.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_ach_channel.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_mpls_ach_channel_experimenter_set(
    of_oxm_ofdpa_mpls_ach_channel_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_ACH_CHANNEL);
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
 * Get value from an object of type of_oxm_ofdpa_mpls_ach_channel.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_ach_channel.
 * @param value Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_oxm_ofdpa_mpls_ach_channel_value_get(
    of_oxm_ofdpa_mpls_ach_channel_t *obj,
    uint16_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_ACH_CHANNEL);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_ofdpa_mpls_ach_channel.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_ach_channel.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_mpls_ach_channel_value_set(
    of_oxm_ofdpa_mpls_ach_channel_t *obj,
    uint16_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_ACH_CHANNEL);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, value);

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
of_oxm_ofdpa_mpls_data_first_nibble_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff1605); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_mpls_data_first_nibble of_oxm_ofdpa_mpls_data_first_nibble
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_mpls_data_first_nibble_push_wire_values(of_oxm_ofdpa_mpls_data_first_nibble_t *obj)
{

    of_oxm_ofdpa_mpls_data_first_nibble_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_mpls_data_first_nibble object
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
 * \ingroup of_oxm_ofdpa_mpls_data_first_nibble
 */

of_oxm_ofdpa_mpls_data_first_nibble_t *
of_oxm_ofdpa_mpls_data_first_nibble_new(of_version_t version)
{
    of_oxm_ofdpa_mpls_data_first_nibble_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_MPLS_DATA_FIRST_NIBBLE] + of_object_extra_len[version][OF_OXM_OFDPA_MPLS_DATA_FIRST_NIBBLE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_mpls_data_first_nibble_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_mpls_data_first_nibble_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_mpls_data_first_nibble_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_mpls_data_first_nibble.
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
of_oxm_ofdpa_mpls_data_first_nibble_init(of_oxm_ofdpa_mpls_data_first_nibble_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_MPLS_DATA_FIRST_NIBBLE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_MPLS_DATA_FIRST_NIBBLE] + of_object_extra_len[version][OF_OXM_OFDPA_MPLS_DATA_FIRST_NIBBLE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_MPLS_DATA_FIRST_NIBBLE;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_mpls_data_first_nibble_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_mpls_data_first_nibble.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_data_first_nibble.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_mpls_data_first_nibble_experimenter_get(
    of_oxm_ofdpa_mpls_data_first_nibble_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_DATA_FIRST_NIBBLE);
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
 * Set experimenter in an object of type of_oxm_ofdpa_mpls_data_first_nibble.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_data_first_nibble.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_mpls_data_first_nibble_experimenter_set(
    of_oxm_ofdpa_mpls_data_first_nibble_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_DATA_FIRST_NIBBLE);
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
 * Get value from an object of type of_oxm_ofdpa_mpls_data_first_nibble.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_data_first_nibble.
 * @param value Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_oxm_ofdpa_mpls_data_first_nibble_value_get(
    of_oxm_ofdpa_mpls_data_first_nibble_t *obj,
    uint8_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_DATA_FIRST_NIBBLE);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_ofdpa_mpls_data_first_nibble.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_data_first_nibble.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_mpls_data_first_nibble_value_set(
    of_oxm_ofdpa_mpls_data_first_nibble_t *obj,
    uint8_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_DATA_FIRST_NIBBLE);
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

static void
of_oxm_ofdpa_mpls_l2_port_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff1008); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_mpls_l2_port of_oxm_ofdpa_mpls_l2_port
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_mpls_l2_port_push_wire_values(of_oxm_ofdpa_mpls_l2_port_t *obj)
{

    of_oxm_ofdpa_mpls_l2_port_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_mpls_l2_port object
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
 * \ingroup of_oxm_ofdpa_mpls_l2_port
 */

of_oxm_ofdpa_mpls_l2_port_t *
of_oxm_ofdpa_mpls_l2_port_new(of_version_t version)
{
    of_oxm_ofdpa_mpls_l2_port_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_MPLS_L2_PORT] + of_object_extra_len[version][OF_OXM_OFDPA_MPLS_L2_PORT];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_mpls_l2_port_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_mpls_l2_port_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_mpls_l2_port_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_mpls_l2_port.
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
of_oxm_ofdpa_mpls_l2_port_init(of_oxm_ofdpa_mpls_l2_port_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_MPLS_L2_PORT] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_MPLS_L2_PORT] + of_object_extra_len[version][OF_OXM_OFDPA_MPLS_L2_PORT];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_MPLS_L2_PORT;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_mpls_l2_port_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_mpls_l2_port.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_l2_port.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_mpls_l2_port_experimenter_get(
    of_oxm_ofdpa_mpls_l2_port_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_L2_PORT);
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
 * Set experimenter in an object of type of_oxm_ofdpa_mpls_l2_port.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_l2_port.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_mpls_l2_port_experimenter_set(
    of_oxm_ofdpa_mpls_l2_port_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_L2_PORT);
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
 * Get value from an object of type of_oxm_ofdpa_mpls_l2_port.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_l2_port.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_mpls_l2_port_value_get(
    of_oxm_ofdpa_mpls_l2_port_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_L2_PORT);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_ofdpa_mpls_l2_port.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_l2_port.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_mpls_l2_port_value_set(
    of_oxm_ofdpa_mpls_l2_port_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_L2_PORT);
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

static void
of_oxm_ofdpa_mpls_l2_port_masked_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff110c); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_mpls_l2_port_masked of_oxm_ofdpa_mpls_l2_port_masked
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_mpls_l2_port_masked_push_wire_values(of_oxm_ofdpa_mpls_l2_port_masked_t *obj)
{

    of_oxm_ofdpa_mpls_l2_port_masked_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_mpls_l2_port_masked object
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
 * \ingroup of_oxm_ofdpa_mpls_l2_port_masked
 */

of_oxm_ofdpa_mpls_l2_port_masked_t *
of_oxm_ofdpa_mpls_l2_port_masked_new(of_version_t version)
{
    of_oxm_ofdpa_mpls_l2_port_masked_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_MPLS_L2_PORT_MASKED] + of_object_extra_len[version][OF_OXM_OFDPA_MPLS_L2_PORT_MASKED];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_mpls_l2_port_masked_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_mpls_l2_port_masked_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_mpls_l2_port_masked_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_mpls_l2_port_masked.
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
of_oxm_ofdpa_mpls_l2_port_masked_init(of_oxm_ofdpa_mpls_l2_port_masked_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_MPLS_L2_PORT_MASKED] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_MPLS_L2_PORT_MASKED] + of_object_extra_len[version][OF_OXM_OFDPA_MPLS_L2_PORT_MASKED];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_MPLS_L2_PORT_MASKED;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_mpls_l2_port_masked_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_mpls_l2_port_masked.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_l2_port_masked.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_mpls_l2_port_masked_experimenter_get(
    of_oxm_ofdpa_mpls_l2_port_masked_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_L2_PORT_MASKED);
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
 * Set experimenter in an object of type of_oxm_ofdpa_mpls_l2_port_masked.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_l2_port_masked.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_mpls_l2_port_masked_experimenter_set(
    of_oxm_ofdpa_mpls_l2_port_masked_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_L2_PORT_MASKED);
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
 * Get value from an object of type of_oxm_ofdpa_mpls_l2_port_masked.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_l2_port_masked.
 * @param value Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_mpls_l2_port_masked_value_get(
    of_oxm_ofdpa_mpls_l2_port_masked_t *obj,
    uint32_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_L2_PORT_MASKED);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_ofdpa_mpls_l2_port_masked.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_l2_port_masked.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_mpls_l2_port_masked_value_set(
    of_oxm_ofdpa_mpls_l2_port_masked_t *obj,
    uint32_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_L2_PORT_MASKED);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Get value_mask from an object of type of_oxm_ofdpa_mpls_l2_port_masked.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_l2_port_masked.
 * @param value_mask Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_mpls_l2_port_masked_value_mask_get(
    of_oxm_ofdpa_mpls_l2_port_masked_t *obj,
    uint32_t *value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_L2_PORT_MASKED);
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
    of_wire_buffer_u32_get(wbuf, abs_offset, value_mask);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value_mask in an object of type of_oxm_ofdpa_mpls_l2_port_masked.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_l2_port_masked.
 * @param value_mask The value to write into the object
 */
void
of_oxm_ofdpa_mpls_l2_port_masked_value_mask_set(
    of_oxm_ofdpa_mpls_l2_port_masked_t *obj,
    uint32_t value_mask)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_L2_PORT_MASKED);
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
    of_wire_buffer_u32_set(wbuf, abs_offset, value_mask);

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
of_oxm_ofdpa_mpls_next_label_is_gal_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff1a05); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_mpls_next_label_is_gal of_oxm_ofdpa_mpls_next_label_is_gal
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_mpls_next_label_is_gal_push_wire_values(of_oxm_ofdpa_mpls_next_label_is_gal_t *obj)
{

    of_oxm_ofdpa_mpls_next_label_is_gal_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_mpls_next_label_is_gal object
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
 * \ingroup of_oxm_ofdpa_mpls_next_label_is_gal
 */

of_oxm_ofdpa_mpls_next_label_is_gal_t *
of_oxm_ofdpa_mpls_next_label_is_gal_new(of_version_t version)
{
    of_oxm_ofdpa_mpls_next_label_is_gal_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_MPLS_NEXT_LABEL_IS_GAL] + of_object_extra_len[version][OF_OXM_OFDPA_MPLS_NEXT_LABEL_IS_GAL];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_mpls_next_label_is_gal_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_mpls_next_label_is_gal_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_mpls_next_label_is_gal_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_mpls_next_label_is_gal.
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
of_oxm_ofdpa_mpls_next_label_is_gal_init(of_oxm_ofdpa_mpls_next_label_is_gal_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_MPLS_NEXT_LABEL_IS_GAL] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_MPLS_NEXT_LABEL_IS_GAL] + of_object_extra_len[version][OF_OXM_OFDPA_MPLS_NEXT_LABEL_IS_GAL];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_MPLS_NEXT_LABEL_IS_GAL;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_mpls_next_label_is_gal_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_mpls_next_label_is_gal.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_next_label_is_gal.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_mpls_next_label_is_gal_experimenter_get(
    of_oxm_ofdpa_mpls_next_label_is_gal_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_NEXT_LABEL_IS_GAL);
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
 * Set experimenter in an object of type of_oxm_ofdpa_mpls_next_label_is_gal.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_next_label_is_gal.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_mpls_next_label_is_gal_experimenter_set(
    of_oxm_ofdpa_mpls_next_label_is_gal_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_NEXT_LABEL_IS_GAL);
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
 * Get value from an object of type of_oxm_ofdpa_mpls_next_label_is_gal.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_next_label_is_gal.
 * @param value Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_oxm_ofdpa_mpls_next_label_is_gal_value_get(
    of_oxm_ofdpa_mpls_next_label_is_gal_t *obj,
    uint8_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_NEXT_LABEL_IS_GAL);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_ofdpa_mpls_next_label_is_gal.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_next_label_is_gal.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_mpls_next_label_is_gal_value_set(
    of_oxm_ofdpa_mpls_next_label_is_gal_t *obj,
    uint8_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_NEXT_LABEL_IS_GAL);
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

static void
of_oxm_ofdpa_mpls_ttl_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff0e05); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_mpls_ttl of_oxm_ofdpa_mpls_ttl
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_mpls_ttl_push_wire_values(of_oxm_ofdpa_mpls_ttl_t *obj)
{

    of_oxm_ofdpa_mpls_ttl_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_mpls_ttl object
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
 * \ingroup of_oxm_ofdpa_mpls_ttl
 */

of_oxm_ofdpa_mpls_ttl_t *
of_oxm_ofdpa_mpls_ttl_new(of_version_t version)
{
    of_oxm_ofdpa_mpls_ttl_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_MPLS_TTL] + of_object_extra_len[version][OF_OXM_OFDPA_MPLS_TTL];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_mpls_ttl_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_mpls_ttl_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_mpls_ttl_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_mpls_ttl.
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
of_oxm_ofdpa_mpls_ttl_init(of_oxm_ofdpa_mpls_ttl_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_MPLS_TTL] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_MPLS_TTL] + of_object_extra_len[version][OF_OXM_OFDPA_MPLS_TTL];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_MPLS_TTL;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_mpls_ttl_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_mpls_ttl.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_ttl.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_mpls_ttl_experimenter_get(
    of_oxm_ofdpa_mpls_ttl_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_TTL);
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
 * Set experimenter in an object of type of_oxm_ofdpa_mpls_ttl.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_ttl.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_mpls_ttl_experimenter_set(
    of_oxm_ofdpa_mpls_ttl_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_TTL);
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
 * Get value from an object of type of_oxm_ofdpa_mpls_ttl.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_ttl.
 * @param value Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_oxm_ofdpa_mpls_ttl_value_get(
    of_oxm_ofdpa_mpls_ttl_t *obj,
    uint8_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_TTL);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_ofdpa_mpls_ttl.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_ttl.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_mpls_ttl_value_set(
    of_oxm_ofdpa_mpls_ttl_t *obj,
    uint8_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_TTL);
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

static void
of_oxm_ofdpa_mpls_type_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff2e06); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_mpls_type of_oxm_ofdpa_mpls_type
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_mpls_type_push_wire_values(of_oxm_ofdpa_mpls_type_t *obj)
{

    of_oxm_ofdpa_mpls_type_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_mpls_type object
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
 * \ingroup of_oxm_ofdpa_mpls_type
 */

of_oxm_ofdpa_mpls_type_t *
of_oxm_ofdpa_mpls_type_new(of_version_t version)
{
    of_oxm_ofdpa_mpls_type_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_MPLS_TYPE] + of_object_extra_len[version][OF_OXM_OFDPA_MPLS_TYPE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_mpls_type_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_mpls_type_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_mpls_type_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_mpls_type.
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
of_oxm_ofdpa_mpls_type_init(of_oxm_ofdpa_mpls_type_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_MPLS_TYPE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_MPLS_TYPE] + of_object_extra_len[version][OF_OXM_OFDPA_MPLS_TYPE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_MPLS_TYPE;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_mpls_type_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_mpls_type.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_type.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_mpls_type_experimenter_get(
    of_oxm_ofdpa_mpls_type_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_TYPE);
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
 * Set experimenter in an object of type of_oxm_ofdpa_mpls_type.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_type.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_mpls_type_experimenter_set(
    of_oxm_ofdpa_mpls_type_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_TYPE);
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
 * Get value from an object of type of_oxm_ofdpa_mpls_type.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_type.
 * @param value Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_oxm_ofdpa_mpls_type_value_get(
    of_oxm_ofdpa_mpls_type_t *obj,
    uint16_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_TYPE);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_ofdpa_mpls_type.
 * @param obj Pointer to an object of type of_oxm_ofdpa_mpls_type.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_mpls_type_value_set(
    of_oxm_ofdpa_mpls_type_t *obj,
    uint16_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_MPLS_TYPE);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, value);

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
of_oxm_ofdpa_oam_y1731_mdl_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff1c05); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_oam_y1731_mdl of_oxm_ofdpa_oam_y1731_mdl
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_oam_y1731_mdl_push_wire_values(of_oxm_ofdpa_oam_y1731_mdl_t *obj)
{

    of_oxm_ofdpa_oam_y1731_mdl_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_oam_y1731_mdl object
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
 * \ingroup of_oxm_ofdpa_oam_y1731_mdl
 */

of_oxm_ofdpa_oam_y1731_mdl_t *
of_oxm_ofdpa_oam_y1731_mdl_new(of_version_t version)
{
    of_oxm_ofdpa_oam_y1731_mdl_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_OAM_Y1731_MDL] + of_object_extra_len[version][OF_OXM_OFDPA_OAM_Y1731_MDL];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_oam_y1731_mdl_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_oam_y1731_mdl_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_oam_y1731_mdl_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_oam_y1731_mdl.
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
of_oxm_ofdpa_oam_y1731_mdl_init(of_oxm_ofdpa_oam_y1731_mdl_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_OAM_Y1731_MDL] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_OAM_Y1731_MDL] + of_object_extra_len[version][OF_OXM_OFDPA_OAM_Y1731_MDL];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_OAM_Y1731_MDL;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_oam_y1731_mdl_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_oam_y1731_mdl.
 * @param obj Pointer to an object of type of_oxm_ofdpa_oam_y1731_mdl.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_oam_y1731_mdl_experimenter_get(
    of_oxm_ofdpa_oam_y1731_mdl_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_OAM_Y1731_MDL);
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
 * Set experimenter in an object of type of_oxm_ofdpa_oam_y1731_mdl.
 * @param obj Pointer to an object of type of_oxm_ofdpa_oam_y1731_mdl.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_oam_y1731_mdl_experimenter_set(
    of_oxm_ofdpa_oam_y1731_mdl_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_OAM_Y1731_MDL);
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
 * Get value from an object of type of_oxm_ofdpa_oam_y1731_mdl.
 * @param obj Pointer to an object of type of_oxm_ofdpa_oam_y1731_mdl.
 * @param value Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_oxm_ofdpa_oam_y1731_mdl_value_get(
    of_oxm_ofdpa_oam_y1731_mdl_t *obj,
    uint8_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_OAM_Y1731_MDL);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_ofdpa_oam_y1731_mdl.
 * @param obj Pointer to an object of type of_oxm_ofdpa_oam_y1731_mdl.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_oam_y1731_mdl_value_set(
    of_oxm_ofdpa_oam_y1731_mdl_t *obj,
    uint8_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_OAM_Y1731_MDL);
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

static void
of_oxm_ofdpa_oam_y1731_opcode_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff1e05); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_oam_y1731_opcode of_oxm_ofdpa_oam_y1731_opcode
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_oam_y1731_opcode_push_wire_values(of_oxm_ofdpa_oam_y1731_opcode_t *obj)
{

    of_oxm_ofdpa_oam_y1731_opcode_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_oam_y1731_opcode object
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
 * \ingroup of_oxm_ofdpa_oam_y1731_opcode
 */

of_oxm_ofdpa_oam_y1731_opcode_t *
of_oxm_ofdpa_oam_y1731_opcode_new(of_version_t version)
{
    of_oxm_ofdpa_oam_y1731_opcode_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_OAM_Y1731_OPCODE] + of_object_extra_len[version][OF_OXM_OFDPA_OAM_Y1731_OPCODE];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_oam_y1731_opcode_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_oam_y1731_opcode_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_oam_y1731_opcode_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_oam_y1731_opcode.
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
of_oxm_ofdpa_oam_y1731_opcode_init(of_oxm_ofdpa_oam_y1731_opcode_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_OAM_Y1731_OPCODE] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_OAM_Y1731_OPCODE] + of_object_extra_len[version][OF_OXM_OFDPA_OAM_Y1731_OPCODE];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_OAM_Y1731_OPCODE;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_oam_y1731_opcode_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_oam_y1731_opcode.
 * @param obj Pointer to an object of type of_oxm_ofdpa_oam_y1731_opcode.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_oam_y1731_opcode_experimenter_get(
    of_oxm_ofdpa_oam_y1731_opcode_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_OAM_Y1731_OPCODE);
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
 * Set experimenter in an object of type of_oxm_ofdpa_oam_y1731_opcode.
 * @param obj Pointer to an object of type of_oxm_ofdpa_oam_y1731_opcode.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_oam_y1731_opcode_experimenter_set(
    of_oxm_ofdpa_oam_y1731_opcode_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_OAM_Y1731_OPCODE);
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
 * Get value from an object of type of_oxm_ofdpa_oam_y1731_opcode.
 * @param obj Pointer to an object of type of_oxm_ofdpa_oam_y1731_opcode.
 * @param value Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_oxm_ofdpa_oam_y1731_opcode_value_get(
    of_oxm_ofdpa_oam_y1731_opcode_t *obj,
    uint8_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_OAM_Y1731_OPCODE);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_ofdpa_oam_y1731_opcode.
 * @param obj Pointer to an object of type of_oxm_ofdpa_oam_y1731_opcode.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_oam_y1731_opcode_value_set(
    of_oxm_ofdpa_oam_y1731_opcode_t *obj,
    uint8_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_OAM_Y1731_OPCODE);
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

static void
of_oxm_ofdpa_ovid_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff1406); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_ovid of_oxm_ofdpa_ovid
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_ovid_push_wire_values(of_oxm_ofdpa_ovid_t *obj)
{

    of_oxm_ofdpa_ovid_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_ovid object
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
 * \ingroup of_oxm_ofdpa_ovid
 */

of_oxm_ofdpa_ovid_t *
of_oxm_ofdpa_ovid_new(of_version_t version)
{
    of_oxm_ofdpa_ovid_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_OVID] + of_object_extra_len[version][OF_OXM_OFDPA_OVID];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_ovid_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_ovid_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_ovid_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_ovid.
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
of_oxm_ofdpa_ovid_init(of_oxm_ofdpa_ovid_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_OVID] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_OVID] + of_object_extra_len[version][OF_OXM_OFDPA_OVID];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_OVID;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_ovid_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_ovid.
 * @param obj Pointer to an object of type of_oxm_ofdpa_ovid.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_ovid_experimenter_get(
    of_oxm_ofdpa_ovid_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_OVID);
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
 * Set experimenter in an object of type of_oxm_ofdpa_ovid.
 * @param obj Pointer to an object of type of_oxm_ofdpa_ovid.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_ovid_experimenter_set(
    of_oxm_ofdpa_ovid_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_OVID);
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
 * Get value from an object of type of_oxm_ofdpa_ovid.
 * @param obj Pointer to an object of type of_oxm_ofdpa_ovid.
 * @param value Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_oxm_ofdpa_ovid_value_get(
    of_oxm_ofdpa_ovid_t *obj,
    uint16_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_OVID);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_ofdpa_ovid.
 * @param obj Pointer to an object of type of_oxm_ofdpa_ovid.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_ovid_value_set(
    of_oxm_ofdpa_ovid_t *obj,
    uint16_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_OVID);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, value);

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
of_oxm_ofdpa_protection_index_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff2a05); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_protection_index of_oxm_ofdpa_protection_index
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_protection_index_push_wire_values(of_oxm_ofdpa_protection_index_t *obj)
{

    of_oxm_ofdpa_protection_index_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_protection_index object
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
 * \ingroup of_oxm_ofdpa_protection_index
 */

of_oxm_ofdpa_protection_index_t *
of_oxm_ofdpa_protection_index_new(of_version_t version)
{
    of_oxm_ofdpa_protection_index_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_PROTECTION_INDEX] + of_object_extra_len[version][OF_OXM_OFDPA_PROTECTION_INDEX];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_protection_index_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_protection_index_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_protection_index_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_protection_index.
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
of_oxm_ofdpa_protection_index_init(of_oxm_ofdpa_protection_index_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_PROTECTION_INDEX] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_PROTECTION_INDEX] + of_object_extra_len[version][OF_OXM_OFDPA_PROTECTION_INDEX];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_PROTECTION_INDEX;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_protection_index_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_protection_index.
 * @param obj Pointer to an object of type of_oxm_ofdpa_protection_index.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_protection_index_experimenter_get(
    of_oxm_ofdpa_protection_index_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_PROTECTION_INDEX);
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
 * Set experimenter in an object of type of_oxm_ofdpa_protection_index.
 * @param obj Pointer to an object of type of_oxm_ofdpa_protection_index.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_protection_index_experimenter_set(
    of_oxm_ofdpa_protection_index_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_PROTECTION_INDEX);
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
 * Get value from an object of type of_oxm_ofdpa_protection_index.
 * @param obj Pointer to an object of type of_oxm_ofdpa_protection_index.
 * @param value Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_oxm_ofdpa_protection_index_value_get(
    of_oxm_ofdpa_protection_index_t *obj,
    uint8_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_PROTECTION_INDEX);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_ofdpa_protection_index.
 * @param obj Pointer to an object of type of_oxm_ofdpa_protection_index.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_protection_index_value_set(
    of_oxm_ofdpa_protection_index_t *obj,
    uint8_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_PROTECTION_INDEX);
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

static void
of_oxm_ofdpa_qos_index_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff0a05); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_qos_index of_oxm_ofdpa_qos_index
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_qos_index_push_wire_values(of_oxm_ofdpa_qos_index_t *obj)
{

    of_oxm_ofdpa_qos_index_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_qos_index object
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
 * \ingroup of_oxm_ofdpa_qos_index
 */

of_oxm_ofdpa_qos_index_t *
of_oxm_ofdpa_qos_index_new(of_version_t version)
{
    of_oxm_ofdpa_qos_index_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_QOS_INDEX] + of_object_extra_len[version][OF_OXM_OFDPA_QOS_INDEX];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_qos_index_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_qos_index_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_qos_index_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_qos_index.
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
of_oxm_ofdpa_qos_index_init(of_oxm_ofdpa_qos_index_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_QOS_INDEX] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_QOS_INDEX] + of_object_extra_len[version][OF_OXM_OFDPA_QOS_INDEX];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_QOS_INDEX;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_qos_index_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_qos_index.
 * @param obj Pointer to an object of type of_oxm_ofdpa_qos_index.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_qos_index_experimenter_get(
    of_oxm_ofdpa_qos_index_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_QOS_INDEX);
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
 * Set experimenter in an object of type of_oxm_ofdpa_qos_index.
 * @param obj Pointer to an object of type of_oxm_ofdpa_qos_index.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_qos_index_experimenter_set(
    of_oxm_ofdpa_qos_index_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_QOS_INDEX);
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
 * Get value from an object of type of_oxm_ofdpa_qos_index.
 * @param obj Pointer to an object of type of_oxm_ofdpa_qos_index.
 * @param value Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_oxm_ofdpa_qos_index_value_get(
    of_oxm_ofdpa_qos_index_t *obj,
    uint8_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_QOS_INDEX);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_ofdpa_qos_index.
 * @param obj Pointer to an object of type of_oxm_ofdpa_qos_index.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_qos_index_value_set(
    of_oxm_ofdpa_qos_index_t *obj,
    uint8_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_QOS_INDEX);
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

static void
of_oxm_ofdpa_rx_timestamp_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff260c); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_rx_timestamp of_oxm_ofdpa_rx_timestamp
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_rx_timestamp_push_wire_values(of_oxm_ofdpa_rx_timestamp_t *obj)
{

    of_oxm_ofdpa_rx_timestamp_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_rx_timestamp object
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
 * \ingroup of_oxm_ofdpa_rx_timestamp
 */

of_oxm_ofdpa_rx_timestamp_t *
of_oxm_ofdpa_rx_timestamp_new(of_version_t version)
{
    of_oxm_ofdpa_rx_timestamp_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_RX_TIMESTAMP] + of_object_extra_len[version][OF_OXM_OFDPA_RX_TIMESTAMP];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_rx_timestamp_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_rx_timestamp_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_rx_timestamp_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_rx_timestamp.
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
of_oxm_ofdpa_rx_timestamp_init(of_oxm_ofdpa_rx_timestamp_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_RX_TIMESTAMP] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_RX_TIMESTAMP] + of_object_extra_len[version][OF_OXM_OFDPA_RX_TIMESTAMP];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_RX_TIMESTAMP;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_rx_timestamp_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_rx_timestamp.
 * @param obj Pointer to an object of type of_oxm_ofdpa_rx_timestamp.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_rx_timestamp_experimenter_get(
    of_oxm_ofdpa_rx_timestamp_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_RX_TIMESTAMP);
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
 * Set experimenter in an object of type of_oxm_ofdpa_rx_timestamp.
 * @param obj Pointer to an object of type of_oxm_ofdpa_rx_timestamp.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_rx_timestamp_experimenter_set(
    of_oxm_ofdpa_rx_timestamp_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_RX_TIMESTAMP);
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
 * Get value from an object of type of_oxm_ofdpa_rx_timestamp.
 * @param obj Pointer to an object of type of_oxm_ofdpa_rx_timestamp.
 * @param value Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_oxm_ofdpa_rx_timestamp_value_get(
    of_oxm_ofdpa_rx_timestamp_t *obj,
    uint64_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_RX_TIMESTAMP);
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
 * Set value in an object of type of_oxm_ofdpa_rx_timestamp.
 * @param obj Pointer to an object of type of_oxm_ofdpa_rx_timestamp.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_rx_timestamp_value_set(
    of_oxm_ofdpa_rx_timestamp_t *obj,
    uint64_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_RX_TIMESTAMP);
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
of_oxm_ofdpa_rxfcl_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff240c); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_rxfcl of_oxm_ofdpa_rxfcl
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_rxfcl_push_wire_values(of_oxm_ofdpa_rxfcl_t *obj)
{

    of_oxm_ofdpa_rxfcl_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_rxfcl object
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
 * \ingroup of_oxm_ofdpa_rxfcl
 */

of_oxm_ofdpa_rxfcl_t *
of_oxm_ofdpa_rxfcl_new(of_version_t version)
{
    of_oxm_ofdpa_rxfcl_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_RXFCL] + of_object_extra_len[version][OF_OXM_OFDPA_RXFCL];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_rxfcl_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_rxfcl_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_rxfcl_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_rxfcl.
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
of_oxm_ofdpa_rxfcl_init(of_oxm_ofdpa_rxfcl_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_RXFCL] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_RXFCL] + of_object_extra_len[version][OF_OXM_OFDPA_RXFCL];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_RXFCL;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_rxfcl_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_rxfcl.
 * @param obj Pointer to an object of type of_oxm_ofdpa_rxfcl.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_rxfcl_experimenter_get(
    of_oxm_ofdpa_rxfcl_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_RXFCL);
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
 * Set experimenter in an object of type of_oxm_ofdpa_rxfcl.
 * @param obj Pointer to an object of type of_oxm_ofdpa_rxfcl.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_rxfcl_experimenter_set(
    of_oxm_ofdpa_rxfcl_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_RXFCL);
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
 * Get value from an object of type of_oxm_ofdpa_rxfcl.
 * @param obj Pointer to an object of type of_oxm_ofdpa_rxfcl.
 * @param value Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_oxm_ofdpa_rxfcl_value_get(
    of_oxm_ofdpa_rxfcl_t *obj,
    uint64_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_RXFCL);
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
 * Set value in an object of type of_oxm_ofdpa_rxfcl.
 * @param obj Pointer to an object of type of_oxm_ofdpa_rxfcl.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_rxfcl_value_set(
    of_oxm_ofdpa_rxfcl_t *obj,
    uint64_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_RXFCL);
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
of_oxm_ofdpa_traffic_class_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff0405); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_traffic_class of_oxm_ofdpa_traffic_class
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_traffic_class_push_wire_values(of_oxm_ofdpa_traffic_class_t *obj)
{

    of_oxm_ofdpa_traffic_class_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_traffic_class object
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
 * \ingroup of_oxm_ofdpa_traffic_class
 */

of_oxm_ofdpa_traffic_class_t *
of_oxm_ofdpa_traffic_class_new(of_version_t version)
{
    of_oxm_ofdpa_traffic_class_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_TRAFFIC_CLASS] + of_object_extra_len[version][OF_OXM_OFDPA_TRAFFIC_CLASS];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_traffic_class_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_traffic_class_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_traffic_class_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_traffic_class.
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
of_oxm_ofdpa_traffic_class_init(of_oxm_ofdpa_traffic_class_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_TRAFFIC_CLASS] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_TRAFFIC_CLASS] + of_object_extra_len[version][OF_OXM_OFDPA_TRAFFIC_CLASS];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_TRAFFIC_CLASS;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_traffic_class_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_traffic_class.
 * @param obj Pointer to an object of type of_oxm_ofdpa_traffic_class.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_traffic_class_experimenter_get(
    of_oxm_ofdpa_traffic_class_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_TRAFFIC_CLASS);
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
 * Set experimenter in an object of type of_oxm_ofdpa_traffic_class.
 * @param obj Pointer to an object of type of_oxm_ofdpa_traffic_class.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_traffic_class_experimenter_set(
    of_oxm_ofdpa_traffic_class_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_TRAFFIC_CLASS);
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
 * Get value from an object of type of_oxm_ofdpa_traffic_class.
 * @param obj Pointer to an object of type of_oxm_ofdpa_traffic_class.
 * @param value Pointer to the child object of type
 * uint8_t to be filled out.
 *
 */
void
of_oxm_ofdpa_traffic_class_value_get(
    of_oxm_ofdpa_traffic_class_t *obj,
    uint8_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_TRAFFIC_CLASS);
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
    of_wire_buffer_u8_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_ofdpa_traffic_class.
 * @param obj Pointer to an object of type of_oxm_ofdpa_traffic_class.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_traffic_class_value_set(
    of_oxm_ofdpa_traffic_class_t *obj,
    uint8_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_TRAFFIC_CLASS);
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

static void
of_oxm_ofdpa_txfcl_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff220c); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_txfcl of_oxm_ofdpa_txfcl
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_txfcl_push_wire_values(of_oxm_ofdpa_txfcl_t *obj)
{

    of_oxm_ofdpa_txfcl_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_txfcl object
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
 * \ingroup of_oxm_ofdpa_txfcl
 */

of_oxm_ofdpa_txfcl_t *
of_oxm_ofdpa_txfcl_new(of_version_t version)
{
    of_oxm_ofdpa_txfcl_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_TXFCL] + of_object_extra_len[version][OF_OXM_OFDPA_TXFCL];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_txfcl_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_txfcl_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_txfcl_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_txfcl.
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
of_oxm_ofdpa_txfcl_init(of_oxm_ofdpa_txfcl_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_TXFCL] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_TXFCL] + of_object_extra_len[version][OF_OXM_OFDPA_TXFCL];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_TXFCL;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_txfcl_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_txfcl.
 * @param obj Pointer to an object of type of_oxm_ofdpa_txfcl.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_txfcl_experimenter_get(
    of_oxm_ofdpa_txfcl_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_TXFCL);
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
 * Set experimenter in an object of type of_oxm_ofdpa_txfcl.
 * @param obj Pointer to an object of type of_oxm_ofdpa_txfcl.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_txfcl_experimenter_set(
    of_oxm_ofdpa_txfcl_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_TXFCL);
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
 * Get value from an object of type of_oxm_ofdpa_txfcl.
 * @param obj Pointer to an object of type of_oxm_ofdpa_txfcl.
 * @param value Pointer to the child object of type
 * uint64_t to be filled out.
 *
 */
void
of_oxm_ofdpa_txfcl_value_get(
    of_oxm_ofdpa_txfcl_t *obj,
    uint64_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_TXFCL);
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
 * Set value in an object of type of_oxm_ofdpa_txfcl.
 * @param obj Pointer to an object of type of_oxm_ofdpa_txfcl.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_txfcl_value_set(
    of_oxm_ofdpa_txfcl_t *obj,
    uint64_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_TXFCL);
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
of_oxm_ofdpa_vrf_push_wire_types(of_object_t *obj)
{
    unsigned char *buf = OF_OBJECT_BUFFER_INDEX(obj, 0);
    switch (obj->version) {
    case OF_VERSION_1_3:
        *(uint32_t *)(buf + 0) = U32_HTON(0xffff0206); /* type_len */
        *(uint32_t *)(buf + 4) = U32_HTON(0x1018); /* experimenter */
        break;
    default:
        UNREACHABLE();
    }
}



/**
 * \defgroup of_oxm_ofdpa_vrf of_oxm_ofdpa_vrf
 */

/**
 * Helper function to push values into the wire buffer
 */
static inline int
of_oxm_ofdpa_vrf_push_wire_values(of_oxm_ofdpa_vrf_t *obj)
{

    of_oxm_ofdpa_vrf_push_wire_types(obj);

    return OF_ERROR_NONE;
}

/**
 * Create a new of_oxm_ofdpa_vrf object
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
 * \ingroup of_oxm_ofdpa_vrf
 */

of_oxm_ofdpa_vrf_t *
of_oxm_ofdpa_vrf_new(of_version_t version)
{
    of_oxm_ofdpa_vrf_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_OXM_OFDPA_VRF] + of_object_extra_len[version][OF_OXM_OFDPA_VRF];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_oxm_ofdpa_vrf_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_oxm_ofdpa_vrf_init(obj, version, bytes, 0);

    if (of_oxm_ofdpa_vrf_push_wire_values(obj) < 0) {
        FREE(obj);
        return NULL;
    }

    return obj;
}

/**
 * Initialize an object of type of_oxm_ofdpa_vrf.
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
of_oxm_ofdpa_vrf_init(of_oxm_ofdpa_vrf_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_OXM_OFDPA_VRF] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_OXM_OFDPA_VRF] + of_object_extra_len[version][OF_OXM_OFDPA_VRF];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_OXM_OFDPA_VRF;

    /* Set up the object's function pointers */

    obj->wire_type_set = of_oxm_ofdpa_vrf_push_wire_types;

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
 * Get experimenter from an object of type of_oxm_ofdpa_vrf.
 * @param obj Pointer to an object of type of_oxm_ofdpa_vrf.
 * @param experimenter Pointer to the child object of type
 * uint32_t to be filled out.
 *
 */
void
of_oxm_ofdpa_vrf_experimenter_get(
    of_oxm_ofdpa_vrf_t *obj,
    uint32_t *experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_VRF);
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
 * Set experimenter in an object of type of_oxm_ofdpa_vrf.
 * @param obj Pointer to an object of type of_oxm_ofdpa_vrf.
 * @param experimenter The value to write into the object
 */
void
of_oxm_ofdpa_vrf_experimenter_set(
    of_oxm_ofdpa_vrf_t *obj,
    uint32_t experimenter)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_VRF);
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
 * Get value from an object of type of_oxm_ofdpa_vrf.
 * @param obj Pointer to an object of type of_oxm_ofdpa_vrf.
 * @param value Pointer to the child object of type
 * uint16_t to be filled out.
 *
 */
void
of_oxm_ofdpa_vrf_value_get(
    of_oxm_ofdpa_vrf_t *obj,
    uint16_t *value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_VRF);
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
    of_wire_buffer_u16_get(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}

/**
 * Set value in an object of type of_oxm_ofdpa_vrf.
 * @param obj Pointer to an object of type of_oxm_ofdpa_vrf.
 * @param value The value to write into the object
 */
void
of_oxm_ofdpa_vrf_value_set(
    of_oxm_ofdpa_vrf_t *obj,
    uint16_t value)
{
    of_wire_buffer_t *wbuf;
    int offset = 0; /* Offset of value relative to the start obj */
    int abs_offset; /* Offset of value relative to start of wbuf */
    of_version_t ver;

    LOCI_ASSERT(obj->object_id == OF_OXM_OFDPA_VRF);
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
    of_wire_buffer_u16_set(wbuf, abs_offset, value);

    OF_LENGTH_CHECK_ASSERT(obj);

    return ;
}
