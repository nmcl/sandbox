#include <System/ndbm.h>
#include <System/sys/file.h>
#include <System/sys/types.h>
#include <System/unistd.h>
#include <System/fcntl.h>
#include <System/stdio.h>
#include <System/stdlib.h>
#include <System/string.h>

#include "local.h"
#include "des-utils.h"
#include "userdb.h"

static int LockFilefd;
static DBM *dbFile;

void InitSKUtils()
{
	LockFilefd = -1;
	dbFile = NULL;
}

void GetDBMLock( int Style )
{
	struct flock LockInfo;

	if ( (LockFilefd == -1) && ( LockFilefd = open( DBMLockFile, O_RDWR | O_CREAT, 0600 )) == -1 )
	{
		perror( "Failed to open the DBLock file" );
		exit( 1 );
	}

	LockInfo.l_type = Style;
	LockInfo.l_whence = SEEK_SET;
	LockInfo.l_start = 0;
	LockInfo.l_len = 0;
	if( fcntl( LockFilefd, F_SETLKW, &LockInfo ) == -1 )
	{
		perror( "flock failed on DB lockfile" );
		exit( 1 );
	}
};

void ReleaseDBMLock()
{
	struct flock LockInfo;

	LockInfo.l_type = F_UNLCK;
	LockInfo.l_whence = SEEK_SET;
	LockInfo.l_start = 0;
	LockInfo.l_len = 0;

	if( fcntl( LockFilefd, F_SETLK, &LockInfo ) == -1 )
		perror( "Failed to release the lock on DB lockfile" );

	close( LockFilefd );
	LockFilefd = -1;
}

void OpenDBM( int Style )
{
	GetDBMLock( Style );

	int mode = ((Style == F_WRLCK) ? O_RDWR | O_CREAT : O_RDONLY | O_CREAT);

	dbFile = dbm_open( DBFile, mode, 0 );

	while ((dbFile == NULL) && (errno == EAGAIN))
	{
	    dbFile = dbm_open( DBFile, mode, 0 );
	}

	if (dbFile == NULL)
	{
	    perror( "Failed to open the StrongKey DB" );
	    exit( 1 );
	}
}

void CloseDBM()
{
	dbm_close( dbFile );
	dbFile = NULL;
	ReleaseDBMLock();
}

int GetUser( char* UserID, struct DBEntry* UserRecord )
{
	datum key, data;

	key.dptr = UserID;
	key.dsize = 8;

	OpenDBM( F_RDLCK );
	data = dbm_fetch( dbFile, key );

	if( data.dptr == NULL )
	{
	    CloseDBM();
	    return -1;
	}

	memset( UserRecord, 0, sizeof( *UserRecord ));
	memcpy( UserRecord, data.dptr, data.dsize);

	CloseDBM();
	
	return 0;
}

int SaveUser( char* UserID, struct DBEntry* anEntry, int Mode )
{
	datum key, data;
	int ReturnResult;

	key.dptr = UserID;
	key.dsize = 8;

	data.dptr = (char *)anEntry;
	data.dsize = sizeof( *anEntry );

	OpenDBM( F_WRLCK );
	ReturnResult = dbm_store( dbFile, key, data, Mode );
	CloseDBM();

	return ReturnResult;
}

int AddUser( char* UserID, struct DBEntry* UserRecord )
{
	return( SaveUser( UserID, UserRecord, DBM_INSERT ));
}

int ChangePassword( char* UserID, char* UserKey )
{
	struct DBEntry anEntry;

	if( GetUser( UserID, &anEntry ) == -1 )
		return -1;

	memcpy( anEntry.PasswordKey, UserKey, 16 );

	return( SaveUser( UserID, &anEntry, DBM_INSERT ));
}

int ResolveKey( char* UserID, char* UserKeyHex )
{
	struct DBEntry anEntry;
	datum key, data;
 
	memset( UserKeyHex, 0, 16 );
	key.dptr = UserID;
	key.dsize = 8;
 
	memset( &anEntry, 0, sizeof( anEntry ));
 
	OpenDBM( F_RDLCK );
	data = dbm_fetch( dbFile, key );
 
	if( data.dptr != NULL )
	    memcpy( &anEntry, data.dptr, data.dsize);
	else
	{
	    CloseDBM();	    
	    return -2;
	}
 
	CloseDBM();
	memcpy( UserKeyHex, anEntry.PasswordKey, 16 );

	return (UserKeyHex[0] == '\0' ? -1:0 );
}

