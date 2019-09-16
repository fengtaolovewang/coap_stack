/*
*******************************************************************************

    c_socket.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_COMMON_C_SOCKET_H
#define	_MACRO_PRIVATE_LIBRARY_COMMON_C_SOCKET_H

#if defined( _WIN32 )
#include <winsock2.h>
#else
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#endif

#ifndef	INVALID_SOCKET
#define	INVALID_SOCKET	( -1 )
#endif

#if defined( _WIN32 )
typedef SOCKET socket_t;
typedef int    ssize_t;
typedef int    socklen_t;
#else
typedef int    socket_t;
#endif

/**
 * @brief socket_accept
 */

static __inline socket_t socket_accept( socket_t s, struct sockaddr * addr, socklen_t * addrlen )
{
	return accept( s, addr, addrlen );
}

/**
 * @brief socket_bind
 */

static __inline int socket_bind( socket_t s, const struct sockaddr * addr, socklen_t addrlen )
{
	return bind( s, addr, addrlen );
}

/**
 * @brief socket_close
 */

static __inline int socket_close( socket_t s )
{
	#if defined( _WIN32 )

	return closesocket( s );

	#else

	return close( s );

	#endif
}

/**
 * @brief socket_connect
 */

static __inline int socket_connect( socket_t s, const struct sockaddr * addr, socklen_t addrlen )
{
	return connect( s, addr, addrlen );
}

/**
 * @brief socket_get_error
 */

static __inline int socket_get_error()
{
	#if defined( _WIN32 )

	return WSAGetLastError();

	#else

	return errno;

	#endif
}

/**
 * @brief socket_getsockopt
 */

static __inline int socket_getsockopt( socket_t s, int level, int optname, void * optval, socklen_t * optlen )
{
	#if defined( _WIN32 )

	return getsockopt( s, level, optname, ( char * )optval, optlen );

	#else

	return getsockopt( s, level, optname, optval, optlen );

	#endif
}

/**
 * @brief socket_listen
 */

static __inline int socket_listen( socket_t s, int backlog )
{
	return listen( s, backlog );
}

/**
 * @brief socket_open
 */

static __inline socket_t socket_open( int domain, int type, int protocol )
{
	return socket( domain, type, protocol );
}

/**
 * @brief socket_recv
 */

static __inline ssize_t socket_recv( socket_t s, void * buf, size_t buflen, int flags )
{
	#if defined( _WIN32 )

	return recv( s, ( char * )buf, ( int )buflen, flags );

	#else

	return recv( s, buf, buflen, flags );

	#endif
}

/**
 * @brief socket_recvfrom
 */

static __inline ssize_t socket_recvfrom( socket_t s, void * buf, size_t buflen, int flags, struct sockaddr * addr, socklen_t * addrlen )
{
	#if defined( _WIN32 )

	return recvfrom( s, ( char * )buf, ( int )buflen, flags, addr, addrlen );

	#else

	return recvfrom( s, buf, buflen, flags, addr, addrlen );

	#endif
}

/**
 * @brief socket_select
 */

static __inline int socket_select( int nfds, fd_set * readfds, fd_set * writefds, fd_set * errorfds, struct timeval * timeout )
{
	return select( nfds, readfds, writefds, errorfds, timeout );
}

/**
 * @brief socket_send
 */

static __inline ssize_t socket_send( socket_t s, const void * buf, size_t buflen, int flags )
{
	#if defined( _WIN32 )

	return send( s, ( const char * )buf, ( int )buflen, flags );

	#else

	return send( s, buf, buflen, flags );

	#endif
}

/**
 * @brief socket_sendto
 */

static __inline ssize_t socket_sendto( socket_t s, const void * buf, size_t buflen, int flags, const struct sockaddr * addr, socklen_t addrlen )
{
	#if defined( _WIN32 )

	return sendto( s, ( const char * )buf, ( int )buflen, flags, addr, addrlen );

	#else

	return sendto( s, buf, buflen, flags, addr, addrlen );

	#endif
}

/**
 * @brief socket_setsockopt
 */

static __inline int socket_setsockopt( socket_t s, int level, int optname, const void * optval, socklen_t optlen )
{
	#if defined( _WIN32 )

	return setsockopt( s, level, optname, ( const char * )optval, optlen );

	#else

	return setsockopt( s, level, optname, optval, optlen );

	#endif
}

/**
 * @brief socket_shutdown
 */

static __inline int socket_shutdown( socket_t s, int how )
{
	return shutdown( s, how );
}

/**
 * @brief socket_set_non_block
 */

static __inline int socket_set_non_block( socket_t s )
{
	#if defined( _WIN32 )

	u_long yes = 1;

	return ( SOCKET_ERROR == ioctlsocket( s, FIONBIO, &yes ) ) ? ( -1 ) : ( 0 );

	#else

	int flags;

	if ( -1 == ( flags = fcntl( s, F_GETFL ) ) )
	{
		return -1;
	}

	if ( -1 == fcntl( s, F_SETFL, flags | O_NONBLOCK ) )
	{
		return -1;
	}

	return 0;

	#endif
}

/**
 * @brief wait_for_single_socket
 */

#if defined( _MSC_VER )
#pragma warning( disable: 4127 )
#endif

static __inline int wait_for_single_socket( socket_t s, unsigned int msec, int flags )
{
	int              rv = -1;
	fd_set           rdfds;
	fd_set           wrfds;
	fd_set           exfds;
	struct timeval   tv;
	fd_set *         prdfds = NULL;
	fd_set *         pwrfds = NULL;
	fd_set *         pexfds = NULL;
	struct timeval * ptv    = NULL;

	if ( INVALID_SOCKET == s )
	{
		return rv;
	}

	if ( 0x01 == ( flags & 0x01 ) )
	{
		FD_ZERO( &rdfds );

		FD_SET( s, &rdfds );

		prdfds = &rdfds;
	}

	if ( 0x02 == ( flags & 0x02 ) )
	{
		FD_ZERO( &wrfds );

		FD_SET( s, &wrfds );

		pwrfds = &wrfds;
	}

	if ( 0x04 == ( flags & 0x04 ) )
	{
		FD_ZERO( &exfds );

		FD_SET( s, &exfds );

		pexfds = &exfds;
	}

	if ( ( unsigned int )-1 != msec )
	{
		tv.tv_sec  = ( int )( msec / 1000 );
		tv.tv_usec = ( int )( msec % 1000 ) * 1000;

		ptv = &tv;
	}

	if ( ( rv = select( ( int )( s + 1 ), prdfds, pwrfds, pexfds, ptv ) ) > 0 )
	{
		rv = 0;

		if ( prdfds && FD_ISSET( s, prdfds ) ) rv |= 0x01;
		if ( pwrfds && FD_ISSET( s, pwrfds ) ) rv |= 0x02;
		if ( pexfds && FD_ISSET( s, pexfds ) ) rv |= 0x04;
	}

	return rv;
}

#if defined( _MSC_VER )
#pragma warning( default: 4127 )
#endif

/**
 * @brief wait_read_for_single_socket
 */

#if defined( _MSC_VER )
#pragma warning( disable: 4127 )
#endif

static __inline int wait_read_for_single_socket( socket_t s, unsigned int msec )
{
	int              rv = -1;
	fd_set           rdfds;
	struct timeval   tv;
	struct timeval * ptv = NULL;

	if ( INVALID_SOCKET == s )
	{
		return rv;
	}

	FD_ZERO( &rdfds );

	FD_SET( s, &rdfds );

	if ( ( unsigned int )-1 != msec )
	{
		tv.tv_sec  = ( int )( msec / 1000 );
		tv.tv_usec = ( int )( msec % 1000 ) * 1000;

		ptv = &tv;
	}

	if ( ( rv = select( ( int )( s + 1 ), &rdfds, NULL, NULL, ptv ) ) <= 0 )
	{
		return rv;
	}

	return ! ( ! FD_ISSET( s, &rdfds ) );
}

#if defined( _MSC_VER )
#pragma warning( default: 4127 )
#endif

#endif	/* _MACRO_PRIVATE_LIBRARY_COMMON_C_SOCKET_H */
