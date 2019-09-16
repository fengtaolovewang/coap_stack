/*
*******************************************************************************

    unix_ipcsem.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_UNIX_IPCSEM_H
#define	_MACRO_PRIVATE_LIBRARY_UNIX_IPCSEM_H

#include <fcntl.h>
#include <stddef.h>
#include <sys/ipc.h>
#include <sys/sem.h>

/**
 * @brief unix_ipcsem_create
 */

static __inline int unix_ipcsem_create( key_t key, int nsems, int flags )
{
	return semget( key, nsems, flags );
}

/**
 * @brief unix_ipcsem_create_with_path
 */

static __inline int unix_ipcsem_create_with_path( const char * path, int id, int nsems, int flags )
{
	return semget( ftok( path, id ), nsems, flags );
}

/**
 * @brief unix_ipcsem_destroy
 */

static __inline int unix_ipcsem_destroy( int ctx )
{
	return semctl( ctx, 0, IPC_RMID, NULL );
}

/**
 * @brief unix_ipcsem_getvalue
 */

static __inline int unix_ipcsem_getvalue( int ctx, int num )
{
	return semctl( ctx, num, GETVAL, NULL );
}

/**
 * @brief unix_ipcsem_getvalues
 */

static __inline int unix_ipcsem_getvalues( int ctx, unsigned short * prv )
{
	return semctl( ctx, 0, GETALL, prv );
}

/**
 * @brief unix_ipcsem_setvalue
 */

static __inline int unix_ipcsem_setvalue( int ctx, int num, int val )
{
	return semctl( ctx, num, SETVAL, val );
}

/**
 * @brief unix_ipcsem_setvalues
 */

static __inline int unix_ipcsem_setvalues( int ctx, const unsigned short * pvs )
{
	return semctl( ctx, 0, SETALL, pvs );
}

/**
 * @brief unix_ipcsem_stat
 */

static __inline int unix_ipcsem_stat( int ctx, struct semid_ds * pds )
{
	return semctl( ctx, 0, IPC_STAT, pds );
}

/**
 * @brief unix_ipcsem_nsems
 */

static __inline int unix_ipcsem_nsems( int ctx )
{
	struct semid_ds rv;

	return ( 0 == unix_ipcsem_stat( ctx, &rv ) ) ? ( rv.sem_nsems ) : ( -1 );
}

/**
 * @brief unix_ipcsem_operation
 */

static __inline int unix_ipcsem_operation( int ctx, int num, short value, short flags )
{
	struct sembuf op = { ( unsigned short )num, value, flags };

	return semop( ctx, &op, 1 );
}

/**
 * @brief unix_ipcsem_operations
 */

static __inline int unix_ipcsem_operations( int ctx, struct sembuf * ops, size_t n )
{
	return semop( ctx, ops, n );
}

#endif	/* _MACRO_PRIVATE_LIBRARY_UNIX_IPCSEM_H */
