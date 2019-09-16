/*
*******************************************************************************

    unix_semaphore.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_UNIX_SEMAPHORE_H
#define	_MACRO_PRIVATE_LIBRARY_UNIX_SEMAPHORE_H

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <common/common_macro.h>

/**
 * @brief unix_semaphore_t
 */

struct unix_semaphore_s
{
	sem_t data;
	int   free;
};

typedef struct unix_semaphore_s unix_semaphore_t;

/**
 * @brief unix_semaphore_create
 */

static __inline unix_semaphore_t * unix_semaphore_create( unix_semaphore_t * sem, unsigned int value )
{
	int rv;

	if ( ! sem )
	{
		if ( NULL != ( sem = ( unix_semaphore_t * )calloc( 1, sizeof( unix_semaphore_t ) ) ) )
		{
			sem->free = 1;

			if ( 0 != ( rv = sem_init( &sem->data, 0, value ) ) )
			{
				free( sem );
			}
		}
	}
	else
	{
		memset( sem, 0, sizeof( unix_semaphore_t ) );

		rv = sem_init( &sem->data, 0, value );
	}

	return ( 0 == rv ) ? ( sem ) : ( NULL );
}

/**
 * @brief unix_semaphore_destroy
 */

static __inline void unix_semaphore_destroy( unix_semaphore_t * sem )
{
	if ( sem )
	{
		sem_destroy( &sem->data );

		if ( 0 != sem->free )
		{
			free( sem );
		}
	}
}

/**
 * @brief unix_semaphore_post
 */

static __inline int unix_semaphore_post( unix_semaphore_t * sem )
{
	return ( ! sem ) ? ( -1 ) : ( sem_post( &sem->data ) );
}

/**
 * @brief unix_semaphore_timedwait
 */

static __inline int unix_semaphore_timedwait( unix_semaphore_t * sem, const struct timespec * abstime )
{
	#if ( _POSIX_C_SOURCE >= 200112L ) || ( _XOPEN_SOURCE >= 600 )

	return ( ! sem ) ? ( -1 ) : ( sem_timedwait( &sem->data, abstime ) );

	#else

	USED_PARAMETER( sem     );
	USED_PARAMETER( abstime );

	return -1;

	#endif
}

/**
 * @brief unix_semaphore_trywait
 */

static __inline int unix_semaphore_trywait( unix_semaphore_t * sem )
{
	return ( ! sem ) ? ( -1 ) : ( sem_trywait( &sem->data ) );
}

/**
 * @brief unix_semaphore_wait
 */

static __inline int unix_semaphore_wait( unix_semaphore_t * sem )
{
	return ( ! sem ) ? ( -1 ) : ( sem_wait( &sem->data ) );
}

#endif	/* _MACRO_PRIVATE_LIBRARY_UNIX_SEMAPHORE_H */
