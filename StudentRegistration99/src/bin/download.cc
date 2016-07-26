#include <System/stdio.h>
#include <System/memory.h>
#include <System/unistd.h>
#include <System/sys/types.h>
#include <System/sys/uio.h>

#define BufferSize 1025

int main( int argc, char **argv )
{
	char Buffer[ BufferSize ];

	/* Initalise buffer with 0 char */
	memset( Buffer, 0, BufferSize );

	/* While there is still data in the file */
	while( read( 0, Buffer, BufferSize - 1 ) == BufferSize  - 1 )
	{
		/* Add a newline to the end of the buffer */
		Buffer[ BufferSize - 1 ] = '\n';

		/* Check this is a valid record */
		if ( Buffer[8] - '0' >= 0 && Buffer[8] - '0' <= '9' )
			/* Write the Buffer and newline to ouput file */
			write( 1, Buffer, BufferSize );
		else
			/* Bad Record Found */
			fprintf( stderr, "Bad Record format found.\n" );

		/* Clear buffer */
		memset( Buffer, 0, BufferSize);
	}
	return 0;
}

