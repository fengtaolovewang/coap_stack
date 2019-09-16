/*
*******************************************************************************

    unix_condition.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_UNIX_CONDITION_H
#define	_MACRO_PRIVATE_LIBRARY_UNIX_CONDITION_H

#include "unix_mutex.h"

/**
 * @brief unix_condition_t
 */

struct unix_condition_s
{
	pthread_cond_t data;
	int            free;
};

typedef struct unix_condition_s unix_condition_t;

/**
 * @brief unix_condition_create
 */

static __inline unix_condition_t * unix_condition_create( unix_condition_t * cond, const pthread_condattr_t * attr, int * error )
{
	int code = ENOMEM;

	if ( ! cond )
	{
		if ( NULL != ( cond = ( unix_condition_t * )calloc( 1, sizeof( unix_condition_t ) ) ) )
		{
			cond->free = 1;

			if ( 0 != ( code = pthread_cond_init( &cond->data, attr ) ) )
			{
				free( cond );
			}
		}
	}
	else
	{
		memset( cond, 0, sizeof( unix_condition_t ) );

		code = pthread_cond_init( &cond->data, attr );
	}

	if ( error )
	{
		*error = code;
	}

	return ( ! code ) ? ( cond ) : ( NULL );
}

/**
 * @brief unix_condition_destroy
 */

static __inline void unix_condition_destroy( unix_condition_t * cond )
{
	if ( cond )
	{
		pthread_cond_destroy( &cond->data );

		if ( 0 != cond->free )
		{
			free( cond );
		}
	}
}

/**
 * @brief unix_condition_broadcast
 */

static __inline int unix_condition_broadcast( unix_condition_t * cond )
{
	return ( ! cond ) ? ( EINVAL ) : ( pthread_cond_broadcast( &cond->data ) );
}

/**
 * @brief unix_condition_signal
 */

static __inline int unix_condition_signal( unix_condition_t * cond )
{
	return ( ! cond ) ? ( EINVAL ) : ( pthread_cond_signal( &cond->data ) );
}

/**
 * @brief unix_condition_timedwait
 */

static __inline int unix_condition_timedwait( unix_condition_t * cond, unix_mutex_t * mutex, const struct timespec * abstime )
{
	if ( ( ! cond ) || ( ! mutex ) )
	{
		return EINVAL;
	}
	else if ( ! abstime )
	{
		return pthread_cond_wait( &cond->data, &mutex->data );
	}
	else
	{
		return pthread_cond_timedwait( &cond->data, &mutex->data, abstime );
	}
}

/**
 * @brief unix_condition_wait
 */

static __inline int unix_condition_wait( unix_condition_t * cond, unix_mutex_t * mutex )
{
	return ( ( ! cond ) || ( ! mutex ) ) ? ( EINVAL ) : ( pthread_cond_wait( &cond->data, &mutex->data ) );
}

#endif	/* _MACRO_PRIVATE_LIBRARY_UNIX_CONDITION_H */
