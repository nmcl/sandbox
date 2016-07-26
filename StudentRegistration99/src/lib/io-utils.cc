#include <System/string.h>
#include <System/stdio.h>
#include <System/sys/time.h>
#include <System/unistd.h>
#include <System/sys/types.h>
#include <System/time.h>
#include <System/errno.h>
#include <System/stdlib.h>

#include "local.h"
#include "Requests.h"

#ifndef DEFS_H_
#  include "Defs.h"
#endif

static char Factor[] = {1, 3, 7, 9, 1, 3, 7, 9};

extern char *Username;

Boolean verbose = FALSE;
FILE *outstr;

void reportf (char* s, char* a, int b, int c, int d, int e, int f)
{
    if (verbose)
    {
	char UserString[ USERNAMELEN + 1 ], TimeInfo[80];
	char OutputPattern[ 1025 ];
	struct timeval tp;
	time_t t;

	memset( OutputPattern, '\0', 1025 );

	memset( UserString, 0, USERNAMELEN + 1 );
	memcpy( UserString, Username, USERNAMELEN );
	gettimeofday( &tp, NULL );

	t = (time_t) tp.tv_sec;
	strftime(TimeInfo, 80, "%d/%m/%y (%a) %H:%M:%S\0", localtime(&t));

	sprintf( OutputPattern, "%s %i %s : %s", TimeInfo, getpid(), UserString, s );
	OutputPattern[1024] = '\0';
	fprintf (outstr, OutputPattern, a, b, c, d, e, f);
	fflush(outstr);
    }
}

/* This function read a whole line until either the EOLn marker is reached or the buffer is filled */
void GetLine( int Connection, char* Line, int Length )
{
	int Loop;
	
	/* Reset the buffer */
	memset( Line, 0, Length );
			/* For each character in the buffer */
	for( Loop = 0; Loop < Length; Loop++ )
	{
	    /* Read one character from the given connection */
	    if( read( Connection, &Line[ Loop ], 1 ) == -1 )
	    {
		/* If the read fails then issue a message saying why and exit */
		perror( "Read failed on connection" );
		if( errno == ETIMEDOUT )
		    memset( Line, 0, Length );
		
		Loop = Length;
	    }
	    else  /* Check to see if we have reached the EOLn marker */
		if ( Loop > 1 && memcmp( &Line[ Loop - 1 ], EOLN, 2 ) == 0)
		    break;    /* If we have then exit the for loop */

	    if ((Loop < Length) && (Loop > 0))
	    {
		/* If the EOLn marker was reached then remove it */
		if( memcmp( &Line[ Loop - 1 ], EOLN, 2 ) == 0 )
		{
		    Line[ Loop ] = '\0';
		    Line[ Loop -1 ] = '\0';
		}
	    }
	}
}

/* This function creats a new token */
void CreateToken( char* TokenText )
{
        struct timeval tp;
	unsigned long TokenTime;
        char TimeBin[ 8 ];
 
			/* Reset the space for the new token */
        memset( TokenText, 0, 16 );
	memset( TimeBin, 0, 8 );
 
			/* Get the current time of day */
        if( gettimeofday( &tp, NULL ) == -1 )
        {
			/* If this fails the create a message why and then exit */
                perror( "Failed to get time of day" );
                exit( 1 );
        }

		/* Modify the time to the number of seconds since 1-1-1900 */
	TokenTime = tp.tv_sec + YEARS_70;
 
		/* Create the token */
        memcpy( TimeBin, &TokenTime, 4 );
        memcpy( &TimeBin[ 4 ], &( tp.tv_usec ), 4 );
        memcpy( &TimeBin[ 4 ], &( tp.tv_usec ), 2 );

		/* Convert this token into its hex form */
        BytesToHexString( TimeBin, TokenText, 8 );
}

/* This function checks to see if the given token is young */
int CheckToken( char* Token )
{
	struct timeval tp;
	struct timezone tz;
	unsigned long TokenTime;
	long Difference;

			/* Convert the hex string into a binary number */
	(void) HexStringToBytes( Token, (char*) &TokenTime, sizeof(TokenTime) );

			/* Get the current time */
	if( gettimeofday( &tp, &tz ) == -1 )
	{
		perror( "Failed in gettimeofday" );
		return -1;
	};

			/* Calculate the difference between now and token time */
	Difference = tp.tv_sec + YEARS_70 - TokenTime;

			/* Validate that the difference is within the valid bounds */

#ifndef DEBUG
	if( Difference > -LOWERLIMIT && Difference < UPPERLIMIT )
		return 0; 
	else
		return -1;
#else
	return 0;
#endif
}

/* This function takes a line of plain text with a token prefix and sends it to the client */
void Send( int Connection, char* Line )
{
	char SendLineEncode[ MAXBUFF ], GivenTime[ 9 ], Null[ 8 ];

	memset( Null, 0, 8 );
	memset( GivenTime, 0, 9 );
	memcpy( GivenTime, Line, 8 );

	CreateToken( Line );

	if( GivenTime[ 0 ] != '\0' )
		memcpy( Line, GivenTime, 8 );

#ifdef VERY_VERBOSE	
	reportf( "S %s : %i\n", Line, strlen( Line ));
#endif	

	memset( SendLineEncode, 0, MAXBUFF );
#ifndef DEBUG
	EncodeBlock( Line, SendLineEncode, strlen( Line ));
#else
	memcpy( SendLineEncode, Line, strlen( Line ));
#endif

#ifdef VERY_VERBOSE	
	reportf( "S %s : %i\n", SendLineEncode, strlen( SendLineEncode ));
#endif	

	memcpy( &SendLineEncode[ strlen( SendLineEncode ) ], EOLN, 2 );

	write( Connection, SendLineEncode, strlen( SendLineEncode ));
}

/* This function receives a line of plain text and decodes it */
void Receive( int Connection, char* Line )
{
	char ReceivedLineEncode[ MAXBUFF ];

	memset( ReceivedLineEncode, 0, MAXBUFF );
	GetLine( Connection, ReceivedLineEncode, MAXBUFF );
	if( ReceivedLineEncode[ 0 ] == '\0' )
		return;

#ifdef VERY_VERBOSE	
	reportf( "R %s : %i\n", ReceivedLineEncode, strlen( ReceivedLineEncode ));
#endif	

#ifndef DEBUG
	DecodeBlock( ReceivedLineEncode, Line, strlen( ReceivedLineEncode ));
#else
	memcpy( Line, ReceivedLineEncode, strlen( ReceivedLineEncode ));
#endif

#ifdef VERY_VERBOSE	
	reportf( "R %s : %i\n", Line, strlen( Line ));
#endif	
}

void AddCheckDigit( char* StudentNumber )
{
	int PositionLoop, Total = 0;
 
        for (PositionLoop = 0; PositionLoop < 8; PositionLoop++)
                Total += ( StudentNumber[ PositionLoop ] - '0') * Factor[ PositionLoop ];
        Total %= 10;
        Total = 10 - Total;
        Total %= 10;
        StudentNumber[ 8 ] = Total + '0';
}

void CalcCheck( int StudentNumber, char* Result )
{
	struct timeval TimeNow_Sec;
	char Year[ 3 ], StudNumber[ 7 ], *Zero = "000000", TempBuff[ 20 ];

	memset( Result, 0, 9 );
	memset( Year, 0, 3 );
	memset( StudNumber, 0, 7 );
	memset( TempBuff, 0, 20 );

	if( gettimeofday( &TimeNow_Sec, NULL ) == -1 )
	{
		perror( "Failed to resolve the current time" );
		exit( 1 );
	}

	time_t t = TimeNow_Sec.tv_sec;
	(void)strftime( TempBuff, 20, "%m%d", gmtime(&t));

	fprintf( stderr, "The date is %s.\n", TempBuff );

	if( memcmp( TempBuff, NEWACCYEAR, 4 ) <= 0 )
		TimeNow_Sec.tv_sec -= ONEYEARSECS;

	t = TimeNow_Sec.tv_sec;
	(void)strftime( Year, 3, "%y", gmtime(&t));
	sprintf( StudNumber, "%i", StudentNumber );

	if( strlen( StudNumber ) == 6 )
		sprintf( Result, "%s%s", Year, StudNumber );
	else
		sprintf( Result, "%s%s%s", Year, &Zero[ strlen( StudNumber ) ], StudNumber );
		
        AddCheckDigit( Result );
 
        return;
 
}


