/*
*******************************************************************************

    unix_file.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_UNIX_FILE_H
#define	_MACRO_PRIVATE_LIBRARY_UNIX_FILE_H

#include <fcntl.h>
#include <unistd.h>

/**
 * @brief unix_file_close
 */

static __inline int unix_file_close( int fd )
{
	return close( fd );
}

/**
 * @brief unix_file_create
 */

static __inline int unix_file_create( const char * filename, mode_t mode )
{
	return creat( filename, mode );
}

/**
 * @brief unix_file_dup
 */

static __inline int unix_file_dup( int fd )
{
	return dup( fd );
}

/**
 * @brief unix_file_dup2
 */

static __inline int unix_file_dup2( int fd, int newfd )
{
	return dup2( fd, newfd );
}

/**
 * @brief unix_file_open
 */

static __inline int unix_file_open( const char * filename, int oflag )
{
	return open( filename, oflag );
}

/**
 * @brief unix_file_pread
 */

static __inline ssize_t unix_file_pread( int fd, void * buffer, size_t nbyte, off_t offset )
{
	return pread( fd, buffer, nbyte, offset );
}

/**
 * @brief unix_file_pwrite
 */

static __inline ssize_t unix_file_pwrite( int fd, const void * buffer, size_t nbyte, off_t offset )
{
	return pwrite( fd, buffer, nbyte, offset );
}

/**
 * @brief unix_file_read
 */

static __inline ssize_t unix_file_read( int fd, void * buffer, size_t nbyte )
{
	return read( fd, buffer, nbyte );
}

/**
 * @brief unix_file_seek
 */

static __inline off_t unix_file_seek( int fd, off_t offset, int whence )
{
	return lseek( fd, offset, whence );
}

/**
 * @brief unix_file_sync
 */

static __inline int unix_file_sync( int fd )
{
	return fsync( fd );
}

/**
 * @brief unix_file_tell
 */

static __inline off_t unix_file_tell( int fd )
{
	return lseek( fd, 0, SEEK_CUR );
}

/**
 * @brief unix_file_truncate
 */

static __inline int unix_file_truncate( int fd, off_t length )
{
	return ftruncate( fd, length );
}

/**
 * @brief unix_file_write
 */

static __inline ssize_t unix_file_write( int fd, const void * buffer, size_t nbyte )
{
	return write( fd, buffer, nbyte );
}

#endif	/* _MACRO_PRIVATE_LIBRARY_UNIX_FILE_H */
