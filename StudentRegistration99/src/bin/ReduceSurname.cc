#include <System/string.h>
#include <System/unistd.h>
#include <System/fcntl.h>
#include <System/stdio.h>

#include "local.h"
#include "Requests.h"

int main ( int, char** )
{
	char InRecords[ 48000 ], OutRecords[ 47000 ];
	int CharRead, ProcessLoop, fd;

	if((fd = open( SurDataFile, O_RDWR | O_CREAT, 0600 )) == -1 )
	{
		perror( "Failed to open destination file" );
		return 1;
	}

	while(( CharRead = read( 0, InRecords, 48000 )) == 48000 )
	{
		for( ProcessLoop = 0; ProcessLoop < 1000; ProcessLoop++ )
			memcpy( &OutRecords[ ProcessLoop * 47 ], &InRecords[ ProcessLoop * 48 ], 47 );
		write( fd, OutRecords, 47000 );
	}

	if( CharRead != -1 )
	{
		for( ProcessLoop = 0; ProcessLoop < CharRead / 48; ProcessLoop++ )
			memcpy( &OutRecords[ ProcessLoop * 47 ], &InRecords[ ProcessLoop * 48 ], 47 );
		write( fd, OutRecords, CharRead / 48 * 47 );
		close( fd );
		return 0;
	}
	return 1;
}

