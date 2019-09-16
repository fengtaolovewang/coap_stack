/*
*******************************************************************************

    unix_thread.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_UNIX_THREAD_H
#define	_MACRO_PRIVATE_LIBRARY_UNIX_THREAD_H

#include <errno.h>
#include <sched.h>
#include <stddef.h>
#include <unistd.h>
#include <pthread.h>

/**
 * @brief unix_thread_cancel
 */

static __inline int unix_thread_cancel( pthread_t handle )
{
	return pthread_cancel( handle );
}

/**
 * @brief unix_thread_create
 */

static __inline int unix_thread_create( pthread_t * pthread, const pthread_attr_t * attr, void * ( *routine )( void * ), void * param )
{
	int rv = EINVAL;

	if ( routine )
	{
		pthread_t hr;

		if ( 0 == ( rv = pthread_create( &hr, attr, routine, param ) ) )
		{
			if ( pthread )
			{
				*pthread = hr;
			}
			else
			{
				pthread_detach( hr );
			}
		}
	}

	return rv;
}

/**
 * @brief unix_thread_create_with_stack_size
 */

static __inline int unix_thread_create_with_stack_size( pthread_t * pthread, size_t stack_size, void * ( *routine )( void * ), void * param )
{
	int rv;

	if ( 0 == stack_size )
	{
		rv = unix_thread_create( pthread, NULL, routine, param );
	}
	else
	{
		pthread_attr_t attr;

		if ( 0 != ( rv = pthread_attr_init( &attr ) ) )
		{
			return rv;
		}

		if ( 0 == ( rv = pthread_attr_setstacksize( &attr, stack_size ) ) )
		{
			rv = unix_thread_create( pthread, &attr, routine, param );
		}

		pthread_attr_destroy( &attr );
	}

	return rv;
}

/**
 * @brief unix_thread_detach
 */

static __inline int unix_thread_detach( pthread_t handle )
{
	return pthread_detach( handle );
}

/**
 * @brief unix_thread_equal
 */

static __inline int unix_thread_equal( pthread_t one, pthread_t two )
{
	return pthread_equal( one, two );
}

/**
 * @brief unix_thread_exit
 */

static __inline void unix_thread_exit( void * retval )
{
	pthread_exit( retval );
}

/**
 * @brief unix_thread_join
 */

static __inline int unix_thread_join( pthread_t handle, void ** retval )
{
	return pthread_join( handle, retval );
}

/**
 * @brief unix_thread_self
 */

static __inline pthread_t unix_thread_self( void )
{
	return pthread_self();
}

/**
 * @brief unix_thread_sleep
 */

static __inline void unix_thread_sleep( unsigned int msec )
{
	usleep( msec * 1000 );
}

/**
 * @brief unix_thread_yield
 */

static __inline int unix_thread_yield( void )
{
	#if defined( __APPLE__ )

	return sched_yield();

	#else

	return pthread_yield();

	#endif
}

/**
 * @brief unix_thread_begin
 */

static __inline int unix_thread_begin( pthread_t * pthread, void * ( *routine )( void * ), void * param )
{
	return unix_thread_create( pthread, NULL, routine, param );
}

#endif	/* _MACRO_PRIVATE_LIBRARY_UNIX_THREAD_H */
