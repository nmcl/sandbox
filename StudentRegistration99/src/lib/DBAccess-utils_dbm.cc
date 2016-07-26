#include <System/unistd.h>
#include <System/stdio.h>
#include <System/ndbm.h>
#include <System/sys/file.h>
#include <System/sys/types.h>
#include <System/fcntl.h>
#include <System/string.h>

#include "userdb.h"
#include "local.h"
#include "Requests.h"

#ifndef DEFS_H_
#  include "Defs.h"
#endif

extern char *RecordLocked;
extern char *RecordCopy;

int DBFetch( char* StudNumb, char* Reply, Boolean Lock )
{
	datum Key, Data;
	struct IndexDBEntry DBInfo;
	FILE *DataFd;
	DBM *dbFile;
	int LoadLoop, LockNumber;

	if( memcmp( RecordLocked, NORECORD, 9 ) != 0 && Lock )
		return 11;

	LockNumber = GetLock( IndexFileLock, F_WRLCK );
	if( LockNumber == -1 )
		return 9;

	if(( dbFile = dbm_open( IndexFile, O_RDWR, 0600 )) == NULL )
	{
		perror( "Failed to open the index database" );
		ReleaseLock( LockNumber );
		return 1;
	}

	Key.dptr = StudNumb;
	Key.dsize = 9;

	Data = dbm_fetch( dbFile, Key );

	if( Data.dptr == NULL )
	{
		dbm_close( dbFile );
		ReleaseLock( LockNumber );
		return 3;
	}

	memcpy( &DBInfo, Data.dptr, sizeof( DBInfo ));

	if( DBInfo.Status == -1 && Lock )
	{
		dbm_close( dbFile );
		ReleaseLock( LockNumber );
		return 10;
	}

	if( DBInfo.Status != 0 && Lock )
	{
		dbm_close( dbFile );
		ReleaseLock( LockNumber );
		return 2;
	}

	DBInfo.Status = getpid();

	Data.dptr = (char *)&DBInfo;

	if( Lock && dbm_store( dbFile, Key, Data, DBM_REPLACE ) < 0)
	{
		perror( "dbm_store failed" );
		dbm_close( dbFile );
		ReleaseLock( LockNumber );
		return 4;
	}

	if( Lock ) memcpy( RecordLocked, Key.dptr, 9 );

	dbm_close( dbFile );
	ReleaseLock( LockNumber );

	LockNumber = GetLock( DataFileLock, F_RDLCK );

	if( LockNumber == -1 )
		return 8;

	if((DataFd = fopen( DataFile, "r" )) == NULL )
	{
		reportf( "Unable to open the datafile.\n" );
		ReleaseLock( LockNumber );
		return 5;
	}

	if( fseek( DataFd, DBInfo.Position, 0 ) == -1 )
	{
		perror( "Unable to seek to position" );
		fclose( DataFd );
		ReleaseLock( LockNumber );
		return 6;
	}

	for( LoadLoop = 0; LoadLoop < RECSIZE && !feof( DataFd ); LoadLoop++ )
		Reply[ LoadLoop ] = fgetc( DataFd );

	if( !feof( DataFd ) )
	{
		fclose( DataFd );
		ReleaseLock( LockNumber );
		memcpy( RecordCopy, Reply, RECSIZE );
		return 0;
	}

	fclose( DataFd );
	ReleaseLock( LockNumber );
	return 7;
}

int DBStore( char* NewData )
{
	datum Key, Data;
	struct IndexDBEntry DBInfo;
	FILE *DataFd;
	DBM *dbFile;
	int LockNumber, WriteLoop, WriteStatus;

	if( memcmp( RecordLocked, NewData, 9 ) != 0 )
		return 11;

	LockNumber = GetLock( IndexFileLock, F_RDLCK );
	if( LockNumber == -1 )
		return 1;

	if(( dbFile = dbm_open( IndexFile, O_RDWR, 0600 )) == NULL )
	{
		perror( "Failed to open the index database" );
		ReleaseLock( LockNumber );
		return 2;
	}

	Key.dptr = NewData;
	Key.dsize = 9;

	Data = dbm_fetch( dbFile, Key );

	if( Data.dptr == NULL )
	{
		dbm_close( dbFile );
		ReleaseLock( LockNumber );
		return 3;
	}

	memcpy( &DBInfo, Data.dptr, sizeof( DBInfo ));

	if( DBInfo.Status == 0 || DBInfo.Status != getpid() )
	{
		dbm_close( dbFile );
		ReleaseLock( LockNumber );
		return 4;
	}

	dbm_close( dbFile );
	ReleaseLock( LockNumber );

	LockNumber = GetLock( DataFileLock, F_WRLCK );

	if( LockNumber == -1 )
		return 5;

	if(( DataFd = fopen( DataFile, "r+" )) == NULL )
	{
		ReleaseLock( LockNumber );
		return 6;
	}

	reportf( "Record write position is %s%i.\n", "", DBInfo.Position );

	if( fseek( DataFd, DBInfo.Position, 0 ) == -1 )
	{
		fclose( DataFd );
		ReleaseLock( LockNumber );
		return 7;
	}

	for( WriteLoop = 0; WriteLoop < RECSIZE &&
		( WriteStatus = putc( NewData[ WriteLoop ], DataFd )) != EOF; WriteLoop++ );

	if( WriteStatus == EOF )
	{
		fclose( DataFd );
		ReleaseLock( LockNumber );
		return 8;
	}

	fclose( DataFd );
	ReleaseLock( LockNumber );

	DBInfo.Status = 0;
	DBInfo.Modified += 1;
	Key.dptr = NewData;
	Key.dsize = 9;
	Data.dptr = (char *) &DBInfo;
	Data.dsize = sizeof( DBInfo );
	LockNumber = GetLock( IndexFileLock, F_WRLCK );

	if(( dbFile = dbm_open( IndexFile, O_RDWR, 0600 )) == NULL )
	{
		perror( "Failed to open index database" );
		ReleaseLock( LockNumber );
		return 9;
	}

	if( dbm_store( dbFile, Key, Data, DBM_REPLACE ) == -1 )
	{
		perror( "Failed to undate the database" );
		dbm_close( dbFile );
		ReleaseLock( LockNumber );
		return 10;
	}

	memcpy( RecordLocked, NORECORD, 9 );
	dbm_close( dbFile );
	ReleaseLock( LockNumber );
	return 0;
}

int DBClose( char* StudNum )
{
	datum Key, Data;
	struct IndexDBEntry DBInfo;
	DBM *dbFile;
	int LockNumber;
	char OutputString[ 70 ];

	memset( OutputString, 0, 70 );
	memcpy( OutputString, StudNum, 9 );

	reportf( "Looking for student number %s.\n", OutputString);
	reportf( "Currently Record locked is %s.\n", RecordLocked );
	if( memcmp( RecordLocked, StudNum, 9 ) != 0 )
		return 6;

	LockNumber = GetLock( IndexFileLock, F_RDLCK );
	if( LockNumber == -1 )
		return 1;

	if(( dbFile = dbm_open( IndexFile, O_RDWR, 0600 )) == NULL )
	{
		perror( "Failed to open index database" );
		ReleaseLock( LockNumber );
		return 2;
	}

	Key.dptr = StudNum;
	Key.dsize = 9;

	Data = dbm_fetch( dbFile, Key );

	if( Data.dptr == NULL )
	{
		dbm_close( dbFile );
		ReleaseLock( LockNumber );
		return 3;
	}

	memset( &DBInfo, 0, sizeof( DBInfo ));
	memcpy( &DBInfo, Data.dptr, sizeof( DBInfo ) );

	if( DBInfo.Status != getpid() )
	{
		dbm_close( dbFile );
		ReleaseLock( LockNumber );
		return 4;
	}

	DBInfo.Status = 0;
	Data.dptr = (char *) &DBInfo;
	Data.dsize = sizeof( DBInfo );

	LockNumber = ChangeLock( LockNumber, F_WRLCK );

	if( dbm_store( dbFile, Key, Data, DBM_REPLACE ) == -1 )
	{
		perror( "dbm_store failed" );
		dbm_close( dbFile );
		ReleaseLock( LockNumber );
		return 5;
	}

	dbm_close( dbFile );
	ReleaseLock( LockNumber );
	memcpy( RecordLocked, NORECORD, 9 );
	return 0;

}

int DBNew( char* StudNumb, char* Reply )
{
	datum Key, Data;
	DBM *dbFile;
	struct IndexDBEntry DBInfo;
	FILE *DataFd;
	int LockNumber, DataLockNumber, WriteLoop, WriteStatus;
	char SourceKey[ 9 ], NewKey[ 9 ], OutputString[ 70 ];
	long NewRecPos;

	if( memcmp( RecordLocked, NORECORD, 9 ) != 0 )
		return 15;

	memset( SourceKey, 0, 9 );
	memset( NewKey, 0, 9 );

	if( memcmp( StudNumb, CREATENEWPATTERN, 9 ) == 0 )
	{
		LockNumber = GetLock( IndexFileLock, F_WRLCK );

		if( LockNumber == -1 )
			return 1;

		if(( dbFile = dbm_open( IndexFile, O_RDWR | O_CREAT, 0600 )) == NULL )
		{
			perror( "Failed to open index database" );
			ReleaseLock( LockNumber );
			return 2;
		}

		memset( SourceKey, 0, 9 );
		memcpy( SourceKey, CREATENEWPATTERN, 9 );
		Key.dptr = SourceKey;
		Key.dsize = 9;

		Data = dbm_fetch( dbFile, Key );

		if( Data.dptr == NULL )
		{
			dbm_close( dbFile );
			ReleaseLock( LockNumber );
			return 3;
		}

		memcpy( &DBInfo, Data.dptr, sizeof( DBInfo ));

		CalcCheck( DBInfo.Status, NewKey );

		memset( SourceKey, 0, 9 );
		memcpy( SourceKey, CREATENEWPATTERN, 9 );
		
		Key.dptr = SourceKey;
		Key.dsize = 9;

		DBInfo.Status += 1;
		Data.dptr = (char *) &DBInfo;
		Data.dsize = sizeof( DBInfo );

		if( dbm_store( dbFile, Key, Data, DBM_REPLACE ) == -1 )
		{
			perror( "dbm_store failed" );
			dbm_close( dbFile );
			ReleaseLock( LockNumber );
			return 4;
		}

		dbm_close( dbFile );
		ReleaseLock( LockNumber );
	}
	else
	  memcpy( NewKey, StudNumb, 9 );

	memset( OutputString, 0, 70 );
	memcpy( OutputString, NewKey, 9 );
	
	reportf( "Newrecord is %s.\n", OutputString);
	reportf( "Starting to install new record.\n" );

	LockNumber = GetLock( IndexFileLock, F_WRLCK );
	if( LockNumber == -1 )
		return 5;

	if(( dbFile = dbm_open( IndexFile, O_RDWR | O_CREAT, 0600 )) == NULL )
	{
		perror( "Failed to open the index database" );
		ReleaseLock( LockNumber );
		return 6;
	}

	Key.dptr = NewKey;
	Key.dsize = 9;

	Data = dbm_fetch( dbFile, Key );

	if( Data.dptr != NULL )
	{
		dbm_close( dbFile );
		ReleaseLock( LockNumber );
		return 7;
	}

	memset( SourceKey, 0, 9 );
	memcpy( SourceKey, CREATENEWPATTERN, 9 );
	Key.dptr = SourceKey;
	Key.dsize = 9;

	Data = dbm_fetch( dbFile, Key );
	
	if( Data.dptr == NULL )
	{
		dbm_close( dbFile );
		ReleaseLock( LockNumber );
		return 8;
	}

	memcpy( &DBInfo, Data.dptr, sizeof( DBInfo ));

	NewRecPos = DBInfo.Position;

	DBInfo.Position += RECSIZE;
	Data.dptr = (char *)&DBInfo;
	Data.dsize = sizeof( DBInfo );

	if( dbm_store( dbFile, Key, Data, DBM_REPLACE ) == -1 )
	{
		dbm_close( dbFile );
		ReleaseLock( LockNumber );
		return 9;
	}

	reportf( "Locking the datafile.\n" );
	DataLockNumber = GetLock( DataFileLock, F_WRLCK );
	if( DataLockNumber == -1 )
	{
		dbm_close( dbFile );
		ReleaseLock( LockNumber );
		return 10;
	};

	reportf( "Opening the Datafile.\n" );
	if(( DataFd = fopen( DataFile, "r+" )) == NULL )
	{
		reportf( "Open failed.\n" );
		dbm_close( dbFile );
		ReleaseLock( LockNumber );
		ReleaseLock( DataLockNumber );
		return 11;
	}

	reportf( "Seeking to %s%i within the datafile.\n", "", DBInfo.Position );
	if( fseek( DataFd, NewRecPos, 0 ) == -1 )
	{
		dbm_close( dbFile );
		ReleaseLock( LockNumber );
		ReleaseLock( DataLockNumber );
		return 12;
	}

	reportf( "Creating new record.\n" );
	memset( &Reply[ 23 ], ' ', RECSIZE );
	memcpy( &Reply[ 23 ], NewKey, 9 );
	CreateBlankRec( &Reply[ 23 ] );

	reportf( "Reply string is\n %s\n", Reply );

	reportf( "Writting record to file.\n" );
	for( WriteLoop = 0; WriteLoop < RECSIZE &&
		( WriteStatus = putc( Reply[ WriteLoop + 23 ], DataFd )) != EOF; WriteLoop++ );

	if( WriteStatus == EOF )
	{
		dbm_close( dbFile );
		fclose( DataFd );
		ReleaseLock( LockNumber );
		ReleaseLock( DataLockNumber );
		return 13;
	}

	reportf( "Closing and release lock on data file.\n" );
	fclose( DataFd );
	ReleaseLock( DataLockNumber );

	Key.dptr = NewKey;
	Key.dsize = 9;

	DBInfo.Status = getpid();
	DBInfo.Position = NewRecPos;
	Data.dptr = (char *)&DBInfo;
	Data.dsize = sizeof( DBInfo );

	reportf( "Storing base record with updated information.\n" );
	if( dbm_store( dbFile, Key, Data, DBM_REPLACE ) == -1 )
	{
		perror( "dbm_store failed" );
		dbm_close( dbFile );
		ReleaseLock( LockNumber );
		return 14;
	}

	reportf( "Closing and releasing lock on index database.\n" );
	dbm_close( dbFile );
	ReleaseLock( LockNumber );
	memcpy( RecordLocked, NewKey, 9 );
	return 0;
}

int DBDel( char* StudNumb )
{
	datum Key, Data;
	struct IndexDBEntry DBInfo;
	int LockNumber;
	DBM *dbFile;

	if( memcmp( RecordLocked, NORECORD, 9 ) == 0 || memcmp( RecordLocked, StudNumb, 9 ) != 0 )
		return 6;

	if(( LockNumber = GetLock( IndexFileLock, F_WRLCK )) == -1 )
		return 1;

	if(( dbFile = dbm_open( IndexFile, O_RDWR, 0600 )) == NULL )
	{
		perror( "Failed to open index database" );
		ReleaseLock( LockNumber );
		return 2;
	}

	Key.dptr = StudNumb;
	Key.dsize = 9;

	Data = dbm_fetch( dbFile, Key );

	if( Data.dptr == NULL )
	{
		dbm_close( dbFile );
		ReleaseLock( LockNumber );
		return 3;
	}

	memcpy( &DBInfo, Data.dptr, sizeof( DBInfo ));

	if( DBInfo.Status != getpid() )
	{
		dbm_close( dbFile );
		ReleaseLock( LockNumber );
		return 4;
	}

	DBInfo.Status = -1;

	Data.dptr = (char*)&DBInfo;
	Data.dsize = sizeof( DBInfo );

	Key.dptr = StudNumb;
	Key.dsize = 9;

	if( dbm_store( dbFile, Key, Data, DBM_REPLACE ) == -1 )
	{
		perror( "IndexDB write failed" );
		dbm_close( dbFile );
		ReleaseLock( LockNumber );
		return 5;
	}

	dbm_close( dbFile );
	ReleaseLock( LockNumber );
	memcpy( RecordLocked, NORECORD, 9 );
	return 0;
}

