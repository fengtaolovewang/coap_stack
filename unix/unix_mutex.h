/*
*******************************************************************************

    unix_mutex.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_UNIX_MUTEX_H
#define	_MACRO_PRIVATE_LIBRARY_UNIX_MUTEX_H

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/**
 * @brief unix_mutex_t
 */

struct unix_mutex_s
{
	pthread_mutex_t data;
	int             free;
};

typedef struct unix_mutex_s unix_mutex_t;

/**
 * @brief unix_mutex_create
 */

static __inline unix_mutex_t * unix_mutex_create( unix_mutex_t * mutex, const pthread_mutexattr_t * attr, int * error )
{
	int code = ENOMEM;

	if ( ! mutex )
	{
		if ( NULL != ( mutex = ( unix_mutex_t * )calloc( 1, sizeof( unix_mutex_t ) ) ) )
		{
			mutex->free = 1;

			if ( 0 != ( code = pthread_mutex_init( &mutex->data, attr ) ) )
			{
				free( mutex );
			}
		}
	}
	else
	{
		memset( mutex, 0, sizeof( unix_mutex_t ) );

		code = pthread_mutex_init( &mutex->data, attr );
	}

	if ( error )
	{
		*error = code;
	}

	return ( ! code ) ? ( mutex ) : ( NULL );
}

/**
 * @brief unix_mutex_destroy
 */

static __inline void unix_mutex_destroy( unix_mutex_t * mutex )
{
	if ( mutex )
	{
		pthread_mutex_destroy( &mutex->data );

		if ( 0 != mutex->free )
		{
			free( mutex );
		}
	}
}

/**
 * @brief unix_mutex_lock
 */

static __inline int unix_mutex_lock( unix_mutex_t * mutex )
{
	return ( ! mutex ) ? ( EINVAL ) : ( pthread_mutex_lock( &mutex->data ) );
}

/**
 * @brief unix_mutex_trylock
 */

static __inline int unix_mutex_trylock( unix_mutex_t * mutex )
{
	return ( ! mutex ) ? ( EINVAL ) : ( pthread_mutex_trylock( &mutex->data ) );
}

/**
 * @brief unix_mutex_unlock
 */

static __inline int unix_mutex_unlock( unix_mutex_t * mutex )
{
	return ( ! mutex ) ? ( EINVAL ) : ( pthread_mutex_unlock( &mutex->data ) );
}

/**
 * @brief unix_mutex_create_with_recursive
 */

static __inline unix_mutex_t * unix_mutex_create_with_recursive( unix_mutex_t * mutex, int * error )
{
	int                 code;
	pthread_mutexattr_t attr;

	if ( 0 == ( code = pthread_mutexattr_init( &attr ) ) )
	{
		if ( 0 == ( code = pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE ) ) )
		{
			mutex = unix_mutex_create( mutex, &attr, &code );
		}

		pthread_mutexattr_destroy( &attr );
	}

	if ( error )
	{
		*error = code;
	}

	return ( ! code ) ? ( mutex ) : ( NULL );
}

#endif	/* _MACRO_PRIVATE_LIBRARY_UNIX_MUTEX_H */
