#include <System/string.h>
#include <System/memory.h>

#include "local.h"
#include "Requests.h"

#ifndef DEFS_H_
#  include "Defs.h"
#endif

extern char *RecordLocked;
extern char *Username;

void CreateReadRecFail( char* String, char* Message )
{
    memcpy( &String[ 16 ], READFAIL, 2 );
    ::memcpy(&String[ 32 ], Message, ::strlen(Message));

    reportf("Read record failed: %s\n", &String[16]);
}

void ReadRec( char* Command, char* Reply )
{
	int FetchResult;

	reportf( "Read Record request received.\n" );
	memcpy( &Reply[ 16 ], READSUCC, 2 );
	memcpy( &Reply[ 18 ], &Command[ 18 ], 14 );

	if(( FetchResult = DBFetch( &Command[ 23 ], &Reply[ 23 ], FALSE )) != 0 )
	switch( FetchResult )
	{
	case 1 :
		CreateReadRecFail( Reply, RR_FAILOPENINDEX );
		return;
	case 2 :
		CreateReadRecFail( Reply, RR_ALREADYLOCKED );
		return;
	case 3 :
		CreateReadRecFail( Reply, RR_RECNOTFOUND );
		return;
	case 4 :
		CreateReadRecFail( Reply, RR_GETLOCKFAILED );
		return;
	case 5 :
		CreateReadRecFail( Reply, RR_OPENDATAFAILED );
		return;
	case 6 :
		CreateReadRecFail( Reply, RR_FAILEDTOLOCATE );
		return;
	case 12:
	        CreateOpenRecFail(Reply, OR_EMPTYRECORD);
		return;
		break;
	case 20:
	        CreateOpenRecFail(Reply, ACTIONFAILURE);
		return;
		break;
	case 24:
	        CreateOpenRecFail(Reply, OR_HOSTFAILURE);
		return;
		break;
	case 25:
	        CreateOpenRecFail(Reply, OR_SERVERFAILURE);
		return;
		break;
	case 26:
	        CreateOpenRecFail(Reply, OR_SERVERACTIVE);
		return;
		break;
	case 29:
	        CreateOpenRecFail(Reply, INVALIDNUMBER);
		return;
		break;
	case 30:
	        CreateOpenRecFail(Reply, SERVERFAILURE);
		return;
		break;
	case 31:
	        CreateOpenRecFail(Reply, VIRTUALSERVERFAILURE);
		return;
		break;
	case 32:
	        CreateOpenRecFail(Reply, OR_ISDUMPING);
		return;
		break;		
	default :
		CreateReadRecFail( Reply, RR_OPENRECFAULT );
		return;
	}

	return ;
}

