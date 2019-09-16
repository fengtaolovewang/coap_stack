/*
*******************************************************************************

    c_bits_reader.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_COMMON_C_BITS_READER_H
#define	_MACRO_PRIVATE_LIBRARY_COMMON_C_BITS_READER_H

#include <stdio.h>
#include <stdint.h>

/**
 * @brief bits_reader_t
 */

struct bits_reader_t
{
	uint32_t avail;
	uint32_t bits;
	void *   user;

	int ( *read )( void * );
};

typedef struct bits_reader_t bits_reader_t;

/**
 * @brief c_bits_reader_init
 */

static __inline bits_reader_t * c_bits_reader_init( bits_reader_t * rdr, int ( *read )( void * ), void * user )
{
	rdr->avail = 0;
	rdr->bits  = 0;
	rdr->user  = user;
	rdr->read  = read;

	return rdr;
}

/**
 * @brief c_bits_reader_read
 */

static __inline uint32_t c_bits_reader_read( bits_reader_t * rdr, uint32_t bits )
{
	uint32_t rv;

	if ( bits <= rdr->avail )
	{
		rdr->avail -= bits;

		rv = ( rdr->bits >> rdr->avail ) & ( ( 1 << bits ) - 1 );
	}
	else
	{
		rv = rdr->bits & ( ( 1 << rdr->avail ) - 1 );

		bits -= rdr->avail;

		for ( rdr->avail = 0; bits >= 8; bits -= 8 )
		{
			int c = rdr->read( rdr->user );

			if ( EOF == c )
			{
				return rv;
			}

			rv = ( rv << 8 ) | ( c & 0xff );
		}

		if ( bits > 0 )
		{
			int c = rdr->read( rdr->user );

			if ( EOF != c )
			{
				rdr->bits = c & 0xff;

				rdr->avail = 8 - bits;

				rv = ( rv << bits ) | ( rdr->bits >> rdr->avail );
			}
		}
	}

	return rv;
}

/**
 * @brief c_bits_reader_sync
 */

static __inline void c_bits_reader_sync( bits_reader_t * rdr )
{
	rdr->avail = 0;
}

/**
 * @brief c_bits_reader_is_eof
 */

static __inline int c_bits_reader_is_eof( bits_reader_t * rdr )
{
	if ( 0 == rdr->avail )
	{
		int c = rdr->read( rdr->user );

		if ( EOF == c )
		{
			return 1;
		}

		rdr->avail = 8;
		rdr->bits  = c & 0xff;
	}

	return 0;
}

#endif	/* _MACRO_PRIVATE_LIBRARY_COMMON_C_BITS_READER_H */
