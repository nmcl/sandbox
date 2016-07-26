#include <System/string.h>
#include <System/memory.h>

#include "Requests.h"

#ifndef DEFS_H_
#  include "Defs.h"
#endif

extern char *RecordLocked;
extern char *Username;
extern char *Department;

void CreateNewRecFail( char* String, char* Message )
{
    memcpy( &String[ 16 ], NEWRECFAIL, 2 );
    ::memcpy(&String[ 32 ], Message, ::strlen(Message));
    
    reportf("Create record failed: %s\n", &String[16]);
}

void NewRec( char* Command, char* Reply )
{
	reportf( "NewRec request received.\n" );
	memcpy( &Reply[ 16 ], NEWRECSUCC, 2 );
	memcpy( &Reply[ 18 ], &Command[ 18 ], 14 );

	if( ViolateSec( Command ) != 0 )
	{
		CreateNewRecFail( Reply, ACCESSVIOLATION );
		return;
	}

#ifdef VERY_VERBOSE
	reportf("Calling DBNew\n");
#endif	
	
	switch( DBNew( &Command[ 23 ], Reply ))
	{
		case 0:
			break;
		case 1:
			CreateNewRecFail( Reply, NR_INDEXLOCKFAIL );
			break;
		case 2:
			CreateNewRecFail( Reply, NR_INDEXOPENFAIL );
			break;
		case 3:
			CreateNewRecFail( Reply, NR_BASEFINDFAIL );
			break;
		case 4:
			CreateNewRecFail( Reply, NR_BASESTORFAIL );
			break;
		case 5:
			CreateNewRecFail( Reply, NR_INDEXLOCKFAIL );
			break;
		case 6:
			CreateNewRecFail( Reply, NR_INDEXOPENFAIL );
			break;
		case 7:
			CreateNewRecFail( Reply, NR_NEWRECFOUND );
			break;
		case 8:
			CreateNewRecFail( Reply, NR_BASEFINDFAIL );
			break;
		case 9:
			CreateNewRecFail( Reply, NR_BASESTORFAIL );
			break;
		case 10:
			CreateNewRecFail( Reply, NR_DATALOCKFAIL );
			break;
		case 11:
			CreateNewRecFail( Reply, NR_DATAOPENFAIL );
			break;
		case 12:
			CreateNewRecFail( Reply, NR_DATASEEKFAIL );
			break;
		case 13:
			CreateNewRecFail( Reply, NR_FAILWHILWRIT );
			break;
		case 14:
			CreateNewRecFail( Reply, NR_BASESTORFAIL );
			break;
		case 15:
			CreateNewRecFail( Reply, ALREADYOPEN );
			break;
	        case 16:
		        CreateNewRecFail(Reply, NR_POOLEXHAUSTED);
			break;
	        case 20:
		        CreateNewRecFail(Reply, ACTIONFAILURE);
			break;
	        case 24:
		        CreateNewRecFail(Reply, NR_HOSTFAILURE);
			break;
	        case 25:
		        CreateOpenRecFail(Reply, OR_SERVERFAILURE);
			return;
			break;
	        case 26:
		        CreateOpenRecFail(Reply, OR_SERVERACTIVE);
			return;
			break;
	        case 27:
		        CreateNewRecFail(Reply, NR_ATOMICINTERROR);
			return;
			break;
	        case 28:
		        CreateNewRecFail(Reply, NR_INDEXACTIONFAILURE);
			return;
			break;
	        case 29:
		        CreateNewRecFail(Reply, NR_INVALIDNUMBER);
			return;
			break;
	        case 30:
		        CreateNewRecFail(Reply, SERVERFAILURE);
			return;
			break;
	        case 31:
		        CreateNewRecFail(Reply, VIRTUALSERVERFAILURE);
			return;
			break;
	        case 32:
		        CreateNewRecFail(Reply, NR_ISDUMPING);
			return;
			break;			
	        case 33:
		        CreateNewRecFail(Reply, NR_RECORDEXISTS);
			return;
			break;
	        case 34:
		        CreateNewRecFail(Reply, NR_NONEWMAP);
			return;
			break;
	        default:
			CreateNewRecFail( Reply, NR_GENERALFAIL );
	}
}

