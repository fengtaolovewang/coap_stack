/*
*******************************************************************************

    unix_ipcshm.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_UNIX_IPCSHM_H
#define	_MACRO_PRIVATE_LIBRARY_UNIX_IPCSHM_H

#include <fcntl.h>
#include <stddef.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/**
 * @brief unix_ipcshm_create
 */

static __inline int unix_ipcshm_create( key_t key, size_t size, int flags )
{
	return shmget( key, size, flags );
}

/**
 * @brief unix_ipcshm_create_with_path
 */

static __inline int unix_ipcshm_create_with_path( const char * path, int id, size_t size, int flags )
{
	return shmget( ftok( path, id ), size, flags );
}

/**
 * @brief unix_ipcshm_destroy
 */

static __inline int unix_ipcshm_destroy( int ctx )
{
	return shmctl( ctx, IPC_RMID, NULL );
}

/**
 * @brief unix_ipcshm_map
 */

static __inline void * unix_ipcshm_map( int ctx, const void * addr, int flags )
{
	return shmat( ctx, addr, flags );
}

/**
 * @brief unix_ipcshm_unmap
 */

static __inline int unix_ipcshm_unmap( const void * addr )
{
	return shmdt( addr );
}

#endif	/* _MACRO_PRIVATE_LIBRARY_UNIX_IPCSHM_H */
