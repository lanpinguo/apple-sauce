/*********************************************************************
*
* (C) Copyright Broadcom Corporation 2000-2014
*
**********************************************************************
*
* @filename   avl.c
*
* @purpose    File contains all the api functions for a generic avl tree
*
* @component  avl
*
* @comments
*
* @create     8/31/2000
*
* @end
*
**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "avl_api.h"
#include "avl.h"
#include "ofdpa_util.h"

/*********************************************************************
* @purpose  Creates the generic avl tree structure and initialize
*
* @param    tree              pointer to the tree structure
* @param    treeHeap          pointer to the tree heap
* @param    dataHeap          pointer to the data heap
* @param    max_entries       maximum number of entries in the tree
* @param    dataLength        length of a data entry
* @param    type              type of data
* @param    lengthSearchKey   length of the search key
*
* @returns  AvlTree_t   pointer to the generic avl tree structure if successful
* @returns  0     if not successful
*
* @notes    RESTRICTIONS:1. First field in the data structure
*                           (of data heap) must be the key
*                        2. Last field in the data structure
*                           (of data heap) must be a void pointer type
*
* @end
*********************************************************************/
void avlCreateAvlTree(avlTree_t *tree, avlTreeTables_t *treeHeap, void *dataHeap,
                      unsigned int max_entries, unsigned int dataLength, unsigned int type,
                      unsigned int lengthSearchKey)
{
  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_TOO_VERBOSE,
                     "AVL heap: %u bytes.\r\n",
                     sizeof(avlTreeTables_t) * max_entries);
  OFDPA_DEBUG_PRINTF(OFDPA_COMPONENT_MAPPING, OFDPA_DEBUG_TOO_VERBOSE,
                     "AVL data: %u bytes.\r\n",
                     dataLength * max_entries);

  bzero((char *)&tree->root, sizeof(avlTreeTables_t));
  bzero((char *)treeHeap, max_entries*sizeof(avlTreeTables_t));
  bzero((char *)dataHeap, max_entries*dataLength);

  tree->type = type;
  tree->semId = dpaSemCreate ("AVL Semaphore", DPA_SEM_BINARY, 1);
  tree->lengthData = dataLength;
  tree->offset_next = dataLength - sizeof(void *);
  tree->lengthSearchKey = lengthSearchKey;

  tree->initialTableHeap = treeHeap;
  tree->initialDataHeap = dataHeap;
  tree->compare = memcmp;
  tree->tableHeap = 0;
  tree->dataHeap = 0;

  avlPurgeAvlTree(tree, max_entries);
}

/*********************************************************************
* @purpose  Set the comparator function for an AVL tree
*
* @param    tree              pointer to the tree structure
* @param    compare           pointer to the new comparator function
*                             if NULL is used, the default comparator
*                             (memcmp()) will be used.
*
* @returns  pointer to the previous comparator (this function does not fail)
*
* @notes    The default comparator in a generic avl tree is memcmp()
*           There are also some canned comparators supplied (these are
*           declared below).  If the user wants to provide a new type
*           specific comparator it should have the same signature as
*           memcmp():
*
*               int bar(void *a, void *b, size_t key_len);
*
*           and it should have the following integer return values:
*
*               >0 (I generally use 1) if a > b
*               0  if a == b
*               <0 if a < b
*
*           The algorithm for the comparison and the definitions
*           of <, >, and == belong entirely to the comparator. The
*           only requirements are that < and > are consistent, and
*           == ensures uniqueness within the tree.
* @end
*********************************************************************/
avlComparator_t avlSetAvlTreeComparator(avlTree_t *t, avlComparator_t c)
{
  avlComparator_t ret = t->compare;
  if (c != 0)
  {
    /*
     * The caller supplied a non-NULL pointer, so use the supplied
     * comparator function.
     */
    t->compare = c;
  }
  else
  {
    /*
     * If the caller supplied a NULL pointer, then restore the default.
     */
    t->compare = memcmp;
  }
  return ret;
}

/*********************************************************************
* @purpose  Deletes an avl tree structure
*
* @param    tree              pointer to an avl tree structure
*
* @returns  0
* @returns  -1
* @returns  -2
*
* @notes
*
* @end
*********************************************************************/
int avlDeleteAvlTree(avlTree_t *tree)
{
  if (tree == 0)
    return -2;

  dpaSemDestroy(tree->semId);

  if (tree->tableHeap != 0)
    free(tree->tableHeap);

  if (tree->dataHeap != 0)
    free(tree->dataHeap);

  return 0;
}

/*********************************************************************
* @purpose  Resets all entries but the root node
*
* @param    avl_tree    pointer to the avl tree structure
* @param    max_entries max number of entries in the structure
*
* @returns  void
*
* @notes
*
* @end
*********************************************************************/
void avlPurgeAvlTree(avlTree_t *avl_tree, unsigned int max_entries)
{
  unsigned int       i;
  unsigned int       offset_next, lengthData;
  avlTreeTables_t *treeHeap;
  void            *dataHeap;

  offset_next = avl_tree->offset_next;
  lengthData = avl_tree->lengthData;

  avl_tree->root.link[LEFT] = 0;
  avl_tree->root.link[RIGHT] = 0;
  avl_tree->count = 0;
  avl_tree->value = 0;

  treeHeap = avl_tree->initialTableHeap;
  dataHeap = avl_tree->initialDataHeap;

  avl_tree->currentTableHeap = treeHeap;
  avl_tree->currentDataHeap = dataHeap;

  for (i = 0; i < max_entries; i++)
  {
    treeHeap[i].link[LEFT] = 0;
    treeHeap[i].link[RIGHT] = &treeHeap[i+1];

    *((unsigned long*)((char*)dataHeap + offset_next)) = (unsigned long)((char*)dataHeap + lengthData);
    dataHeap = (char*)dataHeap + lengthData;
  }

  treeHeap = avl_tree->initialTableHeap;
  dataHeap = avl_tree->initialDataHeap;
  if (i > 0)  /* Added this check to make sure there is no array bound violation */
  {
    treeHeap[i-1].link[RIGHT] = 0;
    *(unsigned int *)((char*)dataHeap + ((i-1)*lengthData) + offset_next) = 0;
  }
}

/*********************************************************************
* @purpose  Inserts ITEM into TREE.  Returns NULL if the item was inserted,
*           otherwise a pointer to the duplicate item
*
* @param    avl_tree    pointer to the avl tree structure
* @param    item        pointer to item to be inserted
*
* @returns  NULL        if item was inserted
* @returns  void  pointer to a duplicate item, if duplicate exists
* @returns  void  pointer to item, if error
*
* @notes
*
* @end
*********************************************************************/
void *avlInsertEntry(avlTree_t *avl_tree, void *item)
{
  void * rc;

  rc = avlAddEntry(avl_tree, item);

  return(rc);
}

/*********************************************************************
* @purpose  Searches AVL tree for an item matching ITEM.  If found, the
*           item is removed from the tree and the actual item found is returned
*           to the caller.  If no item matching ITEM exists in the tree,
*           returns NULL.
*
* @param    avl_tree pointer to the avl tree structure
* @param    item     pointer to item to be deleted
*
* @returns  void *   pointer to the item if deleted
* @returns  NULL     if item does not exist in the tree
*
* @notes
*
* @end
*********************************************************************/
void *avlDeleteEntry(avlTree_t *avl_tree, void *item)
{
  void * rc;

  rc = avlRemoveEntry(avl_tree, item);

  return(rc);
}

/*****************************************************************
* @purpose  Searches AVL tree for exact or get next match
*
* @param    avl_tree    pointer to the avl tree structure
* @param    item        pointer to item to be found
* @param    matchType   AVL_EXACT or AVL_NEXT
*
* @returns  void *      pointer to the item if matched
* @returns  0     if item does not exist in the tree
*
* @notes
*
* @end
*****************************************************************/
void *avlSearch(avlTree_t *avl_tree, void *key, unsigned int flags)
{
  int        diff;
  unsigned int       found_equal;
  avlTreeTables_t *ptr, *saved_node;

  ptr = avl_tree->root.link[LEFT];
  saved_node = 0;
  found_equal = 0;

  while (ptr != NULL)
  {
    diff = avlCompareKey(key, ptr->data,
                         avl_tree->lengthSearchKey,
                         avl_tree->compare);

    if (diff == AVL_EQUAL)
    {
      if (flags & AVL_EXACT)
      {
        saved_node  = ptr;
        found_equal = 1;
        break;
      }
      else
        ptr=ptr->link[RIGHT];
    }
    else if (diff == AVL_LESS_THAN)
    { /*goto left subtree*/
      saved_node=ptr;
      ptr=ptr->link[LEFT];
    }
    else
    {                              /*goto right subtree*/
      ptr=ptr->link[RIGHT];
    }
  }   /* ptr */

  if ((found_equal == 1) ||
      ((flags & AVL_NEXT) && saved_node != 0)) /* if found or doing a get next */
    return saved_node->data;
  else
    return 0;
}

/*****************************************************************
* @purpose  Obtains count of nodes in the tree
*
* @param    avl_tree    pointer to the avl tree structure
*
* @returns  count       count of items in the tree
*
* @notes
*
* @end
*****************************************************************/
unsigned int avlTreeCount(avlTree_t *avl_tree)
{
  return(avl_tree->count);
}

/*****************************************************************
* @purpose  Compare short (16 bit signed keys) and indicate relationship
*
* @param    a           Pointer to the first key
* @param    b           Pointer to the second key
* @param    len         Length of the key (unused)
*
* @returns   0 if the keys are equal
* @returns   1 if key 'a' is greater than key 'b'
* @returns  -1 if key 'a' is less than key 'b'
*
* @notes
*
* @end
*****************************************************************/
int avlCompareShort16(const void *a, const void *b, size_t len)
{
  int16_t va = *(int16_t *)a;
  int16_t vb = *(int16_t *)b;

  if (va == vb)
  {
    return 0;
  }
  if (va > vb)
  {
    return 1;
  }
  return -1;
}

/*****************************************************************
* @purpose  Compare long (32 bit signed keys) and indicate relationship
*
* @param    a           Pointer to the first key
* @param    b           Pointer to the second key
* @param    len         Length of the key (unused)
*
* @returns   0 if the keys are equal
* @returns   1 if key 'a' is greater than key 'b'
* @returns  -1 if key 'a' is less than key 'b'
*
* @notes
*
* @end
*****************************************************************/
int avlCompareLong32(const void *a, const void *b, size_t len)
{
  int32_t va = *(int32_t *)a;
  int32_t vb = *(int32_t *)b;

  if (va == vb)
  {
    return 0;
  }
  if (va > vb)
  {
    return 1;
  }
  return -1;
}

/*****************************************************************
* @purpose  Compare ushort (16 bit unsigned keys) and indicate relationship
*
* @param    a           Pointer to the first key
* @param    b           Pointer to the second key
* @param    len         Length of the key (unused)
*
* @returns   0 if the keys are equal
* @returns   1 if key 'a' is greater than key 'b'
* @returns  -1 if key 'a' is less than key 'b'
*
* @notes
*
* @end
*****************************************************************/
int avlCompareUShort16(const void *a, const void *b, size_t len)
{
  int16_t va = *(int16_t *)a;
  int16_t vb = *(int16_t *)b;

  if (va == vb)
  {
    return 0;
  }
  if (va > vb)
  {
    return 1;
  }
  return -1;
}

/*****************************************************************
* @purpose  Compare ulong (32 bit unsigned keys) and indicate relationship
*
* @param    a           Pointer to the first key
* @param    b           Pointer to the second key
* @param    len         Length of the key (unused)
*
* @returns   0 if the keys are equal
* @returns   1 if key 'a' is greater than key 'b'
* @returns  -1 if key 'a' is less than key 'b'
*
* @notes
*
* @end
*****************************************************************/
int avlCompareULong32(const void *a, const void *b, size_t len)
{
  uint32_t va = *(uint32_t *)a;
  uint32_t vb = *(uint32_t *)b;

  if (va == vb)
  {
    return 0;
  }
  if (va > vb)
  {
    return 1;
  }
  return -1;
}

/*****************************************************************
* @purpose  Compare ulonglong (64 bit unsigned keys) and indicate relationship
*
* @param    a           Pointer to the first key
* @param    b           Pointer to the second key
* @param    len         Length of the key (unused)
*
* @returns   0 if the keys are equal
* @returns   1 if key 'a' is greater than key 'b'
* @returns  -1 if key 'a' is less than key 'b'
*
* @notes
*
* @end
*****************************************************************/
int avlCompareULongLong64(const void *a, const void *b, size_t len)
{
  uint64_t va = *(uint64_t *)a;
  uint64_t vb = *(uint64_t *)b;

  if (va == vb)
  {
    return 0;
  }
  if (va > vb)
  {
    return 1;
  }
  return -1;
}

/*****************************************************************
* @purpose  Compare IP Address keys and indicate relationship
*
* @param    a           Pointer to the first key
* @param    b           Pointer to the second key
* @param    len         Length of the key (unused)
*
* @returns   0 if the keys are equal
* @returns   1 if key 'a' is greater than key 'b'
* @returns  -1 if key 'a' is less than key 'b'
*
* @notes
*
* @end
*****************************************************************/
int avlCompareIPAddr(const void *a, const void *b, size_t len)
{
  unsigned int va = *(unsigned int *)a;
  unsigned int vb = *(unsigned int *)b;

  if (va == vb)
  {
    return 0;
  }
  if (va > vb)
  {
    return 1;
  }
  return -1;
}

/*****************************************************************
* @purpose  Compare IP Network Address (Address and Mask) keys and
*           indicate relationship
*
* @param    a           Pointer to the first key
* @param    b           Pointer to the second key
* @param    len         Length of the key (unused)
*
* @returns   0 if the keys are equal
* @returns   1 if key 'a' is greater than key 'b'
* @returns  -1 if key 'a' is less than key 'b'
*
* @notes   At this time, these are compared the same way they would
*          be if they were compared with memcmp() on a big-endian platform.
*          It might be more useful to compare them by some other criterion
*          (shorter masks first, for example).
*
* @end
*****************************************************************/
int avlCompareIPNetAddr(const void *a, const void *b, size_t len)
{
  unsigned int addr_a = *(unsigned int *)a;
  unsigned int mask_a = *(((unsigned int *)a) + 1);
  unsigned int addr_b = *(unsigned int *)b;
  unsigned int mask_b = *(((unsigned int *)b) + 1);

  if (addr_a < addr_b)
  {
    return -1;
  }
  if (addr_a > addr_b)
  {
    return 1;
  }
  if (mask_a < mask_b)
  {
    return -1;
  }
  if (mask_a > mask_b)
  {
    return 1;
  }
  return 0;
}

/*********************************************************************
* @purpose  Creates the generic avl tree structure and initialize
*
* @param    avlTree           pointer to the tree structure
* @param    max_entries       maximum number of entries in the tree
* @param    dataLength        length of a data entry
* @param    type              type of data
* @param    compareFcn        pointer to the comparator function
*                             if NULL is used, the default comparator
*                             (memcmp()) will be used.
* @param    lengthSearchKey   length of the search key
*
* @returns  0  if avl tree initialization was successful
* @returns  -1  if not successful
*
* @notes    RESTRICTIONS:1. First field in the data structure
*                           (of data heap) must be the key
*                        2. Last field in the data structure
*                           (of data heap) must be a void pointer type
*
* @end
*********************************************************************/
int avlAllocAndCreateAvlTree(avlTree_t *avlTree,
                             unsigned int max_entries,
                             unsigned int dataLength,
                             unsigned int type,
                             avlComparator_t compareFcn,
                             unsigned int lengthSearchKey)
{
  avlTreeTables_t *avlTreeTable;
  void            *avlTreeData;

  /* Allocate the AVL Tree Tables */
  avlTreeTable = malloc(sizeof(avlTreeTables_t) * max_entries);
  if (avlTreeTable == 0)
  {
    return -1;
  }

  /* Allocate the AVL data heap */
  avlTreeData = malloc(dataLength * max_entries);
  if (avlTreeData == 0)
  {
    free(avlTreeTable);
    return -1;
  }

  avlCreateAvlTree(avlTree, avlTreeTable, avlTreeData,
                   max_entries, dataLength, type, lengthSearchKey);

  (void)avlSetAvlTreeComparator(avlTree, compareFcn);
  avlTree->tableHeap = avlTreeTable;
  avlTree->dataHeap = avlTreeData;

  return 0;
}
