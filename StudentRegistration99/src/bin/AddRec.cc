#include <System/string.h>
#include <System/stdio.h>

#include "local.h"
#include "Requests.h"

#include "Defs.h"

#define BuffSize 2048

#define NO 0
#define YES -1
 
char *RecordCopy;
char *RecordLocked;
 
int main ( int argc, char** argv )
{
	char RLocked[ 9 ], OutputBuffer[ 10 ];
	char InputRec[ BuffSize - 1], DBRec[ BuffSize ], Request[ BuffSize ], RecCopy[ BuffSize ];
	int EndOfInput, InputLoop, ReturnResult;

	EndOfInput = NO;

	RecordLocked = RLocked;
	memcpy( RLocked, NORECORD, 9 );

	RecordCopy = RecCopy;
	memset( RecCopy, 0, BuffSize );

	while( EndOfInput == NO )
	{
		memset( InputRec, 0, BuffSize + 1 );
		memset( DBRec, 0, BuffSize );
		memset( Request, 0, BuffSize );

		InputRec[ 0 ] = getchar();

		for( InputLoop = 1; InputLoop < 1024  && InputRec[ InputLoop - 1 ] != EOF; InputLoop++ )
			InputRec[ InputLoop ] = getchar();

		fprintf( stderr, "InputRec reads %s\n", InputRec );

		if( InputLoop != 1024 )
		{
			EndOfInput = YES;
			continue;
		}
		(void)getchar();

		memcpy( Request, InputRec, 8 );

		AddCheckDigit( Request );

		if( memcmp( InputRec, Request, 9 ) != 0 )
		{
			fprintf( stderr, "Incorrect student number.\n" );
			continue;
		}

		fprintf( stderr, "InputRec reads %s\n", InputRec );
		(void)memset( OutputBuffer, 0, 10 );
		(void)memcpy( OutputBuffer, RLocked, 9);
		fprintf( stderr, "Request %s, RLocked %s\n", Request, OutputBuffer );

		if(( ReturnResult = DBNew( Request, DBRec )) != 0 )
		{
			fprintf( stderr, "%i: Error occored while adding record.\n", ReturnResult );
			if( ReturnResult == 7 )
				fprintf( stderr, "Record found.\n" );
			continue;
		}

		fprintf( stderr, "InputRec reads %s\n", InputRec );

		memcpy( DBRec, InputRec, BuffSize );
		memcpy( DBRec, Request, 9 );

		if(( ReturnResult = DBStore( DBRec )) != 0 )
		{
			fprintf( stderr, "%i: SaveRec failed.\n", ReturnResult );
			continue;
		}
		else
			fprintf( stderr, "Record %s stored.\n", Request );

	}

}


