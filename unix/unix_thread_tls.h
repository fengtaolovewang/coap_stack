/*
*******************************************************************************

    unix_thread_tls.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_UNIX_THREAD_TLS_H
#define	_MACRO_PRIVATE_LIBRARY_UNIX_THREAD_TLS_H

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/**
 * @brief unix_thread_tls_t
 */

struct unix_thread_tls_s
{
	pthread_key_t data;
	int           free;
};

typedef struct unix_thread_tls_s unix_thread_tls_t;

/**
 * @brief unix_thread_tls_create
 */

static __inline unix_thread_tls_t * unix_thread_tls_create( unix_thread_tls_t * tls, void ( *destructor )( void * ), int * error )
{
	int code = ENOMEM;

	if ( ! tls )
	{
		if ( NULL != ( tls = ( unix_thread_tls_t * )calloc( 1, sizeof( unix_thread_tls_t ) ) ) )
		{
			tls->free = 1;

			if ( 0 != ( code = pthread_key_create( &tls->data, destructor ) ) )
			{
				free( tls );
			}
		}
	}
	else
	{
		memset( tls, 0, sizeof( unix_thread_tls_t ) );

		code = pthread_key_create( &tls->data, destructor );
	}

	if ( error )
	{
		*error = code;
	}

	return ( ! code ) ? ( tls ) : ( NULL );
}

/**
 * @brief unix_thread_tls_destroy
 */

static __inline void unix_thread_tls_destroy( unix_thread_tls_t * tls )
{
	if ( tls )
	{
		pthread_key_delete( tls->data );

		if ( 0 != tls->free )
		{
			free( tls );
		}
	}
}

/**
 * @brief unix_thread_tls_getspecific
 */

static __inline void * unix_thread_tls_getspecific( unix_thread_tls_t * tls )
{
	return ( ! tls ) ? ( ( void * )NULL ) : ( pthread_getspecific( tls->data ) );
}

/**
 * @brief unix_thread_tls_setspecific
 */

static __inline int unix_thread_tls_setspecific( unix_thread_tls_t * tls, const void * cpv )
{
	return ( ! tls ) ? ( EINVAL ) : ( pthread_setspecific( tls->data, cpv ) );
}

#endif	/* _MACRO_PRIVATE_LIBRARY_UNIX_THREAD_TLS_H */
