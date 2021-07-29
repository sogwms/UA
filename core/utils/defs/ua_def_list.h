/*
 * Copyright (c) 2006-2018, UA-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-03-16     Bernard      the first version
 * 2006-09-07     Bernard      move the kservice APIs to uathread.h
 * 2007-06-27     Bernard      fix the ua_list_remove bug
 * 2012-03-22     Bernard      rename kservice.h to uaservice.h
 * 2017-11-15     JasonJia     Modify ua_slist_foreach to ua_slist_for_each_entry.
 *                             Make code cleanup.
 * 
 * 2021-07-29     letvano      adapt to ua
 */

#ifndef __UA_DEF_LIST_H__
#define __UA_DEF_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "ua_def_ref_compiler.h"

/**
 * Double List structure
 */
struct ua_list_node
{
    struct ua_list_node *next;                          /**< point to next node. */
    struct ua_list_node *prev;                          /**< point to prev node. */
};
typedef struct ua_list_node ua_list_t;                  /**< Type for lists. */

/**
 * Single List structure
 */
struct ua_slist_node
{
    struct ua_slist_node *next;                         /**< point to next node. */
};
typedef struct ua_slist_node ua_slist_t;                /**< Type for single list. */

/**
 * @addtogroup KernelService
 */

/**@{*/

/**
 * ____container_of - return the member address of ptr, if the type of ptr is the
 * struct type.
 */
#define ____container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))


/**
 * @brief initialize a list object
 */
#define UA_LIST_OBJECT_INIT(object) { &(object), &(object) }

/**
 * @brief initialize a list
 *
 * @param l list to be initialized
 */
UA_INLINE void ua_list_init(ua_list_t *l)
{
    l->next = l->prev = l;
}

/**
 * @brief insert a node after a list
 *
 * @param l list to insert it
 * @param n new node to be inserted
 */
UA_INLINE void ua_list_insert_after(ua_list_t *l, ua_list_t *n)
{
    l->next->prev = n;
    n->next = l->next;

    l->next = n;
    n->prev = l;
}

/**
 * @brief insert a node before a list
 *
 * @param n new node to be inserted
 * @param l list to insert it
 */
UA_INLINE void ua_list_insert_before(ua_list_t *l, ua_list_t *n)
{
    l->prev->next = n;
    n->prev = l->prev;

    l->prev = n;
    n->next = l;
}

/**
 * @brief remove node from list.
 * @param n the node to remove from the list.
 */
UA_INLINE void ua_list_remove(ua_list_t *n)
{
    n->next->prev = n->prev;
    n->prev->next = n->next;

    n->next = n->prev = n;
}

/**
 * @brief tests whether a list is empty
 * @param l the list to test.
 */
UA_INLINE int ua_list_isempty(const ua_list_t *l)
{
    return l->next == l;
}

/**
 * @brief get the list length
 * @param l the list to get.
 */
UA_INLINE unsigned int ua_list_len(const ua_list_t *l)
{
    unsigned int len = 0;
    const ua_list_t *p = l;
    while (p->next != l)
    {
        p = p->next;
        len ++;
    }

    return len;
}

/**
 * @brief get the struct for this entry
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of list in structure
 */
#define ua_list_entry(node, type, member) \
    ____container_of(node, type, member)

/**
 * ua_list_for_each - iterate over a list
 * @pos:	the ua_list_t * to use as a loop cursor.
 * @head:	the head for your list.
 */
#define ua_list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/**
 * ua_list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:	the ua_list_t * to use as a loop cursor.
 * @n:		another ua_list_t * to use as temporary storage
 * @head:	the head for your list.
 */
#define ua_list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

/**
 * ua_list_for_each_entry  -   iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define ua_list_for_each_entry(pos, head, member) \
    for (pos = ua_list_entry((head)->next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = ua_list_entry(pos->member.next, typeof(*pos), member))

/**
 * ua_list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:    the type * to use as a loop cursor.
 * @n:      another type * to use as temporary storage
 * @head:   the head for your list.
 * @member: the name of the list_struct within the struct.
 */
#define ua_list_for_each_entry_safe(pos, n, head, member) \
    for (pos = ua_list_entry((head)->next, typeof(*pos), member), \
         n = ua_list_entry(pos->member.next, typeof(*pos), member); \
         &pos->member != (head); \
         pos = n, n = ua_list_entry(n->member.next, typeof(*n), member))

/**
 * ua_list_first_entry - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_struct within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define ua_list_first_entry(ptr, type, member) \
    ua_list_entry((ptr)->next, type, member)

#define UA_SLIST_OBJECT_INIT(object) { NULL }

/**
 * @brief initialize a single list
 *
 * @param l the single list to be initialized
 */
UA_INLINE void ua_slist_init(ua_slist_t *l)
{
    l->next = NULL;
}

UA_INLINE void ua_slist_append(ua_slist_t *l, ua_slist_t *n)
{
    struct ua_slist_node *node;

    node = l;
    while (node->next) node = node->next;

    /* append the node to the tail */
    node->next = n;
    n->next = NULL;
}

UA_INLINE void ua_slist_insert(ua_slist_t *l, ua_slist_t *n)
{
    n->next = l->next;
    l->next = n;
}

UA_INLINE unsigned int ua_slist_len(const ua_slist_t *l)
{
    unsigned int len = 0;
    const ua_slist_t *list = l->next;
    while (list != NULL)
    {
        list = list->next;
        len ++;
    }

    return len;
}

UA_INLINE ua_slist_t *ua_slist_remove(ua_slist_t *l, ua_slist_t *n)
{
    /* remove slist head */
    struct ua_slist_node *node = l;
    while (node->next && node->next != n) node = node->next;

    /* remove node */
    if (node->next != (ua_slist_t *)0) node->next = node->next->next;

    return l;
}

UA_INLINE ua_slist_t *ua_slist_first(ua_slist_t *l)
{
    return l->next;
}

UA_INLINE ua_slist_t *ua_slist_tail(ua_slist_t *l)
{
    while (l->next) l = l->next;

    return l;
}

UA_INLINE ua_slist_t *ua_slist_next(ua_slist_t *n)
{
    return n->next;
}

UA_INLINE int ua_slist_isempty(ua_slist_t *l)
{
    return l->next == NULL;
}

/**
 * @brief get the struct for this single list node
 * @param node the entry point
 * @param type the type of structure
 * @param member the name of list in structure
 */
#define ua_slist_entry(node, type, member) \
    ____container_of(node, type, member)

/**
 * ua_slist_for_each - iterate over a single list
 * @pos:    the ua_slist_t * to use as a loop cursor.
 * @head:   the head for your single list.
 */
#define ua_slist_for_each(pos, head) \
    for (pos = (head)->next; pos != NULL; pos = pos->next)

/**
 * ua_slist_for_each_entry  -   iterate over single list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your single list.
 * @member: the name of the list_struct within the struct.
 */
#define ua_slist_for_each_entry(pos, head, member) \
    for (pos = ua_slist_entry((head)->next, typeof(*pos), member); \
         &pos->member != (NULL); \
         pos = ua_slist_entry(pos->member.next, typeof(*pos), member))

/**
 * ua_slist_first_entry - get the first element from a slist
 * @ptr:    the slist head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the slist_struct within the struct.
 *
 * Note, that slist is expected to be not empty.
 */
#define ua_slist_first_entry(ptr, type, member) \
    ua_slist_entry((ptr)->next, type, member)

/**
 * ua_slist_tail_entry - get the tail element from a slist
 * @ptr:    the slist head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the slist_struct within the struct.
 *
 * Note, that slist is expected to be not empty.
 */
#define ua_slist_tail_entry(ptr, type, member) \
    ua_slist_entry(ua_slist_tail(ptr), type, member)

/**@}*/

#ifdef __cplusplus
}
#endif

#endif
