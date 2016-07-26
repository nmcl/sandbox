#include <System/string.h>
#include <System/memory.h>

#include "local.h"
#include "Requests.h"

#ifndef DEFS_H_
#  include "Defs.h"
#endif

extern char *RecordLocked;
extern char *Username;
extern char *Department;

void CreateOpenRecFail( char* String, char* Message )
{
    memcpy( &String[ 16 ], OPENRECFAIL, 2 );
    ::memcpy(&String[ 32 ], Message, ::strlen(Message));

    reportf("Open record failed: %s\n", &String[16]);
}

void OpenRec( char* Command, char* Reply )
{
	reportf( "Open Record request received.\n" );
	memcpy( &Reply[ 16 ], OPENRECSUCC, 2 );
	memcpy( &Reply[ 18 ], &Command[ 18 ], 14 );

	switch( DBFetch( &Command[ 23 ], &Reply[ 23 ], TRUE ))
	{
	case 0:
		ApplySecurity( Reply );
#ifdef VERY_VERBOSE
		reportf("Applied security\n", "");
#endif		
		return;
		break;
	case 1:
		CreateOpenRecFail( Reply, OR_FAILOPENINDEX );
		return;
		break;
	case 2:
		CreateOpenRecFail( Reply, OR_ALREADYLOCKED );
		return;
		break;
	case 3:
		CreateOpenRecFail( Reply, OR_RECNOTFOUND );
		return;
		break;
	case 4:
		CreateOpenRecFail( Reply, OR_GETLOCKFAILED );
		return;
		break;
	case 5:
		CreateOpenRecFail( Reply, OR_OPENDATAFAILED );
		return;
		break;
	case 6:
		CreateOpenRecFail( Reply, OR_FAILEDTOLOCATE );
		return;
		break;
	case 7:
		CreateOpenRecFail( Reply, OR_DATALOCKFAIL );
		return;
		break;
	case 8:
		CreateOpenRecFail( Reply, OR_INDEXLOCKFAIL );
		return;
		break;
	case 9:
		CreateOpenRecFail( Reply, OR_ENTRYDELETED );
		return;
		break;
	case 10:
		CreateOpenRecFail( Reply, OR_GENERALFAULT );
		return;
		break;
	case 11:
		CreateOpenRecFail( Reply, ALREADYOPEN );
		return;
		break;
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
	default:
		CreateOpenRecFail( Reply, OR_OPENRECFAULT );
		return;
		break;
	}
}

