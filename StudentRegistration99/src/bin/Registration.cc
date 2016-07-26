#include <System/signal.h>
#include <System/stdio.h>
#include <System/errno.h>
#include <System/string.h>
#include <System/sys/file.h>
#include <System/sys/stat.h>
#include <System/sys/types.h>
#include <System/unistd.h>
#include <System/fcntl.h>
#include <System/errno.h>
#include <System/stdlib.h>
#include <System/sys/param.h>

#include "local.h"
#include "Requests.h"

#ifndef DEFS_H_
#  include "Defs.h"
#endif

extern FILE *outstr;
extern Boolean verbose;

char *RecordLocked;
char *Username;
char *RecordCopy;
char *TempFileTemplate;
int WrConn;
int CurrentTmpFile;

#ifdef ARJUNA
extern void initArjuna ();
extern void tidyupArjuna ();
#endif

static Mults[] = { 1, 3, 7, 9, 1, 3, 7, 9 };

Boolean VerifyNumb ( char* theNumber )
{
	int Total = 0, Loop;

	for( Loop = 0; Loop < 8; Loop++ )
		Total += ( theNumber[ Loop ] - '0' ) * Mults[ Loop ];

	Total = ( 10 - (Total % 10) ) % 10;

	return ( Total + '0' == theNumber[ 8 ] ? TRUE : FALSE );
}

void Process( int RdConn )
{
	char Command[ MAXBUFF ], Reply[ MAXBUFF ];
	int NoYoungCount = 0, VersionResult = -1;

#ifdef NO_SAVE
	sleep( 2 );
	memset( Reply, 0, MAXBUFF );
	memcpy( &Reply[ 16 ], MESSAGE, 2 );
	memcpy( &Reply[ 18 ], "00000000000000", 14 );
	memcpy( &Reply[ 32 ], NOSAVEMESS, strlen( NOSAVEMESS ));
	Send( RdConn, Reply );
#else
#ifdef SHUTDOWN_TIME
	sleep( 2 );
	memset( Reply, 0, MAXBUFF );
	memcpy( &Reply[ 16 ], MESSAGE, 2 );
	memcpy( &Reply[ 18 ], "00000000000000", 14 );
	memcpy( &Reply[ 32 ], SHUTDOWNMESS, strlen( SHUTDOWNMESS ));
	Send( RdConn, Reply );
#endif
#endif

	do
	{
		memset( Command, 0, MAXBUFF );
		memset( Reply, 0, MAXBUFF );
		reportf( "Currently record locked is %s.\n", RecordLocked, 0, 0, 0, 0, 0 );

		Receive( RdConn, Command );

		if( Command[ 0 ] == '\0' )
			continue;

		if( strlen( Command ) < 32 )
		{
			char ReplyLine[ MAXBUFF ];

			memset( ReplyLine, 0, MAXBUFF );
			CreateActionFault( ReplyLine, NOENOUGHCHARS, FALSE );
			Log( Command, strlen( Command ), &ReplyLine[ 16 ], strlen( ReplyLine )- 16 );
			Send( WrConn, ReplyLine );
			continue;
		}

		if( CheckToken( Command ) != 0 )
		{
			char ReplyLine[ MAXBUFF ];

			NoYoungCount += 1;
			memset( ReplyLine, 0, MAXBUFF );
			memcpy( ReplyLine, Command, 8 );
			if( NoYoungCount == NOYOUNGLIMIT )
			{
				CreateActionFault( ReplyLine, NOTYOUNG_FATAL, TRUE );
				Log( Command, 32, &ReplyLine[ 16 ], strlen( ReplyLine ) - 16 );
				Send( WrConn, ReplyLine );
				return;
			}
			else
			{
				CreateActionFault( ReplyLine, OLDTOKEN_AF, FALSE );
				Log( Command, 32, &ReplyLine[ 16 ], strlen( ReplyLine ) - 16 );
				Send( WrConn, ReplyLine );
				continue;
			}
		}
		else
			NoYoungCount = 0;

		if( !VerifyNumb( &Command[ 23 ] ) )
			CreateActionFault( Reply, INVALIDSTUDNUMB, FALSE );
		else if( memcmp( &Command[ 16 ], NEWREC, strlen( NEWREC )) == 0 )
			NewRec( Command, Reply );
		else if( memcmp( &Command[ 16 ], OPENREC, strlen( OPENREC )) == 0 )
			OpenRec( Command, Reply );
		else if( memcmp( &Command[ 16 ], CLOSEREC, strlen( CLOSEREC )) ==0 )
			CloseRec( Command, Reply );
		else if( memcmp( &Command[ 16 ], SAVEREC, strlen( SAVEREC )) == 0 )
			SaveRec( Command, Reply );
		else if( memcmp( &Command[ 16 ], DELETEREC, strlen( DELETEREC )) == 0 )
			DeleteRec( Command, Reply );
		else if( memcmp( &Command[ 16 ], FINDREC, strlen( FINDREC )) == 0 )
			FindRec( Command, Reply );
		else if( memcmp( &Command[ 16 ], READREC, strlen( READREC )) == 0 )
			ReadRec( Command, Reply );
		else if( memcmp( &Command[ 16 ], VERSION, strlen( VERSION )) == 0 )
			VersionResult = CheckVersion( Command, Reply );
		else
			CreateActionFault( Reply, CMDNOTKNOWN, FALSE );

		Log( Command, 32, &Reply[ 16 ], strlen( &Reply[ 16 ] ) );
		Send( WrConn, Reply );

		sleep( 2 );
	} while( Command[ 0 ] != '\0' && VersionResult == -1 );

	if( VersionResult != -1 )
	{
		memset( Reply, 0, MAXBUFF );
		CreateActionFault( Reply, VERSIONFAIL_AF, TRUE );
		Send( WrConn, Reply );
	}
}

int main( int argc, char** argv )
{
	char CurrentRecord[ 10 ], User[ USERNAMELEN+1 ], UserKeyHex[ 16 ],
		UserKeyBit[ 8 ],
		TempFilename[ 256 ], UserLockFile[ 256 ], UserMessage[ 256 ],
		Record[ RECSIZE ];
	int RdConn, UserLock;
	struct flock LockInfo;

#ifdef ARJUNA	
	initArjuna();
#endif
	InitSKUtils();

	memset(CurrentRecord, '\0', 10);
	
	RecordLocked = CurrentRecord;
	memcpy( RecordLocked, NORECORD, strlen( NORECORD ));

	Username = User;
	memset( Username, '\0', USERNAMELEN+1 );

	if (strlen(argv[1]) > USERNAMELEN)
	    ::strncpy(Username, argv[1], USERNAMELEN);
	else
	    ::strcpy(Username, argv[1]);

	RecordCopy = Record;
	memset( RecordCopy, 0, RECSIZE );

	TempFileTemplate = TempFilename;

	ResolveKey( Username, UserKeyHex );
	desinit( HexStringToBytes( UserKeyHex, UserKeyBit, 8 ));

	/*
	 * Why do we do this twice?
	 */
	
	if (strlen(argv[1]) > USERNAMELEN)
	    ::strncpy(Username, argv[1], USERNAMELEN);
	else
	    ::strcpy(Username, argv[1]);

	RdConn = ( argc >= 4 ? atoi( argv[ 2 ] ) : 0 );
	WrConn = ( argc >= 4 ? atoi( argv[ 3 ] ) : 1 );

	if (argc > 4)
	    verbose = ((::atoi(argv[4]) == 1) ? TRUE : FALSE);
	else
	    verbose = FALSE;

	char fileName[MAXPATHLEN+1];
	::memset(fileName, '\0', MAXPATHLEN+1);

	::sprintf(fileName, LogFile, getpid());
	    
	outstr = fopen(fileName, "w+");

	if (!outstr)
	    outstr = stdout;
	    
	CurrentTmpFile = -1;

	if( access( UserDIR, F_OK ) == -1 )
	{
	    if( mkdir( UserDIR, 0777 ) == -1 )
	    {
		perror( "Failed to create user lock directory" );
	    }
	}

	(void)umask( 0011 );

	LockInfo.l_type = F_WRLCK;
	LockInfo.l_whence = SEEK_SET;
	LockInfo.l_start = 0;
	LockInfo.l_len = 0;

	memset( UserLockFile, 0, 256 );
	(void) sprintf( UserLockFile, "%s/%s", UserDIR, User );
	
	if(( UserLock = open( UserLockFile, O_RDWR | O_CREAT, 0666 )) == -1)
	{
		perror( "Failed to open the user record lock" );
		if( errno == ETIMEDOUT )
		{
		  memset( UserMessage, 0, 256 );
		  
		  CreateActionFault( UserMessage, USERLOCKFAIL, FALSE );
	
		  Send( WrConn, UserMessage );
		}

		StartTime();
		Process( RdConn );
		EndTime();
	}
	else if( fcntl( UserLock, F_SETLK, &LockInfo ) == -1 )
	{
		perror( "Failed to lock user record lock." );
		memset( UserMessage, 0, 256 );

		CreateActionFault( UserMessage, ALREADYLOGGEDIN, TRUE );
		if( errno == EACCES )
			CreateActionFault( UserMessage, ALREADYLOGGEDIN, TRUE );
		else
			CreateActionFault( UserMessage, USERLOCKFAIL, TRUE );

		Send( WrConn, UserMessage );
	}
	else
	{
		StartTime();
		Process( RdConn );
		EndTime();
	}

	close( RdConn );
	close( WrConn );

	LockInfo.l_type = F_UNLCK;
	LockInfo.l_whence = SEEK_SET;
	LockInfo.l_start = 0;
	LockInfo.l_len = 0;

	if( UserLock != -1 && fcntl( UserLock, F_SETLK, &LockInfo ) == -1 )
	    perror( "Failed to unlock user record" );
	
	if( UserLock != -1 )
	  close( UserLock );

	if( CurrentTmpFile != -1 )
	{
	    close( CurrentTmpFile );
	    unlink( TempFileTemplate );
	};

	if (memcmp( RecordLocked, NORECORD, strlen( NORECORD )) != 0)
	    DBClose( RecordLocked );

#ifdef ARJUNA
	tidyupArjuna();
#endif	

	fclose(outstr);
	
	return 0;
}

