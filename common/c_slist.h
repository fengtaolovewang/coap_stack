/*
*******************************************************************************

    c_slist.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_COMMON_C_SLIST_H
#define	_MACRO_PRIVATE_LIBRARY_COMMON_C_SLIST_H

#include <stdlib.h>

struct slist_node_t
{
	struct slist_node_t * next;
};

struct slist_head_t
{
	struct slist_node_t * first;
	struct slist_node_t * last;
};

typedef struct slist_node_t slist_node_t;
typedef struct slist_head_t slist_head_t;

/**
 * @brief c_slist_head_init
 */

static __inline slist_head_t * c_slist_head_init( slist_head_t * const head )
{
	head->first = NULL;
	head->last  = NULL;

	return head;
}

/**
 * @brief c_slist_first
 */

static __inline slist_node_t * c_slist_first( const slist_head_t * const head )
{
	return head->first;
}

/**
 * @brief c_slist_last
 */

static __inline slist_node_t * c_slist_last( const slist_head_t * const head )
{
	return head->last;
}

/**
 * @brief c_slist_link
 */

static __inline slist_node_t * c_slist_link( slist_node_t * const node, slist_node_t * const prev, slist_node_t * const next )
{
	prev->next = node;

	node->next = next;

	return node;
}

/**
 * @brief c_slist_empty
 */

static __inline int c_slist_empty( const slist_head_t * const head )
{
	return NULL == head->first && NULL == head->last;
}

/**
 * @brief c_slist_insert
 */

static __inline slist_node_t * c_slist_insert( slist_head_t * const head, const slist_node_t * const whence, slist_node_t * const node )
{
	if ( ! node )
	{
		return node;
	}
	else if ( c_slist_empty( head ) )
	{
		node->next = NULL;

		head->first = head->last = node;
	}
	else if ( NULL == whence )
	{
		c_slist_link( node, head->last, NULL );

		head->last = node;
	}
	else if ( head->first == whence )
	{
		node->next = head->first;

		head->first = node;
	}
	else
	{
		slist_node_t * next;

		slist_node_t * prev = head->first;

		for ( next = prev->next; next && next != whence; next = prev->next )
		{
			prev = next;
		}

		if ( next != whence )
		{
			return NULL;
		}

		c_slist_link( node, prev, next );
	}

	return node;
}

/**
 * @brief c_slist_remove
 */

static __inline slist_node_t * c_slist_remove( slist_head_t * const head, slist_node_t * const node, void ( *free )( slist_node_t *, void * ), void * param )
{
	slist_node_t * next;

	if ( ! node )
	{
		return node;
	}
	else if ( c_slist_empty( head ) )
	{
		return node;
	}
	else if ( node == head->first )
	{
		if ( head->first == head->last )
		{
			head->first = head->last = next = NULL;
		}
		else
		{
			head->first = next = node->next;
		}
	}
	else
	{
		slist_node_t * prev = head->first;

		for ( next = prev->next; next && next != node; next = prev->next )
		{
			prev = next;
		}

		if ( next != node )
		{
			return node;
		}

		prev->next = next = node->next;

		if ( node == head->last )
		{
			head->last = prev;
		}
	}

	if ( free )
	{
		free( node, param );
	}
	else
	{
		node->next = NULL;
	}

	return next;
}

/**
 * @brief c_slist_pop_back
 */

static __inline slist_node_t * c_slist_pop_back( slist_head_t * const head, void ( *free )( slist_node_t *, void * ), void * param )
{
	slist_node_t * node = head->last;

	c_slist_remove( head, node, free, param );

	return node;
}

/**
 * @brief c_slist_pop_front
 */

static __inline slist_node_t * c_slist_pop_front( slist_head_t * const head, void ( *free )( slist_node_t *, void * ), void * param )
{
	slist_node_t * node = head->first;

	c_slist_remove( head, node, free, param );

	return node;
}

/**
 * @brief c_slist_push_back
 */

static __inline slist_node_t * c_slist_push_back( slist_head_t * const head, slist_node_t * const node )
{
	return c_slist_insert( head, NULL, node );
}

/**
 * @brief c_slist_push_front
 */

static __inline slist_node_t * c_slist_push_front( slist_head_t * const head, slist_node_t * const node )
{
	return c_slist_insert( head, head->first, node );
}

/**
 * @brief c_slist_clear
 */

static __inline void c_slist_clear( slist_head_t * const head, void ( *free )( slist_node_t *, void * ), void * param )
{
	slist_node_t * node = head->first;

	head->first = head->last = NULL;

	while ( node )
	{
		slist_node_t * next = node->next;

		if ( free )
		{
			free( node, param );
		}
		else
		{
			node->next = NULL;
		}

		node = next;
	}
}

/**
 * @brief c_slist_for_each
 */

static __inline slist_node_t * c_slist_for_each( slist_head_t * const head, int ( *proc )( slist_node_t *, void * ), void * param )
{
	slist_node_t * node = head->first;

	while ( node )
	{
		slist_node_t * next = node->next;

		if ( proc( node, param ) )
		{
			break;
		}

		node = next;
	}

	return node;
}

/**
 * @brief c_slist_count
 */

static __inline unsigned int c_slist_count( const slist_head_t * const head )
{
	unsigned int count = 0;

	slist_node_t * node = head->first;

	while ( node )
	{
		++count;

		node = node->next;
	}

	return count;
}

/**
 * @brief c_slist_combine
 */

static __inline void c_slist_combine( slist_head_t * const head, const slist_node_t * const whence, slist_head_t * const node )
{
	if ( ( ! node ) || head == node )
	{
		return;
	}
	else if ( c_slist_empty( node ) )
	{
		return;
	}
	else if ( c_slist_empty( head ) )
	{
		head->first = node->first;
		head->last  = node->last;
	}
	else if ( NULL == whence )
	{
		head->last->next = node->first;

		head->last = node->last;
	}
	else if ( head->first == whence )
	{
		node->last->next = head->first;

		head->first = node->first;
	}
	else
	{
		slist_node_t * next;

		slist_node_t * prev = head->first;

		for ( next = prev->next; next && next != whence; next = prev->next )
		{
			prev = next;
		}

		if ( next != whence )
		{
			return;
		}

		prev->next = node->first;

		node->last->next = next;
	}

	node->first = node->last = NULL;
}

/**
 * @brief c_slist_remove_if
 */

static __inline slist_node_t * c_slist_remove_if( slist_head_t * const head, int ( *proc )( slist_node_t *, void * ), void ( *free )( slist_node_t *, void * ), void * param )
{
	slist_node_t * prev = NULL;

	slist_node_t * node = head->first;

	for ( ; node; node = node->next )
	{
		if ( ! proc( node, param ) )
		{
			prev = node;

			continue;
		}
		else if ( prev )
		{
			prev->next = node->next;

			if ( node == head->last )
			{
				head->last = prev;
			}
		}
		else
		{
			if ( node == head->last )
			{
				head->first = head->last = node->next;
			}
			else
			{
				head->first = node->next;
			}
		}

		if ( free )
		{
			free( node, param );
		}
		else
		{
			node->next = NULL;
		}

		return node;
	}

	return NULL;
}

#endif	/* _MACRO_PRIVATE_LIBRARY_COMMON_C_SLIST_H */
