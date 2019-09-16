/*
*******************************************************************************

    unix_ipcmsg.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_UNIX_IPCMSG_H
#define	_MACRO_PRIVATE_LIBRARY_UNIX_IPCMSG_H

#include <fcntl.h>
#include <stddef.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/**
 * @brief unix_ipcmsg_create
 */

static __inline int unix_ipcmsg_create( key_t key, int flags )
{
	return msgget( key, flags );
}

/**
 * @brief unix_ipcmsg_create_with_path
 */

static __inline int unix_ipcmsg_create_with_path( const char * path, int id, int flags )
{
	return msgget( ftok( path, id ), flags );
}

/**
 * @brief unix_ipcmsg_destroy
 */

static __inline int unix_ipcmsg_destroy( int ctx )
{
	return msgctl( ctx, IPC_RMID, NULL );
}

/**
 * @brief unix_ipcmsg_recv
 */

static __inline ssize_t unix_ipcmsg_recv( int ctx, void * buffer, size_t buflen, long type, int flags )
{
	return msgrcv( ctx, buffer, buflen, type, flags );
}

/**
 * @brief unix_ipcmsg_send
 */

static __inline int unix_ipcmsg_send( int ctx, const void * data, size_t size, int flags )
{
	return msgsnd( ctx, data, size, flags );
}

#endif	/* _MACRO_PRIVATE_LIBRARY_UNIX_IPCMSG_H */
