/*
*******************************************************************************

    unix_spinlock.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_UNIX_SPINLOCK_H
#define	_MACRO_PRIVATE_LIBRARY_UNIX_SPINLOCK_H

#include <time.h>
#include <sched.h>
#include <pthread.h>

/**
 * @brief unix_spinlock_trylock
 */

static __inline int unix_spinlock_trylock( int * sp )
{
	return 0 == __sync_lock_test_and_set( sp, 1 );
}

/**
 * @brief unix_spinlock_lock
 */

static __inline void unix_spinlock_lock( int * sp )
{
	for ( unsigned int i = 0; ! unix_spinlock_trylock( sp ); ++i )
	{
		if ( i < 4 )
		{
		}
		else if ( i < 32 )
		{
			#if defined( __APPLE__ )

			sched_yield();

			#else

			pthread_yield();

			#endif
		}
		else
		{
			struct timespec rqtp = { 0 };

			rqtp.tv_sec  = 0;
			rqtp.tv_nsec = 1000;

			nanosleep( &rqtp, NULL );
		}
	}
}

/**
 * @brief unix_spinlock_unlock
 */

static __inline void unix_spinlock_unlock( int * sp )
{
	__sync_lock_release( sp );
}

#endif	/* _MACRO_PRIVATE_LIBRARY_UNIX_SPINLOCK_H */
