/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 * The contents of this file are subject to the Netscape Public License
 * Version 1.0 (the "NPL"); you may not use this file except in
 * compliance with the NPL.  You may obtain a copy of the NPL at
 * http://www.mozilla.org/NPL/
 * 
 * Software distributed under the NPL is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the NPL
 * for the specific language governing rights and limitations under the
 * NPL.
 * 
 * The Initial Developer of this code under the NPL is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation.  All Rights
 * Reserved.
 */

#ifndef prclist_h___
#define prclist_h___

#include "prtypes.h"

typedef struct PRCListStr PRCList;

/*
** Circular linked list
*/
struct PRCListStr {
    PRCList	*next;
    PRCList	*prev;
};

/*
** Insert element "_e" into the list, before "_l".
*/
#define PR_INSERT_BEFORE(_e,_l)	 \
    PR_BEGIN_MACRO		 \
	(_e)->next = (_l);	 \
	(_e)->prev = (_l)->prev; \
	(_l)->prev->next = (_e); \
	(_l)->prev = (_e);	 \
    PR_END_MACRO

/*
** Insert element "_e" into the list, after "_l".
*/
#define PR_INSERT_AFTER(_e,_l)	 \
    PR_BEGIN_MACRO		 \
	(_e)->next = (_l)->next; \
	(_e)->prev = (_l);	 \
	(_l)->next->prev = (_e); \
	(_l)->next = (_e);	 \
    PR_END_MACRO

/*
** Return the element following element "_e"
*/
#define PR_NEXT_LINK(_e)	 \
    	((_e)->next)
/*
** Append an element "_e" to the end of the list "_l"
*/
#define PR_APPEND_LINK(_e,_l) PR_INSERT_BEFORE(_e,_l)

/*
** Insert an element "_e" at the head of the list "_l"
*/
#define PR_INSERT_LINK(_e,_l) PR_INSERT_AFTER(_e,_l)

/* Return the head/tail of the list */
#define PR_LIST_HEAD(_l) (_l)->next
#define PR_LIST_TAIL(_l) (_l)->prev

/*
** Remove the element "_e" from it's circular list.
*/
#define PR_REMOVE_LINK(_e)	       \
    PR_BEGIN_MACRO		       \
	(_e)->prev->next = (_e)->next; \
	(_e)->next->prev = (_e)->prev; \
    PR_END_MACRO

/*
** Remove the element "_e" from it's circular list. Also initializes the
** linkage.
*/
#define PR_REMOVE_AND_INIT_LINK(_e)    \
    PR_BEGIN_MACRO		       \
	(_e)->prev->next = (_e)->next; \
	(_e)->next->prev = (_e)->prev; \
	(_e)->next = (_e);	       \
	(_e)->prev = (_e);	       \
    PR_END_MACRO

/*
** Return non-zero if the given circular list "_l" is empty, zero if the
** circular list is not empty
*/
#define PR_CLIST_IS_EMPTY(_l) \
    ((_l)->next == (_l))

/*
** Initialize a circular list
*/
#define PR_INIT_CLIST(_l)  \
    PR_BEGIN_MACRO	   \
	(_l)->next = (_l); \
	(_l)->prev = (_l); \
    PR_END_MACRO

#define PR_INIT_STATIC_CLIST(_l) \
    {(_l), (_l)}

#endif /* prclist_h___ */
