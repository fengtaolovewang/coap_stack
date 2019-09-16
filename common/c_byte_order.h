/*
*******************************************************************************

    c_byte_order.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_COMMON_C_BYTE_ORDER_H
#define	_MACRO_PRIVATE_LIBRARY_COMMON_C_BYTE_ORDER_H

#include <stdint.h>

#if ( ! defined( __LITTLE_ENDIAN__ ) ) && ( ! defined( __BIG_ENDIAN__ ) ) && ( ! defined( __PDP_ENDIAN__ ) )
#if defined( _MSC_VER )
#if 0x41424344UL == ( 'ABCD' )
#ifndef __LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__ 1
#endif
#elif 0x44434241UL == ( 'ABCD' )
#ifndef __BIG_ENDIAN__
#define __BIG_ENDIAN__ 1
#endif
#elif 0x42414443UL == ( 'ABCD' )
#ifndef __PDP_ENDIAN__
#define __PDP_ENDIAN__ 1
#endif
#endif
#elif defined( __GNUC__ ) && defined( __BYTE_ORDER__ )
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#ifndef __LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__ 1
#endif
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#ifndef __BIG_ENDIAN__
#define __BIG_ENDIAN__ 1
#endif
#elif __BYTE_ORDER__ == __ORDER_PDP_ENDIAN__
#ifndef __PDP_ENDIAN__
#define __PDP_ENDIAN__ 1
#endif
#endif
#elif defined( __GNUC__ )
#if 0x41424344UL == ( 'ABCD' )
#ifndef __LITTLE_ENDIAN__
#define __LITTLE_ENDIAN__ 1
#endif
#elif 0x44434241UL == ( 'ABCD' )
#ifndef __BIG_ENDIAN__
#define __BIG_ENDIAN__ 1
#endif
#elif 0x42414443UL == ( 'ABCD' )
#ifndef __PDP_ENDIAN__
#define __PDP_ENDIAN__ 1
#endif
#endif
#endif
#endif

#if ( ! defined( __LITTLE_ENDIAN__ ) ) && ( ! defined( __BIG_ENDIAN__ ) ) && ( ! defined( __PDP_ENDIAN__ ) )
#error Does not define the compiler byte order
#endif

#define __swab16__( x ) \
( uint16_t )( \
( ( ( uint16_t )( x ) & ( uint16_t )0x00ffU ) << 8 ) | \
( ( ( uint16_t )( x ) & ( uint16_t )0xff00U ) >> 8 ) )

#define __swab32__( x ) \
( uint32_t )( \
( ( ( uint32_t )( x ) & ( uint32_t )0x000000ffUL ) << 24 ) | \
( ( ( uint32_t )( x ) & ( uint32_t )0x0000ff00UL ) <<  8 ) | \
( ( ( uint32_t )( x ) & ( uint32_t )0x00ff0000UL ) >>  8 ) | \
( ( ( uint32_t )( x ) & ( uint32_t )0xff000000UL ) >> 24 ) )

#define __swab64__( x ) \
( uint64_t )( \
( uint64_t )( ( ( uint64_t )( x ) & ( uint64_t )0x00000000000000ffULL ) << 56 ) | \
( uint64_t )( ( ( uint64_t )( x ) & ( uint64_t )0x000000000000ff00ULL ) << 40 ) | \
( uint64_t )( ( ( uint64_t )( x ) & ( uint64_t )0x0000000000ff0000ULL ) << 24 ) | \
( uint64_t )( ( ( uint64_t )( x ) & ( uint64_t )0x00000000ff000000ULL ) <<  8 ) | \
( uint64_t )( ( ( uint64_t )( x ) & ( uint64_t )0x000000ff00000000ULL ) >>  8 ) | \
( uint64_t )( ( ( uint64_t )( x ) & ( uint64_t )0x0000ff0000000000ULL ) >> 24 ) | \
( uint64_t )( ( ( uint64_t )( x ) & ( uint64_t )0x00ff000000000000ULL ) >> 40 ) | \
( uint64_t )( ( ( uint64_t )( x ) & ( uint64_t )0xff00000000000000ULL ) >> 56 ) )

/**
 * @brief be16_to_cpu
 */

static __inline uint16_t be16_to_cpu( uint16_t be16 )
{
	#if __LITTLE_ENDIAN__

	return __swab16__( be16 );

	#elif __PDP_ENDIAN__

	#error be16_to_cpu does not support

	#else

	return be16;

	#endif
}

/**
 * @brief be32_to_cpu
 */

static __inline uint32_t be32_to_cpu( uint32_t be32 )
{
	#if __LITTLE_ENDIAN__

	return __swab32__( be32 );

	#elif __PDP_ENDIAN__

	#error be32_to_cpu does not support

	#else

	return be32;

	#endif
}

/**
 * @brief be64_to_cpu
 */

static __inline uint64_t be64_to_cpu( uint64_t be64 )
{
	#if __LITTLE_ENDIAN__

	return __swab64__( be64 );

	#elif __PDP_ENDIAN__

	#error be64_to_cpu does not support

	#else

	return be64;

	#endif
}

/**
 * @brief cpu_to_be16
 */

static __inline uint16_t cpu_to_be16( uint16_t cpu )
{
	#if __LITTLE_ENDIAN__

	return __swab16__( cpu );

	#elif __PDP_ENDIAN__

	#error cpu_to_be16 does not support

	#else

	return cpu;

	#endif
}

/**
 * @brief cpu_to_be32
 */

static __inline uint32_t cpu_to_be32( uint32_t cpu )
{
	#if __LITTLE_ENDIAN__

	return __swab32__( cpu );

	#elif __PDP_ENDIAN__

	#error cpu_to_be32 does not support

	#else

	return cpu;

	#endif
}

/**
 * @brief cpu_to_be64
 */

static __inline uint64_t cpu_to_be64( uint64_t cpu )
{
	#if __LITTLE_ENDIAN__

	return __swab64__( cpu );

	#elif __PDP_ENDIAN__

	#error cpu_to_be64 does not support

	#else

	return cpu;

	#endif
}

/**
 * @brief cpu_to_le16
 */

static __inline uint16_t cpu_to_le16( uint16_t cpu )
{
	#if __BIG_ENDIAN__

	return __swab16__( cpu );

	#elif __PDP_ENDIAN__

	#error cpu_to_le16 does not support

	#else

	return cpu;

	#endif
}

/**
 * @brief cpu_to_le32
 */

static __inline uint32_t cpu_to_le32( uint32_t cpu )
{
	#if __BIG_ENDIAN__

	return __swab32__( cpu );

	#elif __PDP_ENDIAN__

	#error cpu_to_le32 does not support

	#else

	return cpu;

	#endif
}

/**
 * @brief cpu_to_le64
 */

static __inline uint64_t cpu_to_le64( uint64_t cpu )
{
	#if __BIG_ENDIAN__

	return __swab64__( cpu );

	#elif __PDP_ENDIAN__

	#error cpu_to_le64 does not support

	#else

	return cpu;

	#endif
}

/**
 * @brief le16_to_cpu
 */

static __inline uint16_t le16_to_cpu( uint16_t le16 )
{
	#if __BIG_ENDIAN__

	return __swab16__( le16 );

	#elif __PDP_ENDIAN__

	#error le16_to_cpu does not support

	#else

	return le16;

	#endif
}

/**
 * @brief le32_to_cpu
 */

static __inline uint32_t le32_to_cpu( uint32_t le32 )
{
	#if __BIG_ENDIAN__

	return __swab32__( le32 );

	#elif __PDP_ENDIAN__

	#error le32_to_cpu does not support

	#else

	return le32;

	#endif
}

/**
 * @brief le64_to_cpu
 */

static __inline uint64_t le64_to_cpu( uint64_t le64 )
{
	#if __BIG_ENDIAN__

	return __swab64__( le64 );

	#elif __PDP_ENDIAN__

	#error le64_to_cpu does not support

	#else

	return le64;

	#endif
}

static __inline uint16_t be16_to_le16( uint16_t be16 ){ return __swab16__( be16 ); }
static __inline uint32_t be32_to_le32( uint32_t be32 ){ return __swab32__( be32 ); }
static __inline uint64_t be64_to_le64( uint64_t be64 ){ return __swab64__( be64 ); }

static __inline uint16_t le16_to_be16( uint16_t le16 ){ return __swab16__( le16 ); }
static __inline uint32_t le32_to_be32( uint32_t le32 ){ return __swab32__( le32 ); }
static __inline uint64_t le64_to_be64( uint64_t le64 ){ return __swab64__( le64 ); }

#endif	/* _MACRO_PRIVATE_LIBRARY_COMMON_C_BYTE_ORDER_H */
