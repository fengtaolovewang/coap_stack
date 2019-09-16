/*
*******************************************************************************

    unix_rwlock.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_UNIX_RWLOCK_H
#define	_MACRO_PRIVATE_LIBRARY_UNIX_RWLOCK_H

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/**
 * @brief unix_rwlock_t
 */

struct unix_rwlock_s
{
	pthread_rwlock_t data;
	int              free;
};

typedef struct unix_rwlock_s unix_rwlock_t;

/**
 * @brief unix_rwlock_create
 */

static __inline unix_rwlock_t * unix_rwlock_create( unix_rwlock_t * rwlock, const pthread_rwlockattr_t * attr, int * error )
{
	int code = ENOMEM;

	if ( ! rwlock )
	{
		if ( NULL != ( rwlock = ( unix_rwlock_t * )calloc( 1, sizeof( unix_rwlock_t ) ) ) )
		{
			rwlock->free = 1;

			if ( 0 != ( code = pthread_rwlock_init( &rwlock->data, attr ) ) )
			{
				free( rwlock );
			}
		}
	}
	else
	{
		memset( rwlock, 0, sizeof( unix_rwlock_t ) );

		code = pthread_rwlock_init( &rwlock->data, attr );
	}

	if ( error )
	{
		*error = code;
	}

	return ( ! code ) ? ( rwlock ) : ( NULL );
}

/**
 * @brief unix_rwlock_destroy
 */

static __inline void unix_rwlock_destroy( unix_rwlock_t * rwlock )
{
	if ( rwlock )
	{
		pthread_rwlock_destroy( &rwlock->data );

		if ( 0 != rwlock->free )
		{
			free( rwlock );
		}
	}
}

/**
 * @brief unix_rwlock_rdlock
 */

static __inline int unix_rwlock_rdlock( unix_rwlock_t * rwlock )
{
	return ( ! rwlock ) ? ( EINVAL ) : ( pthread_rwlock_rdlock( &rwlock->data ) );
}

/**
 * @brief unix_rwlock_tryrdlock
 */

static __inline int unix_rwlock_tryrdlock( unix_rwlock_t * rwlock )
{
	return ( ! rwlock ) ? ( EINVAL ) : ( pthread_rwlock_tryrdlock( &rwlock->data ) );
}

/**
 * @brief unix_rwlock_trywrlock
 */

static __inline int unix_rwlock_trywrlock( unix_rwlock_t * rwlock )
{
	return ( ! rwlock ) ? ( EINVAL ) : ( pthread_rwlock_trywrlock( &rwlock->data ) );
}

/**
 * @brief unix_rwlock_unlock
 */

static __inline int unix_rwlock_unlock( unix_rwlock_t * rwlock )
{
	return ( ! rwlock ) ? ( EINVAL ) : ( pthread_rwlock_unlock( &rwlock->data ) );
}

/**
 * @brief unix_rwlock_wrlock
 */

static __inline int unix_rwlock_wrlock( unix_rwlock_t * rwlock )
{
	return ( ! rwlock ) ? ( EINVAL ) : ( pthread_rwlock_wrlock( &rwlock->data ) );
}

#endif	/* _MACRO_PRIVATE_LIBRARY_UNIX_RWLOCK_H */
