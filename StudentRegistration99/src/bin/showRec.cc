
#include "Requests.h"
#include <System/stdio.h>
#include <System/ndbm.h>
#include <System/fcntl.h>

void NewLine()
{
	write( 1, ".\n", 2 );
}

void WriteField( char* rec, char* Label, int Start, int Length )
{
	write( 1, Label, strlen( Label ));
	write( 1, &rec[ Start ], Length );
}

void OutputRecord( char* rec )
{
	WriteField( rec, "StudNumb:.", 0, 9 );
	WriteField( rec, "..RStat:.", 144, 1 );
	WriteField( rec, "..Sex:.", 115, 1 );
	WriteField( rec, "..MStat:.", 114, 1 );
	WriteField( rec, "..StudType:.", 112, 1 );
	WriteField( rec, "..CrseType:.", 113, 1 );
	NewLine();
	WriteField( rec, "Surname:.", 9, 38 );
	WriteField( rec, "..FullTim:.", 146, 1 );
	WriteField( rec, "..FullYr:.", 147, 1 );
	NewLine();
	WriteField( rec, "Forename:.", 47, 65 );
	NewLine();
	WriteField( rec, "Title:.", 116, 4 );
	WriteField( rec, "..Init:.", 120, 4 );
	WriteField( rec, "..DOB:.", 124, 8 );
	WriteField( rec, "..Ethn:.", 797, 2 );
	WriteField( rec, "..Dis:.", 799, 1 );
	WriteField( rec, "..Dis\'ty:.", 800, 2 );
	NewLine();
	WriteField( rec, "C.of.N:.", 132, 4 );
	WriteField( rec, "..C.of.B:.", 136, 4 );
	WriteField( rec, "..C.of.D:.", 140, 4 );
	WriteField( rec, "..Count:.", 802, 4 );
	WriteField( rec, "..Fee:.", 145, 1 );
	WriteField( rec, "..Res:.", 148, 1 );
	NewLine();
	write( 1, "Home.Addr:......................Term.Addr:", 41 );
	NewLine();
	WriteField( rec, "", 152, 30 );
	WriteField( rec, "..", 330, 30 );
	NewLine();
	WriteField( rec, "", 182, 30 );
	WriteField( rec, "..", 360, 30 );
	NewLine();
	WriteField( rec, "", 212, 30 );
	WriteField( rec, "..", 390, 30 );
	NewLine();
	WriteField( rec, "", 242, 30 );
	WriteField( rec, "..", 420, 30 );
	NewLine();
	WriteField( rec, "", 272, 30 );
	WriteField( rec, "..", 450, 30 );
	NewLine();
	WriteField( rec, "PCode:.", 302, 8 );
	WriteField( rec, ".................PCode:.", 480, 8 );
	WriteField( rec, "..Tel:.", 488, 20 );
	NewLine();
	WriteField( rec, "Tel:.", 310, 20 );
	WriteField( rec, ".......NOKSur:.", 508, 30 );
	NewLine();
	WriteField( rec, "NOK.Addr:.......................NOKFor:.", 538, 30 );
	NewLine();
	WriteField( rec, "", 591, 30 );
	WriteField( rec, "..NOKRel:.", 568, 23 );
	WriteField( rec, "..Occp:.", 149, 3 );
	NewLine();
	WriteField( rec, "", 621, 30 );
	WriteField( rec, "..NOKPCode", 741, 8 );
	WriteField( rec, "..DProg:.", 769, 4 );
	WriteField( rec, "..Stage:.", 773, 1 );
	NewLine();
	WriteField( rec, "", 651, 30 );
	WriteField( rec, "..NOKTel:.", 749, 20 );
	WriteField( rec, "..GAB:.", 774, 6 );
	NewLine();
	WriteField( rec, "", 681, 30 );
	WriteField( rec, "..TuitFee:.", 780, 9 );
	WriteField( rec, "..Tutor:.", 789, 8 );
	NewLine();
	WriteField( rec, "", 711, 30 );
	WriteField( rec, "..TutDept:.", 806, 4 );
	NewLine();
	WriteField( rec, "MOD1:.", 832, 24 );
	WriteField( rec, "..MOD2:.", 916, 24 );
	NewLine();
	WriteField( rec, "", 856, 30 );
	WriteField( rec, "..", 916, 30 );
	NewLine();
	WriteField( rec, "", 886, 30 );
	WriteField( rec, "..", 970, 30 );
	NewLine();
	WriteField( rec, "MOD3:.", 1000, 24 );
	WriteField( rec, "..TStamp:.", 813, 17 );
	NewLine();
}

/*
 * This program *does not* work with the Arjuna registration service.
 */

int main( int argc, char** argv )
{
	DBM *IndexDB;
	FILE *Datafile;
	datum key, data;
	struct DBEntry Entry;
	int InputLoop;
	char Record[ 1024 ], OutputString[ 70 ], KeyString[9];

	memset( Record, 0, 1024 );

	if( argc != 2 )
	{
		fprintf( stderr, "Incorrect number of parameters.\n" );
		return 1;
	};

	memcpy( KeyString, argv[ 1 ], 9 );

	if(( IndexDB = dbm_open( IndexFile, O_RDWR, 0700 )) == NULL )
	{
		perror( "Failed to open the index database" );
		return 1;
	};

	key.dptr = KeyString;
	key.dsize = 9;

	data = dbm_fetch( IndexDB, key );

	if( data.dptr == NULL )
	{
		fprintf( stderr, "Record not found.\n" );
		dbm_close( IndexDB );
		return 1;
	}

	fprintf( stderr, "Length of Entry should be %i.\n", sizeof( Entry ));
	fprintf( stderr, "Length of data is %i.\n", data.dsize );

	memset( &Entry, 0, sizeof( Entry ));
	memcpy( &Entry, data.dptr, sizeof( Entry ));
	dbm_close( IndexDB );

	if(( Datafile = fopen( DataFile, "r" )) == NULL )
	{
		perror( "Failed to open the record.\n" );
		return 1;
	}

	fprintf( stderr, "Record is at position %i.\n", Entry.Position );

	if( fseek( Datafile, Entry.Position, 0 ) == -1 )
	{
		fclose( Datafile );
		perror( "failed to seek to record position" );
		return 1;
	}

	for( InputLoop = 0; InputLoop < RECSIZE; InputLoop++ )
		Record[ InputLoop ] = getc( Datafile );

	fclose( Datafile );

	OutputRecord( Record );

	return 0;
}

