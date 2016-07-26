#include <System/string.h>
#include <System/memory.h>
#include <System/stdlib.h>
#include <System/stdio.h>
#include <System/ndbm.h>
#include <System/fcntl.h>
#include <System/sys/file.h>
#include <System/sys/types.h>
#include <System/sys/socket.h>
#include <System/unistd.h>

extern "C"
{
  long tell (int);
}

#include "local.h"
#include "Requests.h"

#ifndef DEFS_H_
#  include "Defs.h"
#endif

#define BASEKEY "000000000"

/*
 * This program *does not* work with the Arjuna registration service.
 * Use RecordLoader instead.
 */

int main ( int argc, char** argv )
{
	DBM *DBfd;
	datum key, data;
	int IndexLockNumber, DataLockNumber, fd;
	char Data[ RECSIZE ], OutputString[ 70 ], *BaseKey = BASEKEY;
	struct IndexDBEntry Entry;

	if( argc != 2 )
	{
		fprintf( stderr, "Syntax error: I require 1 parameter.\n" );
		fprintf( stderr, "Syntax:\n\t %s NewRecordBase\n\n" );
		exit( 1 );
	};

	if( strlen( argv[ 1 ] ) != 6 )
	{
		fprintf( stderr, "Error: Parameter is incorrect.\n" );
		fprintf( stderr, "The parameter should be a 6 digit number (include all leading zeros)." );
		fprintf( stderr, "The paramters expected are the 3rd to 8th (inclusive) digits of the first\nunalloacted student number.  The year of any new student number is the current\nyear and the check digit is automatically calc'd\n\n" );
		exit( 1 );
	};

	memset( Data, 0, RECSIZE );
	memset( OutputString, 0, 70 );
	IndexLockNumber = GetLock( IndexFileLock, F_WRLCK );
	DataLockNumber = GetLock( DataFileLock, F_WRLCK );

	if(( DBfd = dbm_open( IndexFile, O_RDWR | O_CREAT, 0600 )) == NULL )
	{
		perror( "Failed to open database" );
		ReleaseLock( IndexLockNumber );
		ReleaseLock( DataLockNumber );
		exit( 1 );
	}

	if(( fd = open( DataFile, O_RDONLY, 0600 )) == -1 )
	{
		perror( "Failed to open the datafile" );
		ReleaseLock( IndexLockNumber );
		ReleaseLock( DataLockNumber );
		exit( 1 );
	}

	key.dptr = Data;
	key.dsize = 9;

	data.dptr = (char *)&Entry;
	data.dsize = sizeof( Entry );

	Entry.Status = Entry.Modified = 0;

	Entry.Position = tell( fd );
	while( read( fd, Data, RECSIZE ) == RECSIZE )
	{
		if( dbm_store( DBfd, key, data, DBM_INSERT ) == 1 )
		{
		  (void)memset( OutputString, 0, 70 );
		  (void)memcpy( OutputString, Data, 9 );
		  printf( "Error adding record %s - record already exists.\n", OutputString );
		}  
		Entry.Position = tell( fd );
	}

	key.dptr = BaseKey;
	key.dsize = 9;

	Entry.Status = atoi( argv[ 1 ] );

	if( dbm_store( DBfd, key, data, DBM_INSERT ) == 1 )
	{
		perror( "Failed to add the base record" );
		dbm_close( DBfd );
		close( fd );
		ReleaseLock( IndexLockNumber );
		ReleaseLock( DataLockNumber );
		return 1;
	}

	dbm_close( DBfd );
	close( fd );
	ReleaseLock( IndexLockNumber );
	ReleaseLock( DataLockNumber );

	return 0;
}

