/*
*******************************************************************************

    unix_iconv.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_UNIX_ICONV_H
#define	_MACRO_PRIVATE_LIBRARY_UNIX_ICONV_H

#include <iconv.h>

/**
 * @brief unix_strconv
 */

static __inline size_t unix_strconv( const char * fromcode, char ** inbuf, size_t * inbytes, const char * tocode, char ** outbuf, size_t * outbytes )
{
	size_t rv = ( size_t )-1;

	iconv_t id = iconv_open( tocode, fromcode );

	if ( id != ( iconv_t )-1 )
	{
		rv = iconv( id, inbuf, inbytes, outbuf, outbytes );

		iconv_close( id );
	}

	return rv;
}

#endif	/* _MACRO_PRIVATE_LIBRARY_UNIX_ICONV_H */
