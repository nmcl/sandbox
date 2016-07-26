
#ifndef memory_h
#define memory_h
#include <memory.h>
#endif

#ifndef ndbm_h
#define ndbm_h
#include <ndbm.h>
#endif

#ifndef fcntl_h
#define fcntl_h
#include <fcntl.h>
#endif

#ifndef iostream_h
#define iostream_h
#include <iostream.h>
#endif

#ifndef fstream_h
#define fstream_h
#include "fstream.h"
#endif

#ifndef userdb_h
#define userdb_h
#include "userdb.h"
#endif

int main( int argc, char **argv )
{
	DBM *SrcFile;
	datum key, data;
	struct DBEntry anEntry;

	cerr << "I'm running.\n";

	if( argc != 2 )
	{
		cerr << "Incorrect number of parameters.\n";
		exit( 1 );
	}

	cerr << "Correct number of args found.\n";

	if(( SrcFile = dbm_open( argv[ 1 ], O_RDONLY, 0600 )) == NULL )
	{
		cerr << "Failed to open the database.\n";
		exit( 1 );
	}

	cerr << "Database open.\n";

	for( key = dbm_firstkey( SrcFile ); key.dptr != NULL; key = dbm_nextkey( SrcFile ))
	{
		char OutputBuffer[ 256 ];

		memset( OutputBuffer, 0, 256 );
		memcpy( OutputBuffer, key.dptr, key.dsize );
		cout << OutputBuffer << "\t";

		data = dbm_fetch( SrcFile, key );

		memcpy( &anEntry, data.dptr, sizeof( struct DBEntry ));

		memset( OutputBuffer, 0, 256 );
		memcpy( OutputBuffer, anEntry.RealUserName, 40 );
		cout << OutputBuffer << "\t";

		memset( OutputBuffer, 0, 256 );
		memcpy( OutputBuffer, anEntry.Department, 4  );
		cout << OutputBuffer << "\t";

		memset( OutputBuffer, 0, 256 );
		memcpy( OutputBuffer, anEntry.ContactNumber, 10 );
		cout << OutputBuffer << "\t";

		memset( OutputBuffer, 0, 256 );
		memcpy( OutputBuffer, anEntry.PasswordKey, 16 );
		cout << OutputBuffer << "\t";

		cout << anEntry.RDWRLevel << "\t" << anEntry.ViewLevel << "\n";
	}

	cerr << "Dump finished.\n";

	dbm_close( SrcFile );
	return 0;
}

