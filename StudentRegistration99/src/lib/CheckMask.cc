#include <System/stdio.h>
#include <System/errno.h>
#include <System/string.h>

#include <Common/Boolean.h>

#include "userdb.h"
#include "local.h"
#include "Requests.h"

#ifndef DEFS_H_
#  include "Defs.h"
#endif

extern char *RecordLocked;
extern char *Username;
extern char *RecordCopy;

void MergeRecords( char* FullRecord, char* PartRecord )
{
	int TransferLoop;

	fprintf( stderr, "Merging records.\n" );
	ApplyMask( PartRecord, '\0' );
	for( TransferLoop = 0; TransferLoop < RECSIZE; TransferLoop++ )
	PartRecord[ TransferLoop ] = ( PartRecord[ TransferLoop ] == '\0' ?
		FullRecord[ TransferLoop ] : PartRecord[ TransferLoop ] );
}

int CheckMask( char* InputString )
{
	char *RecordStart = &InputString[ 23 ];
	struct DBEntry anEntry;

	GetUser( Username, &anEntry );
	if( anEntry.ViewLevel == 5 )
		return 0;

	if( anEntry.ViewLevel == 4 && IsRegistration() )
		return 0;

	if( anEntry.ViewLevel == 3 && ( memcmp( &RecordCopy[ 806 ], anEntry.Department, 4 ) == 0 || memcmp( &RecordCopy[ 806 ], "0000", 4 ) == 0 ))
		return 0;

	if( anEntry.ViewLevel == 2 && ( memcmp( &RecordCopy[ 806 ], anEntry.Department, 4 ) == 0 || memcmp( &RecordCopy[ 806 ], "0000", 4 ) == 0 ) && IsRegistration() )
	    return 0;

	fprintf( stderr, "Doing merge.\n" );
	MergeRecords( RecordCopy, RecordStart );
	return 0;
}

