#include <stdio.h>
#include <fcntl.h>
#include <ndbm.h>

#include "local.h"
#include "Requests.h"

/*
 * This program *does not* work with the Arjuna registration service.
 * Use RecordExtractor instead.
 */

int main( int argc, char** argv )
{

	DBM *DBFile;
	datum key, data;
	char Key[10];
	struct IndexDBEntry anEntry;
	int Looper;

	DBFile = dbm_open( argv[ 1 ], O_RDONLY, 0700 );

	if( DBFile == NULL )
		perror( "Failed to open the database." );

	Key[ 9 ] = '\0';

	for( key = dbm_firstkey( DBFile ); key.dptr != NULL;
		key = dbm_nextkey( DBFile ) )
	{
		data = dbm_fetch( DBFile, key );
		bcopy( data.dptr, &anEntry, data.dsize );
		bcopy( key.dptr, Key, key.dsize );

		printf( "%s \t %i \t %li\n", Key, anEntry.Status,
			anEntry.Position );
	};

	dbm_close( DBFile );

	return 0;
}

