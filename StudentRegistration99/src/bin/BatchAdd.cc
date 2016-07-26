#include <System/string.h>
#include <System/stdlib.h>
#include <System/stdio.h>

#include "local.h"
#include "Requests.h"

#include "Defs.h"

char *RecordCopy;
char *RecordLocked;

int main( int argc, char** argv )
{
	int BaseNumber, Distance, AddLoop, ReturnResult;
	char CopyOfRecord[ 2048 ], Request[ 2048 ], Record[ 2048 ];
	char RLocked[ 9 ];

	if( argc != 3  && argc != 2 )
	{
		fprintf( stderr, "incorrect number of parameters.\n" );
		exit( 1 );
	}

	RecordCopy = CopyOfRecord;
	RecordLocked = RLocked;

	memset( RecordCopy, 0, 2048 );
	memcpy( RLocked, NORECORD, 9 );

 	sscanf( argv[ 1 ], "%i", &BaseNumber );
	if( argc == 3 )
		sscanf( argv[ 2], "%i", &Distance );
	else
		Distance = 1;

	for( AddLoop = 0; AddLoop < Distance; AddLoop++ )
	{
		memset( Record, 0, 2048 );
		memset( Request, 0, 2048 );

		sprintf( Request, "%i", BaseNumber + AddLoop );
		AddCheckDigit( Request );

		fprintf( stderr, "Creating %s\n", Request );
		if(( ReturnResult = DBNew( Request, Record )) != 0 )
		{
			fprintf( stderr, "%i: Error occored while adding record.\n", ReturnResult );
			exit( 1 );
		}

		fprintf( stderr, "Closing %s\n", Request );

		if(( ReturnResult =  DBClose( Request )) != 0 )
		{
			fprintf( stderr, "%i: Failed to close the new record.\n", ReturnResult );
			exit( 1 );
		}

		fprintf( stderr, "Record locked is %s.\n", RecordLocked );
	}

	return 0;

}

