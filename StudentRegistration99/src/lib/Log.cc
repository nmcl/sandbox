#include <System/string.h>
#include <System/stdio.h>
#include <System/time.h>
#include <System/sys/file.h>
#include <System/fcntl.h>
#include <System/sys/time.h>
#include <System/unistd.h>
#include <System/sys/types.h>

#include "local.h"
#include "Requests.h"

#ifndef DEFS_H_
#  include "Defs.h"
#endif

extern char *Username;
extern FILE* outstr;

/*
 * Every Registration now has its own log file, so we don't need a
 * lock.
 */

void StartTime ()
{
	char Message[ 80 ], UserString[ USERNAMELEN + 1 ], TimeInfo[80];
	struct timeval tp;
	time_t t;

	memset( UserString, 0, USERNAMELEN + 1 );
	memcpy( UserString, Username, USERNAMELEN );
	memset( Message, 0, 80 );
	gettimeofday( &tp, NULL );

	t = (time_t) tp.tv_sec;
	strftime(TimeInfo, 80, "%d/%m/%y (%a) %H:%M:%S\0", localtime(&t));

	sprintf( Message, "%s %i %s started", TimeInfo, getpid(), UserString);

	fprintf(outstr, "%s\n", Message);
	fflush(outstr);
}

void EndTime ()
{
	char Message[ 80 ], TimeInfo[80], UserString[USERNAMELEN +1];
	struct timeval tp;
	time_t t;
 
	memset( Message, 0, 80 );
	gettimeofday( &tp, NULL );
	t = (time_t) tp.tv_sec;

	strftime(TimeInfo, 80, "%d/%m/%y (%a) %H:%M:%S\0", localtime(&t));
 
	sprintf( Message, "%s %i %s finished", TimeInfo, getpid(), UserString );

	fprintf(outstr, "%s\n", Message);
	fflush(outstr);	
}

void Log ( char* Req, int ReqLen, char* Reply, int ReplyLen )
{
	char Message[ 200 ], ReqSeg[ 33 ], RepSeg[ 33 ], UserString[ USERNAMELEN + 1 ], TimeInfo[ 80 ];
	struct timeval tp;
	time_t t;

	memset( Message, 0, 200 );
	memset( ReqSeg, 0, 33 );
	memset( RepSeg, 0, 33 );
	memset( UserString, 0, USERNAMELEN + 1 );

	gettimeofday( &tp, NULL );
	t = (time_t) tp.tv_sec;

	strftime(TimeInfo, 80, "%d/%m/%y (%a) %H:%M:%S\0", localtime(&t));

	memcpy( UserString, Username, USERNAMELEN );
	memcpy( ReqSeg, Req, ( ReqLen < 32 ? ReqLen : 32 ));
	memcpy( RepSeg, Reply, (ReplyLen < 32 ? ReplyLen : 32 ));

	sprintf( Message, "%s %i %s Req: %s Rep: %s", TimeInfo, getpid(), UserString, ReqSeg, RepSeg );

	fprintf(outstr, "%s\n", Message);
	fflush(outstr);
}

