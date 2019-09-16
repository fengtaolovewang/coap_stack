/*
*******************************************************************************

    unix_event.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_UNIX_EVENT_H
#define	_MACRO_PRIVATE_LIBRARY_UNIX_EVENT_H

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/**
 * @brief unix_event_t
 */

struct unix_event_s
{
	int             mode;
	volatile int    data;
	pthread_cond_t  cond;
	pthread_mutex_t lock;
	int             free;
};

typedef struct unix_event_s unix_event_t;

/**
 * @brief unix_event_create
 */

static __inline unix_event_t * unix_event_create( unix_event_t * event, int state, int manual, int * error )
{
	int code = ENOMEM;

	if ( ! event )
	{
		if ( NULL != ( event = ( unix_event_t * )calloc( 1, sizeof( unix_event_t ) ) ) )
		{
			event->free = 1;

			if ( 0 == ( code = pthread_cond_init( &event->cond, NULL ) ) )
			{
				if ( 0 != ( code = pthread_mutex_init( &event->lock, NULL ) ) )
				{
					pthread_cond_destroy( &event->cond );
				}
			}

			if ( 0 != code )
			{
				free( event );
			}
		}
	}
	else
	{
		memset( event, 0, sizeof( unix_event_t ) );

		if ( 0 == ( code = pthread_cond_init( &event->cond, NULL ) ) )
		{
			if ( 0 != ( code = pthread_mutex_init( &event->lock, NULL ) ) )
			{
				pthread_cond_destroy( &event->cond );
			}
		}
	}

	if ( ! code )
	{
		event->mode = manual;
		event->data = state;
	}

	if ( error )
	{
		*error = code;
	}

	return ( ! code ) ? ( event ) : ( NULL );
}

/**
 * @brief unix_event_destroy
 */

static __inline void unix_event_destroy( unix_event_t * event )
{
	if ( event )
	{
		pthread_cond_destroy( &event->cond );

		pthread_mutex_destroy( &event->lock );

		if ( 0 != event->free )
		{
			free( event );
		}
	}
}

/**
 * @brief unix_event_reset
 */

static __inline int unix_event_reset( unix_event_t * event )
{
	int code;

	if ( ! event )
	{
		return EINVAL;
	}

	if ( 0 == ( code = pthread_mutex_lock( &event->lock ) ) )
	{
		event->data = 0;

		code = pthread_mutex_unlock( &event->lock );
	}

	return code;
}

/**
 * @brief unix_event_set
 */

static __inline int unix_event_set( unix_event_t * event )
{
	int code;

	if ( ! event )
	{
		return EINVAL;
	}

	if ( 0 == ( code = pthread_mutex_lock( &event->lock ) ) )
	{
		event->data = 1;

		if ( 0 == ( code = pthread_mutex_unlock( &event->lock ) ) )
		{
			code = ( ! event->mode ? pthread_cond_signal( &event->cond ) : pthread_cond_broadcast( &event->cond ) );
		}
	}

	return code;
}

/**
 * @brief unix_event_timedwait
 */

static __inline int unix_event_timedwait( unix_event_t * event, const struct timespec * abstime )
{
	int code;

	if ( ! event )
	{
		return EINVAL;
	}

	if ( 0 == ( code = pthread_mutex_lock( &event->lock ) ) )
	{
		while ( ( ! code ) && ( ! event->data ) )
		{
			code = pthread_cond_timedwait( &event->cond, &event->lock, abstime );
		}

		if ( ( ! code ) && ( ! event->mode ) )
		{
			event->data = 0;
		}

		pthread_mutex_unlock( &event->lock );
	}

	return code;
}

/**
 * @brief unix_event_wait
 */

static __inline int unix_event_wait( unix_event_t * event )
{
	int code;

	if ( ! event )
	{
		return EINVAL;
	}

	if ( 0 == ( code = pthread_mutex_lock( &event->lock ) ) )
	{
		while ( ( ! code ) && ( ! event->data ) )
		{
			code = pthread_cond_wait( &event->cond, &event->lock );
		}

		if ( ( ! code ) && ( ! event->mode ) )
		{
			event->data = 0;
		}

		pthread_mutex_unlock( &event->lock );
	}

	return code;
}

#endif	/* _MACRO_PRIVATE_LIBRARY_UNIX_EVENT_H */
