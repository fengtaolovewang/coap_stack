/*
*******************************************************************************

    unix_time.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_UNIX_TIME_H
#define	_MACRO_PRIVATE_LIBRARY_UNIX_TIME_H

#include <time.h>
#include <stdint.h>
#include <sys/time.h>

#if defined( __MACH__ )
#include <mach/mach_time.h>
#endif

/**
 * @brief unix_get_tick_count
 */

static __inline uint64_t unix_get_tick_count( void )
{
	#if defined( __MACH__ )

	uint64_t                  time;
	mach_timebase_info_data_t timebase;

	mach_timebase_info( &timebase );

	time = mach_absolute_time();

	return ( uint64_t )( ( ( double )time * ( double )timebase.numer ) / ( ( double )timebase.denom * 1e6 ) );

	#elif defined( __linux__ )

	struct timespec ts;

	clock_gettime( CLOCK_MONOTONIC, &ts );

	return ( uint64_t )ts.tv_sec * 1000 + ( uint64_t )ts.tv_nsec / 1000000;

	#else

	#error Unrealized platform of unix_get_tick_count

	#endif
}

/**
 * @brief unix_timespec_delay_msec
 */

static __inline struct timespec * unix_timespec_delay_msec( struct timespec * ts, long msec )
{
	struct timeval ctv;

	gettimeofday( &ctv, 0 );

	ts->tv_sec  = msec / 1000 + ctv.tv_sec;
	ts->tv_nsec = ctv.tv_usec;

	ts->tv_nsec *= 1000;
	ts->tv_nsec += ( msec % 1000 ) * 1000000;

	ts->tv_sec += ts->tv_nsec / 1000000000;
	ts->tv_nsec = ts->tv_nsec % 1000000000;

	return ts;
}

#endif	/* _MACRO_PRIVATE_LIBRARY_UNIX_TIME_H */
