#include <System/string.h>
#include <System/memory.h>

#include "local.h"
#include "Requests.h"

#ifndef DEFS_H_
#  include "Defs.h"
#endif

extern char *RecordLocked;
extern char *Username;

void CreateDeleteRecFail( char* String, char* Message )
{
    memcpy( &String[ 16 ], DELETEFAIL, 2 );
    ::memcpy(&String[ 32 ], Message, ::strlen(Message));

    reportf("Delete record failed: %s\n", &String[16]);
}

void DeleteRec( char* Command, char* Reply )
{

	reportf( "Delete Record requests received.\n" );
	memcpy( &Reply[ 16 ], DELETESUCC, 2 );
	memcpy( &Reply[ 18 ], &Command[ 18 ], 14 );

	if( ViolateSec( Command ) != 0 )
	{
		CreateDeleteRecFail( Reply, ACCESSVIOLATION );
		return;
	}

	switch( DBDel( &Command[ 23 ] ))
	{
	case 0:
		return;
		break;
	case 1:
		CreateDeleteRecFail( Reply, DR_INDEXLOCKFAIL );
		return;
		break;
	case 2:
		CreateDeleteRecFail( Reply, DR_DBOPENFAIL );
		return;
		break;
	case 3:
		CreateDeleteRecFail( Reply, DR_NORECORD );
		return;
		break;
	case 4:
		CreateDeleteRecFail( Reply, DR_NOTLOCKOWN );
		return;
		break;
	case 5:
		CreateDeleteRecFail( Reply, DR_DBWRITEFAIL );
		return;
		break;
	case 6:
		CreateDeleteRecFail( Reply, NORECOPEN );
		return;
		break;
	case 20:
	        CreateDeleteRecFail(Reply, ACTIONFAILURE);
		return;
		break;
	default:
		CreateDeleteRecFail( Reply, DR_GENERAL );
		return;
		break;
	}
}

