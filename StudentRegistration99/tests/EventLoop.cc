#include <System/string.h>
#include <System/memory.h>
#include <System/stdio.h>
#include <System/unistd.h>
#include <System/stdlib.h>

#include <local.h>
#include <Defs.h>

extern const char* Username;

void Login( char* Input, char* Output )
{
    char KeyHex[ 16 ], KeyBit[ 8 ], InputHex[ 16 ], InputBit[ 8 ];
    char OutputBit[ 8 ], OutputHex[ 16 ], InputString[ 40 ];

    memset( KeyHex, 0, 16 );
    memset( KeyBit, 0, 8 );

    memset( InputHex, 0, 16 );
    memset( InputBit, 0, 8 );
    memset( OutputBit, 0, 8 );
    memset( OutputHex, 0, 16 );

    memset( InputString, 0, 40 );

    scanf( "%s", InputString );

    memcpy(KeyHex, InputString, 16);
    HexStringToBytes( KeyHex, KeyBit, 8 );
    desinit(  KeyBit );

    memcpy(InputHex, Input, 16);
    endes( HexStringToBytes( InputHex, InputBit, 8 ), OutputBit );
    BytesToHexString( OutputBit, OutputHex, 8 );

    memcpy( Output, OutputHex, 16 );
    memcpy( &Output[ 16 ], Username, ::strlen(Username) );
    memcpy( memchr( Output, '\0', MAXBUFF ), EOLN, strlen(EOLN));
}

void VersionMessage( char* Output )
{
    memcpy( &Output[ 16 ], "VN12345000000000", 16 );
    scanf( "%s", &Output[ 32 ] );
}

void DoLogin( int Handle )
{
	char InputLine[ MAXBUFF ], ReplyLine[ MAXBUFF ];

	for(;;)
	{
		memset( InputLine, 0, MAXBUFF );
		memset( ReplyLine, 0, MAXBUFF );

		/* Receive( Handle, InputLine ); */
		GetLine(Handle, InputLine, MAXBUFF);
		fprintf(stderr, "Line = %s\n", InputLine);
		if( InputLine[ 0 ] == '\0' )
			break;;
		if( strncmp( &InputLine[ 16 ], "LN", 2 ) == 0 )
			Login( InputLine, ReplyLine );
		else if( strncmp( &InputLine[ 16 ], "LS", 2 ) == 0 )
			VersionMessage( ReplyLine );
		else if( strncmp( &InputLine[ 16 ], "VS", 2 ) == 0 )
			break;
		else
		{
			int OutputLoop;

			fprintf( stderr, "Unexpected command string, I received the string::\n" );
			for( OutputLoop = 0; OutputLoop < strlen( InputLine ); OutputLoop++ )
				fprintf( stderr, "%i\t%c\n", InputLine[ OutputLoop ], InputLine[ OutputLoop ] );
			close( Handle );

			exit( 1 );
		}

		/* Send( Handle, ReplyLine );*/
		write(Handle, ReplyLine, strlen(ReplyLine));
	}
}

