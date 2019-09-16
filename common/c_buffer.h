/*
*******************************************************************************

    c_buffer.h

    Programmer:		Hetao

    Remark:

*******************************************************************************
*/

#ifndef	_MACRO_PRIVATE_LIBRARY_COMMON_C_BUFFER_H
#define	_MACRO_PRIVATE_LIBRARY_COMMON_C_BUFFER_H

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>

/**
 * @brief buffer_head_t
 */

struct buffer_head_t
{
	char * bp;
	char * ep;
	char * rp;
	char * wp;
};

typedef struct buffer_head_t buffer_head_t;

/**
 * @brief c_buffer_head_init
 */

static __inline buffer_head_t * c_buffer_head_init( buffer_head_t * head, void * buf, size_t cap, size_t len )
{
	if ( len > cap ) len = cap;

	head->bp = ( char * )buf;
	head->ep = ( char * )buf + cap;
	head->rp = ( char * )buf;
	head->wp = ( char * )buf + len;

	return head;
}

/**
 * @brief c_buffer_read
 */

static __inline int c_buffer_read( buffer_head_t * head, void * buf, size_t len )
{
	if ( head->rp == head->wp )
	{
		return EOF;
	}
	else if ( head->rp + len > head->wp )
	{
		len = head->wp - head->rp;
	}

	memcpy( buf, head->rp, len );

	head->rp += len;

	return ( int )len;
}

/**
 * @brief c_buffer_write
 */

static __inline int c_buffer_write( buffer_head_t * head, const void * buf, size_t len )
{
	if ( head->rp == head->ep )
	{
		return EOF;
	}
	else if ( head->rp + len > head->ep )
	{
		len = head->ep - head->rp;
	}

	memcpy( head->rp, buf, len );

	head->rp += len;

	if ( head->rp > head->wp )
	{
		head->wp = head->rp;
	}

	return ( int )len;
}

/**
 * @brief c_buffer_seek
 */

static __inline intptr_t c_buffer_seek( buffer_head_t * head, intptr_t offset, int whence )
{
	if ( SEEK_SET == whence )
	{
		if ( offset >= 0 )
		{
			if ( head->bp + offset > head->wp )
			{
				head->rp = head->wp;
			}
			else
			{
				head->rp = head->bp + offset;
			}
		}
	}
	else if ( SEEK_CUR == whence )
	{
		if ( offset < 0 )
		{
			if ( head->rp + offset < head->bp )
			{
				head->rp = head->bp;
			}
			else
			{
				head->rp += offset;
			}
		}
		else
		{
			if ( head->rp + offset > head->wp )
			{
				head->rp = head->wp;
			}
			else
			{
				head->rp += offset;
			}
		}
	}
	else if ( SEEK_END == whence )
	{
		if ( offset <= 0 )
		{
			if ( head->wp + offset < head->bp )
			{
				head->rp = head->bp;
			}
			else
			{
				head->rp = head->wp + offset;
			}
		}
	}

	return head->rp - head->bp;
}

/**
 * @brief c_buffer_lseek
 */

static __inline intptr_t c_buffer_lseek( buffer_head_t * head, intptr_t offset, int whence )
{
	if ( SEEK_SET == whence )
	{
		if ( offset >= 0 )
		{
			if ( head->bp + offset > head->ep )
			{
				head->rp = head->ep;
			}
			else
			{
				head->rp = head->bp + offset;
			}
		}
	}
	else if ( SEEK_CUR == whence )
	{
		if ( offset < 0 )
		{
			if ( head->rp + offset < head->bp )
			{
				head->rp = head->bp;
			}
			else
			{
				head->rp += offset;
			}
		}
		else
		{
			if ( head->rp + offset > head->ep )
			{
				head->rp = head->ep;
			}
			else
			{
				head->rp += offset;
			}
		}
	}
	else if ( SEEK_END == whence )
	{
		if ( offset <= 0 )
		{
			if ( head->ep + offset < head->bp )
			{
				head->rp = head->bp;
			}
			else
			{
				head->rp = head->ep + offset;
			}
		}
	}

	if ( head->rp > head->wp )
	{
		head->wp = head->rp;
	}

	return head->rp - head->bp;
}

/**
 * @brief c_buffer_pread
 */

static __inline int c_buffer_pread( buffer_head_t * head, void * buf, size_t len, uintptr_t offset )
{
	char * rp = head->bp + offset;

	if ( rp >= head->wp )
	{
		return EOF;
	}
	else if ( rp + len > head->wp )
	{
		len = head->wp - rp;
	}

	memcpy( buf, rp, len );

	return ( int )len;
}

/**
 * @brief c_buffer_pwrite
 */

static __inline int c_buffer_pwrite( buffer_head_t * head, const void * buf, size_t len, uintptr_t offset )
{
	char * rp = head->bp + offset;

	if ( rp >= head->ep )
	{
		return EOF;
	}
	else if ( rp + len > head->ep )
	{
		len = head->ep - rp;
	}

	memcpy( rp, buf, len );

	rp += len;

	if ( rp > head->wp )
	{
		head->wp = rp;
	}

	return ( int )len;
}

/**
 * @brief c_buffer_peek
 */

static __inline int c_buffer_peek( buffer_head_t * head )
{
	return ( head->rp == head->wp ) ? ( EOF ) : ( ( unsigned char )( *head->rp ) );
}

/**
 * @brief c_buffer_getc
 */

static __inline int c_buffer_getc( buffer_head_t * head )
{
	return ( head->rp == head->wp ) ? ( EOF ) : ( ( unsigned char )( *head->rp++ ) );
}

/**
 * @brief c_buffer_putc
 */

static __inline int c_buffer_putc( buffer_head_t * head, int c )
{
	int result = EOF;

	if ( head->rp == head->ep )
	{
		return result;
	}

	result = ( unsigned char )( *head->rp++ = ( char )c );

	if ( head->rp > head->wp )
	{
		head->wp = head->rp;
	}

	return result;
}

/**
 * @brief c_buffer_puts
 */

static __inline int c_buffer_puts( buffer_head_t * head, const char * s )
{
	return c_buffer_write( head, s, strlen( s ) );
}

/**
 * @brief c_buffer_gets
 */

static __inline char * c_buffer_gets( buffer_head_t * head, char * buf, size_t len )
{
	char * s = buf;

	if ( head->rp == head->wp )
	{
		return NULL;
	}

	for ( ;; )
	{
		int c = c_buffer_getc( head );

		if ( EOF == c )
		{
			break;
		}
		else if ( '\r' == c )
		{
			if ( '\n' == c_buffer_peek( head ) )
			{
				c_buffer_getc( head );
			}

			break;
		}
		else if ( '\n' == c )
		{
			break;
		}
		else if ( len > 1 )
		{
			*s++ = ( char )c;

			--len;
		}
	}

	if ( len )
	{
		*s = '\0';
	}

	return buf;
}

/**
 * @brief c_buffer_vprintf
 */

static __inline int c_buffer_vprintf( buffer_head_t * head, const char * format, va_list arglst )
{
	int n;

	if ( head->rp == head->ep )
	{
		return EOF;
	}

	n = vsnprintf( head->rp, head->ep - head->rp, format, arglst );

	if ( n < 0 || n > head->ep - head->rp )
	{
		n = ( int )( head->ep - head->rp );

		head->rp = head->ep;
	}
	else
	{
		head->rp += n;
	}

	if ( head->rp > head->wp )
	{
		head->wp = head->rp;
	}

	return n;
}

/**
 * @brief c_buffer_printf
 */

static __inline int c_buffer_printf( buffer_head_t * head, const char * format, ... )
{
	int n;

	va_list arglst;

	va_start( arglst, format );

	n = c_buffer_vprintf( head, format, arglst );

	va_end( arglst );

	return n;
}

/**
 * @brief c_buffer_is_eof
 */

static __inline int c_buffer_is_eof( buffer_head_t * head )
{
	return head->rp == head->wp;
}

/**
 * @brief c_buffer_size
 */

static __inline size_t c_buffer_size( buffer_head_t * head )
{
	return head->wp - head->rp;
}

/**
 * @brief c_buffer_free_size
 */

static __inline size_t c_buffer_free_size( buffer_head_t * head )
{
	return head->ep - head->wp;
}

/**
 * @brief c_buffer_ungetc
 */

static __inline int c_buffer_ungetc( buffer_head_t * head, int c )
{
	if ( head->rp == head->bp )
	{
		return EOF;
	}

	return ( unsigned char )( *( --head->rp ) = ( char )c );
}

#endif	/* _MACRO_PRIVATE_LIBRARY_COMMON_C_BUFFER_H */
