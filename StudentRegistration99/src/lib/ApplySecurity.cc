#include <System/stdio.h>
#include <System/errno.h>
#include <System/string.h>
#include <System/ndbm.h>
#include <System/fcntl.h>

#include <Common/Boolean.h>

#include "userdb.h"
#include "local.h"
#include "Requests.h"

#ifndef DEFS_H_
#  include "Defs.h"
#endif

extern char *RecordLocked;
extern char *Username;

void DoRDWR( char* Message, char* Dept, int RDWRLevel )
{
	char *RecordStart;

	RecordStart = &( Message[ 23 ] );

	RecordStart[ 831 ] = 'A';

	switch( RDWRLevel )
	{
	case 1:
		break;
	case 2:
		if(( memcmp( &RecordStart[ 806 ], Dept, 4 ) == 0 || memcmp( &RecordStart[ 806 ], "0000", 4) == 0 ) && IsRegistration() )
			RecordStart[ 831 ] = 'B';
		break;
	case 3:
		if( memcmp( &RecordStart[ 806 ], Dept, 4 ) == 0 || memcmp( &RecordStart[ 806 ], "0000", 4) == 0 )
			RecordStart[ 831 ] = 'B';
		break;
	case 4:
		if( IsRegistration() )
			RecordStart[ 831 ] = 'B';
		break;
	case 5:
	case 6:
	case 7:
		RecordStart[ 831 ] = 'B';
		break;
	}
}

void DoView( char* Message, char* Dept, int ViewLevel )
{
	char *RecordStart;
 
	RecordStart = &( Message[ 23 ] );

	switch( ViewLevel )
	{
	case 1:
		break;
	case 2:
		if( memcmp( &RecordStart[ 806 ], Dept, 4 ) == 0 && IsRegistration() )
			RecordStart[ 831 ] += 2;
		break;
	case 3:
		if( memcmp( &RecordStart[ 806 ], Dept, 4 ) == 0 )
			RecordStart[ 831 ] += 2;
		break;
	case 4:
		if( IsRegistration() )
			RecordStart[ 831 ] += 2;
		break;
	case 5:
		RecordStart[ 831 ] += 2;
		break;
	}
}

void ApplyMask( char* Record, char MaskChar )
{
		/* MStat */
	memset( &Record[ 114 ], MaskChar, 1 );
		/* DOB, Nationality, COB, Domicile */
	memset( &Record[ 124 ], MaskChar, 20 );
		/* Fee Stat */
	memset( &Record[ 145 ], MaskChar, 1 );
		/* Occp Code, Home Addr : L1 L2 L3 L4 L5, Home Postcode, Home Phone */
	memset( &Record[ 149 ], MaskChar, 181 );
		/* NOK Surname, NOK Forename, NOK Rel., NOK Addr : L1 L2 L3 L4 L5, NOK Postcode, NOK Phone */
	memset( &Record[ 508 ], MaskChar, 261 );
		/* GAB, Tuition Fee */
	memset( &Record[ 774 ], MaskChar, 15 );
		/* Ethnicity */
	memset( &Record[ 787 ], MaskChar, 1 );
		/* UK County */
	memset( &Record[ 805 ], MaskChar, 1 );
}

void ApplyRestrictions( char* Message )
{
	char *RecordStart;

	RecordStart = &Message[ 23 ];

	switch( RecordStart[ 831 ] )
	{
	case 'A':
	case 'B':
		reportf( "Applying mask to output.\n" );
		ApplyMask( RecordStart, ' ' );
		break;
	case 'C':
	case 'D':
		break;
	}
}

void ApplySecurity( char* Message )
{
	struct DBEntry anEntry;

	GetUser( Username, &anEntry );

	DoRDWR( Message, anEntry.Department, anEntry.RDWRLevel );
	DoView( Message, anEntry.Department, anEntry.ViewLevel );

	ApplyRestrictions( Message );
}



