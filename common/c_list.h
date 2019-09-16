/*
*******************************************************************************

    c_list.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_COMMON_C_LIST_H
#define	_MACRO_PRIVATE_LIBRARY_COMMON_C_LIST_H

#include <stdlib.h>

/**
 * @brief list_head_t
 */

struct list_head_t
{
	struct list_head_t * prev;
	struct list_head_t * next;
};

typedef struct list_head_t list_head_t;

/**
 * @brief c_list_head_init
 */

static __inline list_head_t * c_list_head_init( list_head_t * const head )
{
	head->prev = head;
	head->next = head;

	return head;
}

/**
 * @brief c_list_begin
 */

static __inline list_head_t * c_list_begin( const list_head_t * const head )
{
	return head->next;
}

/**
 * @brief c_list_end
 */

static __inline list_head_t * c_list_end( const list_head_t * const head )
{
	return ( list_head_t * )head;
}

/**
 * @brief c_list_rbegin
 */

static __inline list_head_t * c_list_rbegin( const list_head_t * const head )
{
	return head->prev;
}

/**
 * @brief c_list_rend
 */

static __inline list_head_t * c_list_rend( const list_head_t * const head )
{
	return ( list_head_t * )head;
}

/**
 * @brief c_list_empty
 */

static __inline int c_list_empty( const list_head_t * const head )
{
	return head == head->next;
}

/**
 * @brief c_list_link
 */

static __inline list_head_t * c_list_link( list_head_t * const node, list_head_t * const prev, list_head_t * const next )
{
	prev->next = node;

	node->prev = prev;
	node->next = next;

	next->prev = node;

	return node;
}

/**
 * @brief c_list_insert
 */

static __inline list_head_t * c_list_insert( const list_head_t * const whence, list_head_t * const node )
{
	return c_list_link( node, whence->prev, ( list_head_t * )whence );
}

/**
 * @brief c_list_remove
 */

static __inline list_head_t * c_list_remove( list_head_t * const node, void ( *free )( list_head_t *, void * ), void * param )
{
	list_head_t * next = node->next;

	node->prev->next = node->next;
	node->next->prev = node->prev;

	node->prev = node;
	node->next = node;

	if ( free )
	{
		free( node, param );
	}
	else
	{
		node->prev = NULL;
		node->next = NULL;
	}

	return next;
}

/**
 * @brief c_list_pop_back
 */

static __inline list_head_t * c_list_pop_back( list_head_t * const head, void ( *free )( list_head_t *, void * ), void * param )
{
	list_head_t * node = head->prev;

	if ( head != node )
	{
		c_list_remove( node, free, param );
	}

	return node;
}

/**
 * @brief c_list_pop_front
 */

static __inline list_head_t * c_list_pop_front( list_head_t * const head, void ( *free )( list_head_t *, void * ), void * param )
{
	list_head_t * node = head->next;

	if ( head != node )
	{
		c_list_remove( node, free, param );
	}

	return node;
}

/**
 * @brief c_list_push_back
 */

static __inline list_head_t * c_list_push_back( list_head_t * const head, list_head_t * const node )
{
	return c_list_link( node, head->prev, head );
}

/**
 * @brief c_list_push_front
 */

static __inline list_head_t * c_list_push_front( list_head_t * const head, list_head_t * const node )
{
	return c_list_link( node, head, head->next );
}

/**
 * @brief c_list_clear
 */

static __inline void c_list_clear( list_head_t * const head, void ( *free )( list_head_t *, void * ), void * param )
{
	list_head_t *first, *last, *next;

	for ( first = c_list_begin( head ), last = c_list_end( head ); first != last; first = next )
	{
		next = c_list_remove( first, free, param );
	}
}

/**
 * @brief c_list_for_each
 */

static __inline list_head_t * c_list_for_each( list_head_t * const head, int reverse, int ( *proc )( list_head_t *, void * ), void * param )
{
	list_head_t *first, *last, *next;

	if ( reverse )
	{
		for ( first = c_list_rbegin( head ), last = c_list_rend( head ); first != last; first = next )
		{
			next = first->prev;

			if ( proc( first, param ) )
			{
				break;
			}
		}
	}
	else
	{
		for ( first = c_list_begin( head ), last = c_list_end( head ); first != last; first = next )
		{
			next = first->next;

			if ( proc( first, param ) )
			{
				break;
			}
		}
	}

	return first;
}

/**
 * @brief c_list_count
 */

static __inline unsigned int c_list_count( const list_head_t * const head )
{
	unsigned int count = 0;

	list_head_t *first, *last;

	for ( first = c_list_begin( head ), last = c_list_end( head ); first != last; first = first->next )
	{
		++count;
	}

	return count;
}

#endif	/* _MACRO_PRIVATE_LIBRARY_COMMON_C_LIST_H */
