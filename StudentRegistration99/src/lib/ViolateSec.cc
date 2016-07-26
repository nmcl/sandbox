#include <System/string.h>
#include <System/stdio.h>

#include "userdb.h"
#include "local.h"
#include "Requests.h"

#ifndef DEFS_H_
#  include "Defs.h"
#endif

extern char *Username;
extern char *RecordCopy;

int ViolateSec( char* Command )
{
	char *opCmd = &Command[ 16 ];
	struct DBEntry anEntry;

	GetUser( Username, &anEntry );

	if( memcmp( opCmd, SAVEREC, 2 ) == 0 )
	{
		if( anEntry.RDWRLevel == 1 )
			return 2;
		else if( anEntry.RDWRLevel == 2 )
		{
			if(( memcmp( &RecordCopy[ 806 ], "0000", 4 ) == 0 ||
			     memcmp( anEntry.Department, &RecordCopy[ 806 ], 4 ) == 0 ) && IsRegistration() )
				return 0;
			else
				return 2;
		}
		else if( anEntry.RDWRLevel == 3 )
		{
			if( memcmp( &RecordCopy[ 806 ], "0000", 4 ) == 0 ||
				memcmp( anEntry.Department, &RecordCopy[ 806 ], 4 ) == 0 )
				return 0;
			else
				return 2;
		}
		else if( anEntry.RDWRLevel == 4 )
		{
			if( IsRegistration() )
				return 0;
			else
				return 2;
		}
		else
		  return 0;
	}
	else if( memcmp( opCmd, NEWREC, 2 ) == 0 )
	{
		if( anEntry.RDWRLevel == 1 || anEntry.RDWRLevel == 2 || anEntry.RDWRLevel == 3
			|| anEntry.RDWRLevel == 4 || anEntry.RDWRLevel == 5 )
			return 2;
		else
			return 0;
	}
	else if( memcmp( opCmd, DELETEREC, 2 ) == 0 )
	{
		if( anEntry.RDWRLevel == 1 || anEntry.RDWRLevel == 2 || anEntry.RDWRLevel == 3
			|| anEntry.RDWRLevel == 4 || anEntry.RDWRLevel == 5 )
			return 2;
		else
			return 0;
	}
	else
		return 0;
}

