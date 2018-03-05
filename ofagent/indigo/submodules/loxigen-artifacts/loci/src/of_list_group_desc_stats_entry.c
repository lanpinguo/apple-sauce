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
 * \defgroup of_list_group_desc_stats_entry of_list_group_desc_stats_entry
 */

/**
 * Create a new of_list_group_desc_stats_entry object
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
 * \ingroup of_list_group_desc_stats_entry
 */

of_list_group_desc_stats_entry_t *
of_list_group_desc_stats_entry_new(of_version_t version)
{
    of_list_group_desc_stats_entry_t *obj;
    int bytes;

    bytes = of_object_fixed_len[version][OF_LIST_GROUP_DESC_STATS_ENTRY] + of_object_extra_len[version][OF_LIST_GROUP_DESC_STATS_ENTRY];

    /* Allocate a maximum-length wire buffer assuming we'll be appending to it. */
    if ((obj = (of_list_group_desc_stats_entry_t *)of_object_new(OF_WIRE_BUFFER_MAX_LENGTH)) == NULL) {
        return NULL;
    }

    of_list_group_desc_stats_entry_init(obj, version, bytes, 0);

    return obj;
}

/**
 * Initialize an object of type of_list_group_desc_stats_entry.
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
of_list_group_desc_stats_entry_init(of_list_group_desc_stats_entry_t *obj,
    of_version_t version, int bytes, int clean_wire)
{

    LOCI_ASSERT(of_object_fixed_len[version][OF_LIST_GROUP_DESC_STATS_ENTRY] >= 0);
    if (clean_wire) {
        MEMSET(obj, 0, sizeof(*obj));
    }
    if (bytes < 0) {
        bytes = of_object_fixed_len[version][OF_LIST_GROUP_DESC_STATS_ENTRY] + of_object_extra_len[version][OF_LIST_GROUP_DESC_STATS_ENTRY];
    }
    obj->version = version;
    obj->length = bytes;
    obj->object_id = OF_LIST_GROUP_DESC_STATS_ENTRY;

    /* Set up the object's function pointers */

    /* Grow the wire buffer */
    if (obj->wire_object.wbuf != NULL) {
        int tot_bytes;

        tot_bytes = bytes + obj->wire_object.obj_offset;
        of_wire_buffer_grow(obj->wire_object.wbuf, tot_bytes);
    }
}


/**
 * Associate an iterator with a list
 * @param list The list to iterate over
 * @param obj The list entry iteration pointer
 * @return OF_ERROR_RANGE if the list is empty (end of list)
 *
 * The obj instance is completely initialized.  The caller is responsible
 * for cleaning up any wire buffers associated with obj before this call
 */

int
of_list_group_desc_stats_entry_first(of_list_group_desc_stats_entry_t *list,
    of_group_desc_stats_entry_t *obj)
{
    int rv;

    of_group_desc_stats_entry_init(obj,
            list->version, 0, 1);
    if ((rv = of_list_first((of_object_t *)list, (of_object_t *)obj)) < 0) {
        return rv;
    }

    of_object_wire_init((of_object_t *) obj, OF_GROUP_DESC_STATS_ENTRY,
                        list->length);
    if (obj->length == 0) {
        return OF_ERROR_PARSE;
    }

    return rv;
}

/**
 * Advance an iterator to the next element in a list
 * @param list The list being iterated
 * @param obj The list entry iteration pointer
 * @return OF_ERROR_RANGE if already at the last entry on the list
 *
 */

int
of_list_group_desc_stats_entry_next(of_list_group_desc_stats_entry_t *list,
    of_group_desc_stats_entry_t *obj)
{
    int rv;

    if ((rv = of_list_next((of_object_t *)list, (of_object_t *)obj)) < 0) {
        return rv;
    }

    rv = of_object_wire_init((of_object_t *) obj, OF_GROUP_DESC_STATS_ENTRY,
        list->length);

    if ((rv == OF_ERROR_NONE) && (obj->length == 0)) {
        return OF_ERROR_PARSE;
    }

    return rv;
}

/**
 * Set up to append an object of type of_group_desc_stats_entry to an of_list_group_desc_stats_entry.
 * @param list The list that is prepared for append
 * @param obj Pointer to object to hold data to append
 *
 * The obj instance is completely initialized.  The caller is responsible
 * for cleaning up any wire buffers associated with obj before this call.
 *
 * See the generic documentation for of_list_append_bind.
 */

int
of_list_group_desc_stats_entry_append_bind(of_list_group_desc_stats_entry_t *list,
    of_group_desc_stats_entry_t *obj)
{
    return of_list_append_bind((of_object_t *)list, (of_object_t *)obj);
}

/**
 * Append an item to a of_list_group_desc_stats_entry list.
 *
 * This copies data from item and leaves item untouched.
 *
 * See the generic documentation for of_list_append.
 */

int
of_list_group_desc_stats_entry_append(of_list_group_desc_stats_entry_t *list,
    of_group_desc_stats_entry_t *item)
{
    return of_list_append((of_object_t *)list, (of_object_t *)item);
}

