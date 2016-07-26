#include <System/string.h>
#include <System/stdio.h>
#include <System/sys/time.h>
#include <System/signal.h>
#include <System/unistd.h>
#include <System/stdlib.h>

#include "local.h"
#include "Requests.h"
#include "des-utils.h"

#ifndef DEFS_H_
#  include "Defs.h"
#endif

#include <iostream.h>

extern char *Username;

void alarmTime(int dummy DOTDOTDOT)
{
	reportf( "Auth failed: too slow\n" );
	exit( 1 );
}

void WriteSuccess( int connection )
{
	char Reply[ 20 ];

	memset( Reply, 0, 20 );
	CreateToken( Reply );
	memcpy( &Reply[ 16 ], LOGINSUCC, strlen( LOGINSUCC ));
	memcpy( &Reply[ 16 + strlen( LOGINSUCC ) ], EOLN, 2 );

	write( connection, Reply, 16 + strlen( LOGINSUCC ) + 2 );
}

void WriteFail( int connection )
{
	char Reply[ 20 ];

	memset( Reply, 0, 20 );
	CreateToken( Reply );
	memcpy( &Reply[ 16 ], LOGINFAIL, strlen( LOGINFAIL ));
	memcpy( &Reply[ 16 + strlen( LOGINFAIL ) ], EOLN, 2);

	write( connection, Reply, 16 + strlen( LOGINFAIL ) + 2 );
}

int seclogin( int connection, char* UserID )
{
	char Request[ 256 ], Reply[ MAXBUFF ], UserKeyHex[ 16 ], UserKeyBit[ 8 ];
	char CorrectReplyBit[ 8 ], CorrectReplyHex[ 16 ], CorrectReplyEncodeBit[ 8 ], CorrectReplyEncodeHex[ 16 ];
	char OutputString[ 256 ];
	struct timeval SendTime, ReceiveTime;
	int ResReturn, outputdump;

	memset( Request, 0, 256 );
	InitSKUtils();

	CreateToken( Request );
	memcpy( &Request[ 16 ], LOGIN, strlen( LOGIN ));
	memcpy( &Request[ 16 + strlen( LOGIN )], LOGINACTION, strlen( LOGINACTION ));
	memcpy( &Request[ 16 + strlen( LOGIN ) + strlen( LOGINACTION )], EOLN, strlen( EOLN ));
	
	if( signal( SIGALRM, alarmTime ) == SIG_ERR)
        {
                perror( "Failed to set alarm signal");
                exit( 1 );
        }
	write( connection, Request, strlen( Request ) );

	(void) alarm( MAXAUTHTIME );

	memset( Reply, 0, MAXBUFF );
	if( gettimeofday( &SendTime, NULL ) == -1 )
	{
		perror( "Failed in gettimeofday" );
		exit( 1 );
	};

	GetLine( connection, Reply, MAXBUFF );

	(void) alarm( 0 );
	if( signal( SIGALRM, SIG_DFL ) == SIG_ERR)
	    perror( "Failed to reset the alarm event call" );

	if( gettimeofday( &ReceiveTime, NULL ) == -1 )
	{
		perror( "Failed in gettimeofday" );
		exit( 1 );
	};

	if( ReceiveTime.tv_sec - SendTime.tv_sec > MAXAUTHTIME )
	{
		WriteFail( connection );
		return 1;
	}

	(void)memset( UserKeyBit, 0, 8 );
	(void)memset( UserKeyHex, 0, 16 );
	(void)memset( UserID, 0, 9 );

	char* eolnptr = strstr(&Reply[16], EOLN);
	
	(void)memcpy( UserID, &Reply[ 16 ], eolnptr - &Reply[16]);

	if( (ResReturn = ResolveKey( UserID, UserKeyHex )) != 0 )
	{
		(void)memset( OutputString, 0, 256 );
		(void)memcpy( OutputString, UserID, strlen( UserID ));
		reportf( "Resolve returned %s%i.\n", "", ResReturn );
		reportf( "Failed to resolve ID of %s.\n", OutputString );

		WriteFail( connection );
		return 2;
	}

	memcpy( Username, UserID, strlen( UserID ));

	desinit( HexStringToBytes( UserKeyHex, UserKeyBit, 8 ));

	memset( CorrectReplyBit, 0, 8 );
	memset( CorrectReplyHex, 0, 16 );
	memset( CorrectReplyEncodeHex, 0, 16 );
	memset( CorrectReplyEncodeBit, 0, 8 );

	memcpy( CorrectReplyHex, Request, 16);

	HexStringToBytes( CorrectReplyHex, CorrectReplyBit, 8 );
	endes( CorrectReplyBit, CorrectReplyEncodeBit );
	BytesToHexString( CorrectReplyEncodeBit, CorrectReplyEncodeHex, 8 );

#ifndef DEBUG
	if( memcmp( CorrectReplyEncodeHex, Reply, 16 ) != 0 )
		if( memcmp( CorrectReplyEncodeHex, Reply, 16 ) == 0 )
		{
			WriteFail( connection );
			return 3; /* Cancel */
		}
		else
		{
			memset( OutputString, 0, 256);
			memcpy( OutputString, CorrectReplyEncodeHex, 16 );
			reportf( "Encode hex string is %s\n", OutputString );
			WriteFail( connection );
			return 4;
		}
#endif

	WriteSuccess( connection );
	return -1;
}

