#include <System/stdio.h>
#include <System/string.h>
#include <System/fcntl.h>
#include <System/unistd.h>

#include "local.h"
#include "Requests.h"
#include "des-utils.h"

#ifndef DEFS_H_
#  include "Defs.h"
#endif

/* Create a version failure message */
void CreateVersionFail( char* OutputLine, char* Message )
{
	memset( OutputLine, 0, MAXBUFF );
	memcpy( &OutputLine[ 16 ], VERSIONFAIL, 2 );
	memset( &OutputLine[ 18 ], '0', 14 );
	memcpy( &OutputLine[ 32 ], Message, strlen( Message ));
}

/* Create a version success message */
void CreateVersionSuccess( char* OutputLine )
{
	memset( OutputLine, 0, MAXBUFF );
	memcpy( &OutputLine[ 16 ], VERSIONSUCC, 2 );
	memset( &OutputLine[ 18 ], '0', 14 );
}

/* This function checks the version of the connected client */
int CheckVersion( char* VersionLine, char* Reply )
{
	char FileBuffer[ 2048 ], MinVersion[ 20 ], CurrentVersion[ 20 ];
	int VersionFilefd, BytesRead, StartPosition;

			/* Validate that the token to the message is young */
	
	if ( CheckToken( VersionLine ) == -1 )
	{
		CreateActionFault( Reply, OLDTOKEN_AF, TRUE );
		/* If the token isn't young then create an action fault message and
		   send it to the client and return a fail status */
		return 0;
	}

#ifndef DEBUG	
	/* Validate that the incoming message is a version statement */
	if ( memcmp( &VersionLine[ 16 ], VERSION, strlen( VERSION ) ) !=0 )
	{
	    /* If not then create an action fault, send it and return a fail status */
	    CreateActionFault( Reply, INCORRECTACTION, TRUE );
	    return 0;
	}

	/* Open the version information file */
	if ( ( VersionFilefd = open( VersionFile, O_RDONLY, 0600 )) == -1 )
	{
	    /* If the open fails then create an action fault, send it and then return a fail status */
		perror( "Failed to open version information file" );
		CreateActionFault( Reply, OPENFILEFAIL, TRUE );
                return 0;
	}

	/* Empty the file input buffer and then read no more than 2048 bytes from the file */
	memset( FileBuffer, 0, 2048 );
	BytesRead = read( VersionFilefd, FileBuffer, 2048 );
	/* Close the version information file */
	close( VersionFilefd );

	/* Check to see if the input line long enough and that the client type character
	   can be found within the version information file */
	if( strlen( VersionLine ) >= 41 && memchr( FileBuffer, VersionLine[ 40 ], BytesRead ) != NULL )
	{
	    /* If it is found then the version information has been found and this calcs the
	       start position of the information */
	    
	    StartPosition = (char *)memchr( FileBuffer, VersionLine[ 40 ], BytesRead ) - FileBuffer + 1;
	}
	else
	{
	    /* If there is a problem then set the start of the information to an impossible value */
	    StartPosition = -1;
	}

	/* Check to see if the start of the information is impossible */
	if( StartPosition < 0 )
	{
	    CreateVersionFail( Reply, UNKNOWNMACHTYPE );
	    return 0;
	}

	/* Pull the version information out of the file buffer */
	sscanf( &FileBuffer[ StartPosition ], "%s %s", MinVersion, CurrentVersion );

	/* Check the clients version against the current version number */
	if( memcmp( CurrentVersion, &VersionLine[32], 8 ) <= 0 )
	{
	    CreateVersionSuccess( Reply );
	    return -1;
	}
	else    /* Check the clients version number against that of minimum version number */
	{
	    if( memcmp( MinVersion, &VersionLine[32], 8 ) <= 0 )
	    {
		CreateVersionFail( Reply, OLDVERSION );
		return -1;
	    }
	    else
	    {
		CreateVersionFail( Reply, OLDVERSION_FATAL );
		return 0;
	    }
	}
#else
	CreateVersionSuccess( Reply );
	return -1;
#endif	
}

