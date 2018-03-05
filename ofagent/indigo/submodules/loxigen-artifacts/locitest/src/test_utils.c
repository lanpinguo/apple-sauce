/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford Junior University */
/* Copyright (c) 2011, 2012 Open Networking Foundation */
/* Copyright (c) 2012, 2013 Big Switch Networks, Inc. */
/* See the file LICENSE.loci which should have been included in the source distribution */

/**
 *
 * Test utility functions
 *
 */

#include <locitest/test_common.h>
#include <loci/of_utils.h>

/**
 * Test has output port utility function
 */
int
test_has_outport(void)
{
    of_list_action_t *list;
    of_action_t elt;
    of_action_set_dl_src_t *set_dl_src;
    of_action_output_t *output;

    set_dl_src = &elt.set_dl_src;
    output = &elt.output;

    list = of_list_action_new(OF_VERSION_1_0);
    TEST_ASSERT(list != NULL);

    TEST_ASSERT(of_action_list_has_out_port(list, OF_PORT_DEST_WILDCARD));
    TEST_ASSERT(!of_action_list_has_out_port(list, 1));

    /* Add some other action */
    of_action_set_dl_src_init(set_dl_src, OF_VERSION_1_0, -1, 1);
    TEST_OK(of_list_action_append_bind(list, (of_action_t *)set_dl_src));

    TEST_ASSERT(of_action_list_has_out_port(list, OF_PORT_DEST_WILDCARD));
    TEST_ASSERT(!of_action_list_has_out_port(list, 1));

    /* Add port 2 */
    of_action_output_init(output, OF_VERSION_1_0, -1, 1);
    TEST_OK(of_list_action_append_bind(list, (of_action_t *)output));
    of_action_output_port_set(output, 2);

    TEST_ASSERT(of_action_list_has_out_port(list, OF_PORT_DEST_WILDCARD));
    TEST_ASSERT(!of_action_list_has_out_port(list, 1));
    TEST_ASSERT(of_action_list_has_out_port(list, 2));

    /* Add port 1 */
    of_action_output_init(output, OF_VERSION_1_0, -1, 1);
    TEST_OK(of_list_action_append_bind(list, (of_action_t *)output));
    of_action_output_port_set(output, 1);

    TEST_ASSERT(of_action_list_has_out_port(list, OF_PORT_DEST_WILDCARD));
    TEST_ASSERT(of_action_list_has_out_port(list, 1));
    TEST_ASSERT(of_action_list_has_out_port(list, 2));

    /* Start over and put action at front of list */
    of_list_action_delete(list);

    list = of_list_action_new(OF_VERSION_1_0);
    TEST_ASSERT(list != NULL);

    /* Add port 2 */
    of_action_output_init(output, OF_VERSION_1_0, -1, 1);
    TEST_OK(of_list_action_append_bind(list, (of_action_t *)output));
    of_action_output_port_set(output, 2);

    TEST_ASSERT(of_action_list_has_out_port(list, OF_PORT_DEST_WILDCARD));
    TEST_ASSERT(!of_action_list_has_out_port(list, 1));
    TEST_ASSERT(of_action_list_has_out_port(list, 2));

    /* Add some other action */
    of_action_set_dl_src_init(set_dl_src, OF_VERSION_1_0, -1, 1);
    TEST_OK(of_list_action_append_bind(list, (of_action_t *)set_dl_src));

    TEST_ASSERT(of_action_list_has_out_port(list, OF_PORT_DEST_WILDCARD));
    TEST_ASSERT(!of_action_list_has_out_port(list, 1));
    TEST_ASSERT(of_action_list_has_out_port(list, 2));

    /* Add port 1 */
    of_action_output_init(output, OF_VERSION_1_0, -1, 1);
    TEST_OK(of_list_action_append_bind(list, (of_action_t *)output));
    of_action_output_port_set(output, 1);

    TEST_ASSERT(of_action_list_has_out_port(list, OF_PORT_DEST_WILDCARD));
    TEST_ASSERT(of_action_list_has_out_port(list, 1));
    TEST_ASSERT(of_action_list_has_out_port(list, 2));

    of_list_action_delete(list);

    return TEST_PASS;
}

static int
test_of_object_new_from_message(void)
{
    /* v1 OFPT_HELLO, xid=0x12345678 */
    uint8_t data[] = { 0x01, 0x00, 0x00, 0x08, 0x12, 0x34, 0x56, 0x78 };
    uint8_t *buf = malloc(sizeof(data));
    memcpy(buf, data, sizeof(data));

    of_object_t *obj = of_object_new_from_message(buf, sizeof(data));

    TEST_ASSERT(obj != NULL);
    TEST_ASSERT(obj->version == OF_VERSION_1_0);
    TEST_ASSERT(obj->object_id == OF_HELLO);

    uint32_t xid;
    of_hello_xid_get(obj, &xid);
    TEST_ASSERT(xid == 0x12345678);

    of_object_delete(obj);

    return TEST_PASS;
}

static int
test_of_object_new_from_message_preallocated(void)
{
    /* v1 OFPT_HELLO, xid=0x12345678 */
    uint8_t buf[] = { 0x01, 0x00, 0x00, 0x08, 0x12, 0x34, 0x56, 0x78 };

    of_object_storage_t storage;
    of_object_t *obj = of_object_new_from_message_preallocated(
        &storage, buf, sizeof(buf));

    TEST_ASSERT(obj != NULL);
    TEST_ASSERT(obj->version == OF_VERSION_1_0);
    TEST_ASSERT(obj->object_id == OF_HELLO);

    uint32_t xid;
    of_hello_xid_get(obj, &xid);
    TEST_ASSERT(xid == 0x12345678);

    return TEST_PASS;
}

int
run_utility_tests(void)
{
    RUN_TEST(has_outport);
    RUN_TEST(of_object_new_from_message);
    RUN_TEST(of_object_new_from_message_preallocated);
    RUN_TEST(dump_objs);

    return TEST_PASS;
}
