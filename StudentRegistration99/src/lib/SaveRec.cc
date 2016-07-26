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

void CreateSaveRecFail( char* String, char* Message )
{
    memcpy( &String[ 16 ], SAVEFAIL, 2 );
    ::memcpy(&String[ 32 ], Message, ::strlen(Message));

    reportf("Save record failed: %s\n", &String[16]);
}

void SaveRec( char* Command, char* Reply )
{
	int CheckMaskResult, SecurityResult;
	char DuffAnswer[ MAXBUFF ];

	reportf( "Save Record request received.\n" );
	memcpy( &Reply[ 16 ], SAVESUCC, 2 );
	memcpy( &Reply[ 18 ], &Command[ 18 ], 14 );

	if(( SecurityResult = ViolateSec( Command )) != 0 )
	{
	  reportf("ViolateSec error %s%d\n", "", SecurityResult);
	  
		CloseRec( Command, DuffAnswer );
		switch( SecurityResult )
		{
		case 1:
			CreateSaveRecFail( Reply, ACCCIOLFAIL );
			break;
		case 2:
			CreateSaveRecFail( Reply, ACCESSVIOLATION );
			break;
		}
		return;
	}
	else
	if(( CheckMaskResult = CheckMask( Command )) != 0 )
	{
	  reportf("CheckMask error\n");
	  
		switch( CheckMaskResult )
		{
		case 1:
			CreateSaveRecFail( Reply, SR_INDEXOPENFAIL );
			break;
		case 2:
			CreateSaveRecFail( Reply, SR_NOTLOCKOWNER );
			break;
		case 3:
			CreateSaveRecFail( Reply, SR_RECNOTFOUND );
			break;
		case 4:
			CreateSaveRecFail( Reply, OR_GETLOCKFAILED );
			break;
		case 5:
			CreateSaveRecFail( Reply, SR_OPENDATAFAIL );
			break;
		case 6:
			CreateSaveRecFail( Reply, SR_FAILEDTOLOCATE );
			break;
		case 7:
			CreateSaveRecFail( Reply, SR_DATALOCKFAIL );
			break;
		case 8:
			CreateSaveRecFail( Reply, SR_INDEXLOCKFAIL );
			break;
		case 9:
			CreateSaveRecFail( Reply, OR_ENTRYDELETED );
			break;
		case 10:
			CreateSaveRecFail( Reply, SR_GENERALFAULT );
			break;
		case 11:
			CreateSaveRecFail( Reply, ALREADYOPEN );
			break;
		}
		return;
	}
	else
	{
#ifdef VERY_VERBOSE	    
	  reportf("Calling store\n");
#endif	  
	  switch( DBStore( &Command[ 23 ] ))
	  {
	  case 0:
#ifdef VERY_VERBOSE	      
	    reportf("Stored ok\n");
#endif	    
	    return;
	  case 1:
	    CreateSaveRecFail( Reply, SR_INDEXLOCKFAIL );
	    return;
	  case 2:
	  case 9:
	    CreateSaveRecFail( Reply, SR_INDEXOPENFAIL );
	    return;
	  case 3:
	    CreateSaveRecFail( Reply, SR_RECNOTFOUND );
	    return;
	  case 4:
	    CreateSaveRecFail( Reply, SR_NOTLOCKOWNER );
	    return;
	  case 5:
	    CreateSaveRecFail( Reply, SR_DATALOCKFAIL );
	    return;
	  case 6:
	    CreateSaveRecFail( Reply, SR_OPENDATAFAIL );
	    return;
	  case 7:
	    CreateSaveRecFail( Reply, SR_FAILEDTOLOCATE );
	    return;
	  case 8:
	    CreateSaveRecFail( Reply, SR_WRITEFAIL );
	    return;
	  case 10:
	    CreateSaveRecFail( Reply, SR_INDEXMODFAIL );
	    return;
	  case 11:
	    CreateSaveRecFail( Reply, SR_NORECOPEN );
	    return;
	  case 20:
	    CreateSaveRecFail(Reply, ACTIONFAILURE);
	    return;
	    break;
	  case 21:
	    CreateSaveRecFail(Reply, SR_INVALIDDATA);
	    return;
	    break;
	  case 30:
	    CreateSaveRecFail(Reply, SERVERFAILURE);
	    return;
	    break;
	  case 31:
	    CreateSaveRecFail(Reply, SR_READONLY);
	    return;
	    break;
	  default:
	    CreateSaveRecFail( Reply, SR_GENERALFAULT );
	    return;
	  }
	}
}

