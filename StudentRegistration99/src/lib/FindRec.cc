#include <System/string.h>
#include <System/memory.h>
#include <System/stdio.h>
#include <System/ndbm.h>
#include <System/fcntl.h>
#include <System/sys/types.h>
#include <System/unistd.h>

#ifdef ARJUNA
#include "arjuna.h"
#include <Arjuna/AtomicA.h>
#endif

#include "local.h"
#include "Requests.h"

#ifndef DEFS_H_
#  include "Defs.h"
#endif

extern "C"
{
  int mkstemp (char*);
}

extern char *RecordLocked;
extern char *Username;
extern char *TempFileTemplate;
extern int WrConn;
extern int CurrentTmpFile;

void CreateFindRecFail( char* String, char* Message )
{
    memcpy( &String[ 16 ], FINDFAIL, 2 );
    ::memcpy(&String[ 32 ], Message, ::strlen(Message));

    reportf("Find record failed: %s", &String[16]);
}

int IsBlank( char* Pattern, int Length )
{
	char BlankSpace[ 65 ];

	memset( BlankSpace, ' ', 65 );

	return ( memcmp( Pattern, BlankSpace, Length ) == 0 ? OK : NOT_OK );
}

int ChkDegCode( char* Pattern, char* Record )
{
	return ( memcmp( &Pattern[ 769 ], &Record[ 769 ], 4 ) == 0 ? OK : NOT_OK );
}

int ChkStage( char* Pattern, char* Record )
{
        return ( Pattern[ 773 ] == Record[ 773 ] ? OK : NOT_OK );
}

int ChkOtherNames( char* Pattern, char* Record )
{
	char PatternNames[ 66 ], OtherNames[ 66 ], *CurrentName;

	memset( OtherNames, 0, 66 );
	memset( PatternNames, 0, 66 );
	
	memcpy( OtherNames, &Record[ 47 ], 65 );
	memcpy( PatternNames, &Pattern[ 47 ], 65 );

	CurrentName = strtok( PatternNames, " " );

	while( CurrentName != NULL )
	if( strstr( OtherNames, CurrentName ) == NULL )
		return NOT_OK;
	else
		CurrentName = strtok( NULL, " " );

        return OK;
}

int ChkSex( char* Pattern, char* Record )
{
        return ( Pattern[ 115  ] == Record[ 115 ] ? OK : NOT_OK );
}

int ChkInitials( char* Pattern, char* Record )
{
       	int ProcessLoop;
	char Initials[ 5 ], *Result;

	memset( Initials, 0, 5 );
	
	memcpy( Initials, &Record[ 120 ], 4 );

	for( ProcessLoop = 0; ProcessLoop < 4; ProcessLoop++ )
	if((Pattern[ 120 + ProcessLoop ] != ' ') && ( (Result = strchr( Initials, Pattern[ 120 + ProcessLoop ] )) == NULL))
		return NOT_OK;

	return OK;
}


int ChkDOB( char* Pattern, char* Record )
{
	return ( memcmp( &Pattern[ 124 ], &Record[ 124 ], 8 ) == 0 ? OK : NOT_OK );
}

int RecordMatch( char* Pattern, char* Record )
{

	char BlankSpace[ 65 ];

	memset( BlankSpace, ' ', 65 );

	if( IsBlank( &Pattern[ 769 ], 4 ) != OK && ChkDegCode( Pattern, Record ) != OK )
		return NOT_OK;
	else if( IsBlank( &Pattern[ 773 ], 1 ) != OK && ChkStage( Pattern, Record ) != OK )
		return NOT_OK;
	else if( IsBlank( &Pattern[ 47 ], 65 ) != OK && ChkOtherNames( Pattern, Record ) != OK )
		return NOT_OK;
	else if( IsBlank( &Pattern[ 115 ], 1 ) != OK && ChkSex( Pattern, Record ) != OK )
		return NOT_OK;
	else if( IsBlank( &Pattern[ 120 ], 4 ) != OK && ChkInitials( Pattern, Record ) != OK )
		return NOT_OK;
	else if( IsBlank( &Pattern[ 124 ], 8 ) != OK && ChkDOB( Pattern, Record ) != OK )
		return NOT_OK;

	return OK;
}

int FindMatches( char* Pattern, int TmpFile )
{
	char Possible[ MAXFINDRECS * 47 ], Record[ 1024 ], FileRecord[ 64 ], Number[ 5 ];
	DBM *DBHandle;
	int CheckingLoop, NumOfMatches = 0, fd, NumberLoop;
	datum key,data;
	long Position;

	memset( Possible, 0, MAXFINDRECS * 47 );

	if(( DBHandle = dbm_open( SurIndexFile, O_RDONLY, 0600 )) == NULL )
	{
		perror( "Failed to open the Surname database" );
		return -1;
	}

	key.dptr = &Pattern[ 9 ];
	key.dsize = 2;

	data = dbm_fetch( DBHandle, key );

	if( data.dptr == NULL )
	{
		dbm_close( DBHandle );
		return -2;
	}

	memcpy( &Position, data.dptr, sizeof( Position ));

	dbm_close( DBHandle );

	if(( fd = open( SurDataFile, O_RDONLY, 0600 )) == -1 )
	{
		perror( "Failed to open the surname datafile" );
		return -3;
	}

	if( lseek( fd, Position, SEEK_SET ) == -1 )
	{
		perror( "Failed to seek to data location" );
		return -4;
	}

	if( read( fd, Possible, MAXFINDRECS * 47 ) == -1 )
	{
		perror( "Failed to read from the datafile" );
		return -5;
	}

	close( fd );

#if defined(ARJUNA) && defined(LOCAL_ONLY)
	if (openDBM() != 0)
	    return -5;

	AtomicAction A;

	A.Begin();
#endif
	
	for( CheckingLoop = 0; CheckingLoop < MAXFINDRECS; CheckingLoop++ )
	if( memcmp( &Pattern[ 9 ], &Possible[ CheckingLoop * 47 ], 38 ) == 0 )
	{
		memset( Record, 0, 1024 );
		memset( FileRecord, ' ', 64 );
		memcpy( FileRecord, &Possible[ CheckingLoop * 47 + 38 ], 9 );

#if defined(ARJUNA) && defined(LOCAL_ONLY)
		if( DBFind( &Possible[ CheckingLoop * 47 + 38 ], Record ) != 0 )
#else
		if( DBFetch( &Possible[ CheckingLoop * 47 + 38 ], Record, FALSE ) != 0 )
#endif
			memcpy( &FileRecord[ 10 ], FR_NODETAILS, strlen( FR_NODETAILS ));
		else if( RecordMatch( Pattern, Record ) == OK )
		{
			FileRecord[ 10 ] = Record[ 144 ];
			memcpy( &FileRecord[ 12 ], &Record[ 769 ], 4 );
			FileRecord[ 17 ] = Record[ 773 ];
			memcpy( &FileRecord[ 19 ], &Record[ 47 ], 40 );
			memcpy( &FileRecord[ 60 ], "0000", 4 );
		}
		else
			continue;

		if( write( TmpFile, FileRecord, 64 ) != 64 )
		{
			perror( "Failed to write to tmpfile " );
#if defined(ARJUNA) && defined(LOCAL_ONLY)
			A.Abort();
			(void) closeDBM();
#endif			
			return -6;
		}
	}

#if defined(ARJUNA) && defined(LOCAL_ONLY)
	A.End();

	(void) closeDBM();
#endif
	
	switch((NumOfMatches = lseek( TmpFile, 0, SEEK_CUR)/64))
	{
	case -1:
		perror( "Failed to get current position" );
		return -7;
		break;
	case 0:
		return 0;
		break;
	default:
		if( lseek( TmpFile, 0, SEEK_SET ) == -1 )
		{
			perror( "Failed to seek to 0 position" );
			return -8;
		}
		break;
	}

	for( NumberLoop = 0; NumberLoop < NumOfMatches; NumberLoop++ )
	{
	  memset( FileRecord, 0, 64 );
	  
		if(read( TmpFile, FileRecord, 64 ) != 64 )
		{
			perror( "Fail to read correct number of charactes" );
			return -9;
		}

		sprintf( Number, "%i", NumOfMatches - NumberLoop );
		memset( &FileRecord[ 60 ], '0', 4 );
		memcpy( &FileRecord[ 64 - strlen( Number ) ], Number, strlen( Number ));

		if( lseek( TmpFile, -64, SEEK_CUR ) == -1 )
		{
			perror( "Failed to reposition within temp file" );
			return -10;
		}

		if( write( TmpFile, FileRecord, 64 ) != 64 )
		{
			perror( "Failed to write to tempfile" );
			return -11;
		}
	}

	return NumOfMatches;
}

void FindRec( char* Command, char* Reply )
{
	int CharRead;

	if( IsRegistration() )
	{
		memcpy( &Reply[ 18 ], &Command[ 18 ], 14 );
		CreateFindRecFail( Reply, FR_FINDBAR );
		return;
	}
		
	memcpy( &Reply[ 16 ], FINDSUCC, 2 );
	memcpy( &Reply[ 18 ], &Command[ 18 ], 14 );

	if( memcmp( &Command[ 23 ], "000000000", 9 ) == 0 )
	{
		if( CurrentTmpFile != -1 )
		{
			close( CurrentTmpFile );
			unlink( TempFileTemplate );
		}

		memset( TempFileTemplate, 0, 256 );
		
		memcpy( TempFileTemplate, TMPFILETEMPLATE, strlen( TMPFILETEMPLATE ));
		if(( CurrentTmpFile = mkstemp( TempFileTemplate )) == -1 )
		{
			perror( "Unable to open temp file" );
			CreateFindRecFail( Reply, FR_TMPOPENFAIL );
			return;
		}

		if( FindMatches( &Command[ 23 ], CurrentTmpFile ) == -1 )
		{
			CreateFindRecFail( Reply, FR_TMPMAKEFAIL );
			return;
		}

		if( lseek( CurrentTmpFile, 0, SEEK_SET ) == -1 )
		{
			perror( "Failed to seek to beginning of datafile." );
			CreateFindRecFail( Reply, FR_TMPSEEKFAIL );
			return;
		}
	}
	else if( CurrentTmpFile == -1 )
	{
		CreateFindRecFail( Reply, FR_NOTMPFILE );
		return;
	}

	if((CharRead = read( CurrentTmpFile, &Reply[ 23 ], 1024 )) == -1 )
	{
		perror( "Failed to read the required number of characters" );
		close( CurrentTmpFile );
		unlink( TempFileTemplate );
		CurrentTmpFile = -1;
		CreateFindRecFail( Reply, FR_TMPREADFAIL );
		return;
	}
	else if( CharRead == 0 )
	{
		close( CurrentTmpFile );
		unlink( TempFileTemplate );
		CurrentTmpFile = -1;
		CreateFindRecFail( Reply, FR_NORECORDS );
		return;
	}
	else if( CharRead != 1024 || memcmp( &Reply[ 1044 ], "0001", 4 ) == 0)
	{
		close( CurrentTmpFile );
		unlink( TempFileTemplate );
		CurrentTmpFile = -1;
		if( CharRead != 1024 )
			memcpy( &Reply[ 23 + CharRead ], "000000000", 9 );
	}

	return;
}


