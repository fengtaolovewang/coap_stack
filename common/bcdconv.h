/*
*******************************************************************************

    bcdconv.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_COMMON_BCDCONV_H
#define	_MACRO_PRIVATE_LIBRARY_COMMON_BCDCONV_H

#include <stdint.h>

/**
 * @brief bcd2val
 */

static __inline uint32_t bcd2val( uint32_t bcd )
{
	return ( ( bcd >> 28 ) & 0x0f ) * 10000000 +
	       ( ( bcd >> 24 ) & 0x0f ) * 1000000  +
	       ( ( bcd >> 20 ) & 0x0f ) * 100000   +
	       ( ( bcd >> 16 ) & 0x0f ) * 10000    +
	       ( ( bcd >> 12 ) & 0x0f ) * 1000     +
	       ( ( bcd >> 8  ) & 0x0f ) * 100      +
	       ( ( bcd >> 4  ) & 0x0f ) * 10       +
	       ( ( bcd >> 0  ) & 0x0f );
}

/**
 * @brief val2bcd
 */

static __inline uint32_t val2bcd( uint32_t val )
{
	return ( ( val / 10000000 ) % 10 ) << 28 |
	       ( ( val / 1000000  ) % 10 ) << 24 |
	       ( ( val / 100000   ) % 10 ) << 20 |
	       ( ( val / 10000    ) % 10 ) << 16 |
	       ( ( val / 1000     ) % 10 ) << 12 |
	       ( ( val / 100      ) % 10 ) << 8  |
	       ( ( val / 10       ) % 10 ) << 4  |
	       ( ( val / 1        ) % 10 );
}

#endif	/* _MACRO_PRIVATE_LIBRARY_COMMON_BCDCONV_H */
