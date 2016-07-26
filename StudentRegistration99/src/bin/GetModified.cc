#include <System/unistd.h>
#include <System/stdlib.h>
#include <System/string.h>
#include <System/ndbm.h>
#include <System/stdio.h>
#include <System/sys/file.h>
#include <System/sys/types.h>
#include <System/sys/stat.h>
#include <System/fcntl.h>

extern "C"
{
  long tell (int);
}

#include "local.h"
#include "Requests.h"

#include "Defs.h"

/*
 * This program *does not* work with the Arjuna registration service.
 * Use RecordInspector instead.
 */

int main( int argc, char** argv )
{
	struct IndexDBEntry anEntry;
	DBM *dbFile;
	FILE *theDataFile;
	int DBLock, DataLock, fd, InputLoop;
	datum key, data;
	char aDataRecord[ 1024 ];

	if( argc != 2 )
	{
		fprintf( stderr, "Incorrect number of parameters.\n" );
		exit( 1 );
	}

	if(( fd = open( argv[ 1 ], O_CREAT | O_RDWR, 0600 )) == -1 )
	{
		perror( "Failed to open output file" );
		exit( 1 );
	}

	if(( DBLock = GetLock( IndexFileLock, F_RDLCK )) == -1 )
	{
		perror( "Failed to get lock on database" );
		close( fd );
		unlink( argv[ 1 ] );
		exit( 1 );
	}

	if(( DataLock = GetLock( DataFileLock, F_RDLCK )) == -1 )
	{
		perror( "Failed to get lock on datafile" );
		ReleaseLock( DBLock );
		close( fd );
		unlink( argv[ 1 ] );
		exit( 1 );
	}

	if(( dbFile = dbm_open( IndexFile, O_RDWR, 0600 )) == NULL )
	{
		perror( "Failed to open the database" );
		ReleaseLock( DBLock );
		ReleaseLock( DataLock );
		close( fd );
		unlink( argv[ 1 ] );
		exit( 1 );
	}

	if(( theDataFile = fopen( DataFile, "r" )) == NULL )
	{
		perror( "Failed to open the datafile" );
		dbm_close( dbFile );
		ReleaseLock( DBLock );
		ReleaseLock( DataLock );
		close( fd );
		unlink( argv[ 1 ] );
		exit( 1 );
	}

	for( key = dbm_firstkey( dbFile ); key.dptr != NULL; key = dbm_nextkey( dbFile ))
	{
		data = dbm_fetch( dbFile, key );
		if( data.dptr == NULL )
			perror( "Failed to lookup entry" );
		else
		{
			memcpy( &anEntry, data.dptr, sizeof( anEntry ));
			if( anEntry.Modified == 0 )
				continue;
			else
			{
				if( fseek( theDataFile, anEntry.Position, 0 ) == -1 )
				{
					perror( "Failed to locate record" );
					dbm_close( dbFile );
					ReleaseLock( DBLock );
					fclose( theDataFile );
					ReleaseLock( DataLock );
					close( fd );
					unlink( argv[ 1 ] );
					exit( 1 );
				}
				else
				{
					memset( aDataRecord, 0, 1024 );
					for( InputLoop = 0; InputLoop < 1024; InputLoop++ )
						aDataRecord[ InputLoop ] = fgetc( theDataFile );
					write( fd, aDataRecord, 1024 );
					write( fd, "\n", 1 );
					write( 1, aDataRecord, 9 );
					write( 1, "\n", 1 );
				}
			}
		}
	}

	dbm_close( dbFile );
	ReleaseLock( DBLock );
	fclose( theDataFile );
	ReleaseLock( DataLock );

	if( tell(fd) == 0 )
		unlink( argv[ 1 ] );

	close( fd );

	return 0;
}


