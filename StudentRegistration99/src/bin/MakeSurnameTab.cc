#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include "Defs.h"

int main( int argc, char** argv )
{
	char RecordSpace[ 1024000 ], OutputSpace[ 48000 ];
	int Infd, Outfd, CharsRead, ProcessLoop;

	if( argc != 3 )
	{
		fprintf( stderr, "Incorrect number of parameters.\n" );
		fprintf( stderr, "Syntax:\n" );
		fprintf( stderr, "\t%s SourceFileName DestFilename\n\n\n", argv[ 0 ] );
		exit( 1 );
	}

	if( (Infd = open( argv[ 1 ], O_RDONLY )) == -1 )
	{
		perror( "Failed to open the source file" );
		exit( 1 );
	}

	if( (Outfd = open( argv[ 2 ], O_RDWR | O_CREAT, 0600 )) == -1 )
	{
		perror( "Failed to open the destination file" );
		exit( 1 );
	}

	memset( RecordSpace, 0, 1024000 );

	while( ( CharsRead = read( Infd, RecordSpace, 1024000 )) == 1024000 )
	{
		memset( OutputSpace, 0, 48000 );
		for( ProcessLoop = 0; ProcessLoop < sizeof( RecordSpace ) / 1024; ProcessLoop++ )
		{
			memcpy( &OutputSpace[ ProcessLoop * 48 + 38 ], &RecordSpace[ ProcessLoop * 1024 ], 9 );
			memcpy( &OutputSpace[ ProcessLoop * 48 ], &RecordSpace[ ProcessLoop * 1024 + 9 ], 38 );
			OutputSpace[ ProcessLoop * 48 + 47 ] = '\n';
		}

		if( write( Outfd, OutputSpace, 48000 ) != 48000 )
		{
			perror( "Failed to write all characters" );
			exit( 1 );
		};
	}

	if( CharsRead == -1 )
	{
		perror( "Failed to read from data file" );
		exit( 1 );
	}

	for( ProcessLoop = 0; ProcessLoop < ( CharsRead / 1024 ); ProcessLoop++ )
	{
		memcpy( &OutputSpace[ ProcessLoop * 48 + 38 ], &RecordSpace[ ProcessLoop * 1024 ], 9 );
		memcpy( &OutputSpace[ ProcessLoop * 48 ], &RecordSpace[ ProcessLoop * 1024 + 9 ], 38 );
		OutputSpace[ ProcessLoop * 48 + 47 ] = '\n';
	}

	if( write( Outfd, OutputSpace, CharsRead / 1024 * 48 ) != CharsRead / 1024 * 48 )
	{
		perror( "Failed to write all characters" );
		exit( 1 );
	}

	close( Infd );
	close( Outfd );

	return 0;
}

