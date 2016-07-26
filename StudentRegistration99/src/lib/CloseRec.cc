#include <System/stdio.h>
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

void CreateCloseRecFail( char* String, char* Message )
{
    memcpy( &String[ 16 ], CLOSERECFAIL, 2 );
    ::memcpy(&String[ 32 ], Message, ::strlen(Message));
    
    reportf("Close record failed: %s\n", &String[16]);
}

void CloseRec( char* Command, char* Reply )
{
	reportf( "Close Record request received.\n" );
	memcpy( &Reply[ 16 ], CLOSERECSUCC, 2 );
	memcpy( &Reply[ 18 ], &Command[ 18 ], 14 );

	switch( DBClose( &Command[ 23 ] ) )
	{
	case 0:
		return;
	case 1:
		CreateCloseRecFail( Reply, CR_INDEXLOCKFAIL );
		return;
	case 2:
		CreateCloseRecFail( Reply, CR_INDEXOPENFAIL );
		return;
	case 3:
		CreateCloseRecFail( Reply, CR_RECNOTFOUND );
		return;
	case 4:
		CreateCloseRecFail( Reply, CR_NOTLOCKOWNER );
		return;
	case 5:
		CreateCloseRecFail( Reply, CR_LOCKCHANGEFAIL );
		return;
	case 6:
		CreateCloseRecFail( Reply, NORECOPEN );
		return;
	case 29:
		CreateCloseRecFail( Reply, INVALIDNUMBER );
		return;		
	default:
		CreateCloseRecFail( Reply, CR_GENERALERROR );
		return;
	}
}

