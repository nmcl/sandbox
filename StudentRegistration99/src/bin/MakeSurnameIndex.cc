#include <System/memory.h>
#include <System/unistd.h>
#include <System/stdio.h>
#include <System/ndbm.h>
#include <System/fcntl.h>

#include "local.h"
#include "Requests.h"

int main ( int, char** )
{
	char RecordSpace[ 47000 ], LastPair[ 2 ];
	int ProcessLoop, CharRead, fd;
	long Position = 0;
	datum key, data;
	DBM *DBHandle;

	memset( LastPair, 0, 2 );
	memset( RecordSpace, 0, 47000 );

	if(( fd = open( SurDataFile, O_RDONLY, 0600 )) == -1 )
	{
		perror( "Failed to open the data file" );
		return 1;
	}

	if(( DBHandle = dbm_open( SurIndexFile, O_RDWR | O_CREAT, 0600 )) == NULL )
	{
		perror( "Failed to open the database file" );
		return 1;
	}

	key.dptr = LastPair;
	key.dsize = 2;
	data.dptr = (char *)&Position;
	data.dsize = sizeof( Position );

	while(( CharRead = read( fd, RecordSpace, 47000 )) == 47000 )
	for( ProcessLoop = 0; ProcessLoop < 1000; ProcessLoop++ )
	if( memcmp( LastPair, &RecordSpace[ ProcessLoop * 47 ], 2 ) != 0 )
	{
		memcpy( LastPair, &RecordSpace[ ProcessLoop * 47 ], 2 );

		if( dbm_store( DBHandle, key, data, DBM_INSERT ) == 1 )
		{
			fprintf( stderr, "Error: entry already in DB.\n" );
			return 1;
		}

		Position += 47;
	}
	else
		Position += 47;

	if( CharRead != -1 )
	for( ProcessLoop = 0; ProcessLoop < CharRead / 47; ProcessLoop++ )
	if( memcmp( LastPair, &RecordSpace[ ProcessLoop * 47 ], 2 ) != 0 )
	{
		memcpy( LastPair, &RecordSpace[ ProcessLoop * 47 ], 2 );

		if( dbm_store( DBHandle, key, data, DBM_INSERT ) == 1 )
		{
			fprintf( stderr, "Error: entry already in DB.\n" );
			return 1;
		}

		Position += 47;
        }
	else
		Position += 47;

	dbm_close( DBHandle );
	close( fd );

	return 0;
}

