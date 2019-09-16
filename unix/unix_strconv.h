/*
*******************************************************************************

    unix_strconv.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_UNIX_STRCONV_H
#define	_MACRO_PRIVATE_LIBRARY_UNIX_STRCONV_H

#include <errno.h>
#include <wchar.h>
#include <string.h>

#include "unix_iconv.h"

/**
 * @brief unix_mbsconv
 */

static __inline int unix_mbsconv( char * output, size_t outlen, const wchar_t * source, size_t srclen )
{
	size_t length, rv;

	if ( ! output )
	{
		outlen = 0;
	}

	if ( ! source )
	{
		srclen = 0;
	}
	else if ( ( int )srclen < 0 )
	{
		srclen = wcslen( source );
	}

	outlen *= sizeof( char    );
	srclen *= sizeof( wchar_t );

	length = outlen;

	rv = unix_strconv( "UTF-32LE", ( char ** )&source, &srclen, "UTF-8", ( char ** )&output, &outlen );

	return ( ( size_t )-1 == rv && E2BIG != errno ) ? ( 0 ) : ( ( int )( ( length - outlen ) / sizeof( char ) ) );
}

/**
 * @brief unix_wcsconv
 */

static __inline int unix_wcsconv( wchar_t * output, size_t outlen, const char * source, size_t srclen )
{
	size_t length, rv;

	if ( ! output )
	{
		outlen = 0;
	}

	if ( ! source )
	{
		srclen = 0;
	}
	else if ( ( int )srclen < 0 )
	{
		srclen = strlen( source );
	}

	outlen *= sizeof( wchar_t );
	srclen *= sizeof( char    );

	length = outlen;

	rv = unix_strconv( "UTF-8", ( char ** )&source, &srclen, "UTF-32LE", ( char ** )&output, &outlen );

	return ( ( size_t )-1 == rv && E2BIG != errno ) ? ( 0 ) : ( ( int )( ( length - outlen ) / sizeof( wchar_t ) ) );
}

/**
 * @brief unix_mbstowcs
 */

static __inline wchar_t * unix_mbstowcs( wchar_t * buffer, size_t buflen, const char * source, size_t srclen )
{
	if ( buffer && buflen > 0 )
	{
		int rv = unix_wcsconv( buffer, buflen - 1, source, srclen );

		buffer[ ( rv < ( int )buflen ? rv : ( int )buflen - 1 ) ] = 0;
	}

	return buffer;
}

/**
 * @brief unix_wcstombs
 */

static __inline char * unix_wcstombs( char * buffer, size_t buflen, const wchar_t * source, size_t srclen )
{
	if ( buffer && buflen > 0 )
	{
		int rv = unix_mbsconv( buffer, buflen - 1, source, srclen );

		buffer[ ( rv < ( int )buflen ? rv : ( int )buflen - 1 ) ] = 0;
	}

	return buffer;
}

#endif	/* _MACRO_PRIVATE_LIBRARY_UNIX_STRCONV_H */
