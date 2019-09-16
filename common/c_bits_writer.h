/*
*******************************************************************************

    c_bits_writer.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_COMMON_C_BITS_WRITER_H
#define	_MACRO_PRIVATE_LIBRARY_COMMON_C_BITS_WRITER_H

#include <stdio.h>
#include <stdint.h>

/**
 * @brief bits_writer_t
 */

struct bits_writer_t
{
	uint32_t avail;
	uint32_t bits;
	void *   user;

	int ( *write )( void *, int );
};

typedef struct bits_writer_t bits_writer_t;

/**
 * @brief c_bits_writer_init
 */

static __inline bits_writer_t * c_bits_writer_init( bits_writer_t * wrr, int ( *write )( void *, int ), void * user )
{
	wrr->avail = 0;
	wrr->bits  = 0;
	wrr->user  = user;
	wrr->write = write;

	return wrr;
}

/**
 * @brief c_bits_writer_write
 */

static __inline int c_bits_writer_write( bits_writer_t * wrr, uint32_t bits, uint32_t value )
{
	if ( 0 == bits || bits > 32 )
	{
		return -1;
	}
	else if ( bits < 32 )
	{
		value &= ( ( 1 << bits ) - 1 );
	}

	if ( bits < wrr->avail )
	{
		wrr->avail -= bits;

		wrr->bits |= ( value << wrr->avail );
	}
	else
	{
		bits -= wrr->avail;

		wrr->bits &= ~( ( 1 << wrr->avail ) - 1 );

		wrr->bits |= ( value >> bits ) & ( ( 1 << wrr->avail ) - 1 );

		if ( wrr->avail > 0 && EOF == wrr->write( wrr->user, wrr->bits & 0xff ) )
		{
			return 8;
		}

		for ( wrr->avail = 0; bits >= 8; bits -= 8 )
		{
			if ( EOF == wrr->write( wrr->user, ( value >> ( bits - 8 ) ) & 0xff ) )
			{
				return ( int )bits;
			}
		}

		if ( bits > 0 )
		{
			wrr->avail = 8 - bits;

			wrr->bits = ( ( value & ( ( 1 << bits ) - 1 ) ) << wrr->avail );
		}
	}

	return 0;
}

/**
 * @brief c_bits_writer_flush
 */

static __inline int c_bits_writer_flush( bits_writer_t * wrr )
{
	return ( wrr->avail > 0 && EOF == wrr->write( wrr->user, wrr->bits & 0xff ) ) ? ( EOF ) : ( 0 );
}

#endif	/* _MACRO_PRIVATE_LIBRARY_COMMON_C_BITS_WRITER_H */
