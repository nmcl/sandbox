#include <System/sys/file.h>
#include <System/sys/types.h>
#include <System/unistd.h>
#include <System/fcntl.h>
#include <System/stdio.h>

#ifndef DEFS_H_
#  include "Defs.h"
#endif

int GetLock( char* File, int Style )
{
	int fd;
	struct flock LockInfo;

	if(( fd = open( File, O_RDWR | O_CREAT, 0600 )) == -1 )
	{
		perror( "Failed to open lock file" );
		return -1;
	}

	LockInfo.l_type = Style;
	LockInfo.l_whence = SEEK_SET;
	LockInfo.l_start = 0;
	LockInfo.l_len = 0;

	if( fcntl( fd, F_SETLKW, &LockInfo ) == -1 )
	{
	    perror( "flock failed on required file" );
	    close( fd );
	    return -1;
	}

	return fd;
}

int ChangeLock( int fd, int Style )
{
	struct flock LockInfo;

	LockInfo.l_type = Style;
	LockInfo.l_whence = SEEK_SET;
	LockInfo.l_start = 0;
	LockInfo.l_len = 0;

	if( fcntl( fd, F_SETLKW, &LockInfo ) == -1 )
	{
		perror( "flock - change lock failed" );
		ReleaseLock( fd );
		return -1;
	}
	else
		return fd;
}

int ReleaseLock( int fd )
{
	struct flock LockInfo;

	LockInfo.l_type = F_UNLCK;
	LockInfo.l_whence = SEEK_SET;
	LockInfo.l_start = 0;
	LockInfo.l_len = 0;

	if( fcntl( fd, F_SETLKW, &LockInfo ) == -1 )
	{
		perror( "flock - release failed" );
		close( fd );
		return -1;
	}
	else
	{
		close( fd );
		return 0;
	}
}

