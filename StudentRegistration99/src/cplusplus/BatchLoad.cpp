
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

#ifndef stdio_h
#define stdio_h
#include <stdio.h>
#endif

#ifndef userdb_h
#define userdb_h
#include "userdb.h"
#endif

int GetField( char *String, int Length )
{
	int Position;

	for( Position = 0; Position < Length && String[ Position ] != '\t'; Position++ )
	{};

	if( Position == Length )
		return Length - 1;

	return Position;
}

int main( int argc, char **argv )
{
	DBM *DstFile;
	datum key, data;
	struct DBEntry anEntry;
	char InputBuffer[ 1024 ];

	cerr << "I'm running.\n";

	if( argc != 2 )
	{
		cerr << "Incorrect number of parameters.\n";
		exit( 1 );
	}

	cerr << "Correct number of args found.\n";


	if(( DstFile = dbm_open( argv[ 1 ], O_CREAT | O_WRONLY, 0600 )) == NULL )
	{
		cerr << "Failed to open the database.\n";
		exit( 1 );
	}

	cerr << "Database open.\n";

	while( cin.getline( InputBuffer, 1024 ) )
	{
		char Key[ 16 ];
		int LastPos, StartPos;

		memset( Key, 0, 16 );
		memset( anEntry.RealUserName, 0, 40 );
		memset( anEntry.Department, 0, 4 );
		memset( anEntry.ContactNumber, 0, 10 );
		memset( anEntry.PasswordKey, 0, 16 );
		anEntry.RDWRLevel = 0;
		anEntry.ViewLevel = 0;

		data.dptr = (char *)&anEntry;
		data.dsize = sizeof( struct DBEntry );

		StartPos = 0;
		LastPos = GetField( InputBuffer, cin.gcount());
		memcpy( Key, InputBuffer, LastPos );
		key.dptr = Key;
		key.dsize = 8;

		StartPos += LastPos + 1;
		LastPos = GetField( &InputBuffer[ StartPos ], cin.gcount() - StartPos );
		memcpy( anEntry.RealUserName, &InputBuffer[ StartPos ], LastPos );

		StartPos += LastPos + 1;
		LastPos = GetField( &InputBuffer[ StartPos ], cin.gcount() - StartPos );
		memcpy( anEntry.Department, &InputBuffer[ StartPos ], LastPos );

		StartPos += LastPos + 1;
		LastPos = GetField( &InputBuffer[ StartPos ], cin.gcount() - StartPos );
		memcpy( anEntry.ContactNumber, &InputBuffer[ StartPos ], LastPos );

		StartPos += LastPos + 1;
		LastPos = GetField( &InputBuffer[ StartPos ], cin.gcount() - StartPos );
		memcpy( anEntry.PasswordKey, &InputBuffer[ StartPos ], LastPos );

		StartPos += LastPos + 1;
		LastPos = GetField( &InputBuffer[ StartPos ], cin.gcount() - StartPos );
		if( LastPos != 1 )
		{
			cerr << "Number length incorrect.\n";
		}
		anEntry.RDWRLevel = InputBuffer[ StartPos ] - '0';

		StartPos += LastPos + 1;
		LastPos = GetField( &InputBuffer[ StartPos ], cin.gcount() - StartPos );
		if( LastPos != 1 )
		{
			cerr << "Number length incorrect.\n";
		}
		anEntry.ViewLevel = InputBuffer[ StartPos ] - '0';

		if( dbm_store( DstFile, key, data, DBM_INSERT ) == 1 )
		{
			cerr << "Duplicate entry found.\n";
			exit( 1 );
		}

	}

	dbm_close( DstFile );

	return 0;
}

