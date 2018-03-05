/****************************Copyright (c)*********************************************                                     
*File Name	: ofdpa_list.h
*Author			: lanpinguo 	
*Creat Date	: 2015Äê3ÔÂ9ÈÕ
*Description: port list_head from kernel
**---------- Version------------------------------------------------------------------
*Vertion		: V1.0
*Note				: None
*
*-------------------------------------------------------------------------------------
**************************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __OFDPA_LIST_H__
#define __OFDPA_LIST_H__

#ifndef NULL
#define NULL            0
#endif


struct ofdpa_list_head {
	struct ofdpa_list_head *next, *prev;
};


#define ofdpa_offsetof(TYPE, MEMBER) ((unsigned int)&((TYPE *)0)->MEMBER)

#define ofdpa_container_of(ptr, type, member) \
        (type *)( (char *)ptr - ofdpa_offsetof(type,member) )
        
#define ofdpa_list_entry(ptr, type, member) ofdpa_container_of(ptr, type, member)


#define ofdpa_list_for_each_entry(pos, head, type, member)				\
	for (pos = ofdpa_list_entry((head)->next, type, member);	\
	     &pos->member != (head); 	\
	     pos = ofdpa_list_entry(pos->member.next, type, member))


#define OFDPA_LIST_HEAD_INIT(name) { &(name), &(name) }

#define OFDPA_LIST_HEAD(name) \
	struct ofdpa_list_head name = OFDPA_LIST_HEAD_INIT(name)

static __inline void OFDPA_INIT_LIST_HEAD(struct ofdpa_list_head *list)
{
	list->next = list;
	list->prev = list;
}

static __inline void __ofdpa_list_add(struct ofdpa_list_head *new_node,
			      struct ofdpa_list_head *prev,
			      struct ofdpa_list_head *next)
{
	next->prev = new_node;
	new_node->next = next;
	new_node->prev = prev;
	prev->next = new_node;
}

/**
 * ofdpa_list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static __inline void ofdpa_list_add(struct ofdpa_list_head *new, struct ofdpa_list_head *head)
{
	__ofdpa_list_add(new, head, head->next);
}

/**
 * ofdpa_list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static __inline void ofdpa_list_add_tail(struct ofdpa_list_head *new, struct ofdpa_list_head *head)
{
	__ofdpa_list_add(new, head->prev, head);
}


/**
 * ofdpa_list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define ofdpa_list_entry(ptr, type, member) \
	ofdpa_container_of(ptr, type, member)

/**
 * ofdpa_list_first_entry - get the first element from a list
 * @ptr:	the list head to take the element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define ofdpa_list_first_entry(ptr, type, member) \
	ofdpa_list_entry((ptr)->next, type, member)

#define ofdpa_list_last_entry(ptr, type, member) \
	ofdpa_list_entry((ptr)->prev, type, member)


/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static __inline void __ofdpa_list_del(struct ofdpa_list_head * prev, struct ofdpa_list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

/**
 * ofdpa_list_del - deletes entry from list.
 * @entry: the element to delete from the list.
 * Note: ofdpa_list_empty() on entry does not return true after this, the entry is
 * in an undefined state.
 */
static __inline void __ofdpa_list_del_entry(struct ofdpa_list_head *entry)
{
	__ofdpa_list_del(entry->prev, entry->next);
}

static __inline void ofdpa_list_del(struct ofdpa_list_head *entry)
{
	__ofdpa_list_del(entry->prev, entry->next);
	entry->next = NULL;
	entry->prev = NULL;
}

/**
 * ofdpa_list_replace - replace old entry by new one
 * @old : the element to be replaced
 * @new : the new element to insert
 *
 * If @old was empty, it will be overwritten.
 */
static __inline void ofdpa_list_replace(struct ofdpa_list_head *old,
				struct ofdpa_list_head *new)
{
	new->next = old->next;
	new->next->prev = new;
	new->prev = old->prev;
	new->prev->next = new;
}

static __inline void ofdpa_list_replace_init(struct ofdpa_list_head *old,
					struct ofdpa_list_head *new)
{
	ofdpa_list_replace(old, new);
	OFDPA_INIT_LIST_HEAD(old);
}

/**
 * ofdpa_list_del_init - deletes entry from list and reinitialize it.
 * @entry: the element to delete from the list.
 */
static __inline void ofdpa_list_del_init(struct ofdpa_list_head *entry)
{
	__ofdpa_list_del_entry(entry);
	OFDPA_INIT_LIST_HEAD(entry);
}

/**
 * ofdpa_list_move - delete from one list and add as another's head
 * @list: the entry to move
 * @head: the head that will precede our entry
 */
static __inline void ofdpa_list_move(struct ofdpa_list_head *list, struct ofdpa_list_head *head)
{
	__ofdpa_list_del_entry(list);
	ofdpa_list_add(list, head);
}

/**
 * ofdpa_list_move_tail - delete from one list and add as another's tail
 * @list: the entry to move
 * @head: the head that will follow our entry
 */
static __inline void ofdpa_list_move_tail(struct ofdpa_list_head *list,
				  struct ofdpa_list_head *head)
{
	__ofdpa_list_del_entry(list);
	ofdpa_list_add_tail(list, head);
}

/**
 * ofdpa_list_is_last - tests whether @list is the last entry in list @head
 * @list: the entry to test
 * @head: the head of the list
 */
static __inline int ofdpa_list_is_last(const struct ofdpa_list_head *list,
				const struct ofdpa_list_head *head)
{
	return list->next == head;
}

/**
 * ofdpa_list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static __inline int ofdpa_list_empty(const struct ofdpa_list_head *head)
{
	return head->next == head;
}

/**
 * ofdpa_list_empty_careful - tests whether a list is empty and not being modified
 * @head: the list to test
 *
 * Description:
 * tests whether a list is empty _and_ checks that no other CPU might be
 * in the process of modifying either member (next or prev)
 *
 * NOTE: using ofdpa_list_empty_careful() without synchronization
 * can only be safe if the only activity that can happen
 * to the list entry is ofdpa_list_del_init(). Eg. it cannot be used
 * if another CPU could re-ofdpa_list_add() it.
 */
static __inline int ofdpa_list_empty_careful(const struct ofdpa_list_head *head)
{
	struct ofdpa_list_head *next = head->next;
	return (next == head) && (next == head->prev);
}
/**
 * ofdpa_list_for_each	-	iterate over a list
 * @pos:	the &struct ofdpa_list_head to use as a loop cursor.
 * @head:	the head for your list.
 */
#define ofdpa_list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)



#endif /* __OFDPA_LIST_H__ */

