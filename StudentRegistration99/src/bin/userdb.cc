#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ndbm.h>
#include <fcntl.h>
#include <errno.h>

#include "local.h"
#include "userdb.h"
#include "Defs.h"

#define LASTRDWROPTION 7
#define LASTVIEWOPTION 5
#define MINPASSLEN 8

#define CMDPROMPT "Command? "
#define CANCEL "Cancel"

#define DEFAULTDEPT "XXXX"

#define ADDUSER "add"
#define DELUSER "del"
#define MODUSER "mod"
#define LISTUSER "list"
#define HELP "help"
#define EXIT "exit"

#define NULLPASSWORD "0101010101010101"

void PrintRecord( char* UserID, struct DBEntry* anEntry )
{
	char OutputString[50], NullString[16];

	memset(OutputString, '\0', 50);
	memset(NullString, 0, 16);

	/* Copy each item to the OuputString, print the information
	   then reset the OutputString to null */
	memcpy(OutputString, UserID, 8);
	printf("User details for : %s\n", OutputString);
	memset(OutputString, '\0', 50);

	memcpy(OutputString, anEntry->RealUserName, 40);
        printf("\tRealname : %s", OutputString);
	memset(OutputString, '\0', 50);

	memcpy(OutputString, anEntry->Department, 4);
        printf("\tDepartment : %s\n",OutputString);
	memset(OutputString, '\0', 50);
	
	memcpy(OutputString, anEntry->ContactNumber, 10);
	printf("\tContact Number : %s", OutputString);

	/* Check if the password item is blank and print approriate message */
	if (memcmp(anEntry->PasswordKey, NULLPASSWORD, 16) == 0) 
		printf("\tPassword is undefined\n");
	else
		printf("\tPassword is defined\n");
	
        printf("\tRead / Write parameter : %i", anEntry->RDWRLevel);
	printf("\tView parameter : %i\n\n\n", anEntry->ViewLevel);
}

int TryLine( char* Prompt, char* String, int Length )
{
	char InputString[ 256 ];
	int NameInputLoop = 0;

	memset( InputString, 0, 256 );
	memset( String, 0, Length );

	printf( "%s", Prompt );

	if( (InputString[ NameInputLoop ] = getc( stdin )) != EOF )
		while( InputString[ NameInputLoop ] != EOF
		&& InputString[ NameInputLoop ++ ] != '\n'
		&& NameInputLoop != sizeof( InputString ))
			InputString[ NameInputLoop ] = getc( stdin );

	if( InputString[ NameInputLoop - 1] == '\n' )
		InputString[ NameInputLoop - 1 ] = '\0';
 
	if( strlen( InputString ) > (unsigned int) Length )
	{
		printf( "The maximum characters allowed is %i\n", Length );
		printf( "Can you please re-enter\n\n" );
		return FALSE ;
	}
 
	memcpy( String, InputString, Length );
	return ( InputString[ 0 ] == EOF ? EOF : TRUE );
}

void GetText( char* Prompt, char* String, int Length, int CanDefault )
{
	int ReturnResult;

	if( CanDefault == TRUE )
	  while( TryLine( Prompt, String, Length ) == FALSE );
	else
	  while(( ReturnResult = TryLine( Prompt, String, Length )) == FALSE ||
		ReturnResult == TRUE && String[ 0 ] == '\0' );
}

void DoAddUser( DBM* DBfd )
{
	char UserID[ 9 ], OutputString[ 256 ], TempString[ 50 ], InputString[ 80 ];
	struct DBEntry anEntry;
	datum key, data;
	int Value;

	memset( OutputString, 0, 256 );
	memset( UserID, 0, 9 );
	memset( &anEntry, 0, sizeof( anEntry ));

	GetText( "What is the id of the person you wish to add? ", UserID, 8, FALSE );
	printf( "\n" );

	if( UserID[ 0 ] == EOF )
	{
		printf( "%s\n", CANCEL );
		return;
	}

	key.dptr = UserID;
	key.dsize = 8;

	data = dbm_fetch( DBfd, key );

	if( data.dptr != NULL )
	{
		printf( "A user with this id already exists.\n\n" );
		return;
	};

	GetText( "What is the real name of this user? ", anEntry.RealUserName, 40, TRUE );
	printf( "\n" );

	if( anEntry.RealUserName[ 0 ] == EOF )
	{
		printf( "%s\n", CANCEL );
		return;
	}

	memset( TempString, 0, 50 );
	memcpy( TempString, anEntry.RealUserName, 40 );
	(void)sprintf( OutputString, "What is %s's department code[ %s ]? ", TempString, DEFAULTDEPT );
	GetText( OutputString, anEntry.Department, 4, TRUE );
	printf( "\n" );

	if( anEntry.Department[ 0 ] == EOF )
	{
		printf( "%s\n", CANCEL );
		return;
	}

	if( anEntry.Department[ 0 ] == '\0' && !feof( stdin ))
		memcpy( anEntry.Department, DEFAULTDEPT, 4 );

	memset( TempString, 0, 50 );
	memcpy( TempString, anEntry.RealUserName, 40 );
	(void)sprintf( OutputString, "What is %s's extension? ", TempString );
	GetText( OutputString, anEntry.ContactNumber, 4, TRUE);
	printf( "\n" );

	if( anEntry.ContactNumber[ 0 ] == EOF )
	{
		printf( "%s\n", CANCEL );
		return;
	}

	memset( InputString, 0, 80 );

	while( strlen( InputString ) < MINPASSLEN )
	{
	  memset( InputString, 0, 80 );
	  
		GetText( "What is the initial password for this user? ", InputString, 80, TRUE );

		if( InputString[ 0 ] == EOF )
		{
			printf( "%s\n", CANCEL );
			return;
		}

		if( InputString[ 0 ] == '\0' )
		{
			printf( "WARNING : Password hasn't been defined.\n" );
			break;
		}

		if( strlen( InputString ) < 8 )
			printf( "\nMinimum number of characters to a password is %i\nPlease re-enter\n\n", MINPASSLEN );
	}

	memset( TempString, 0, 50 );
	memset( anEntry.PasswordKey, 0, 16 );

	strongk( InputString, TempString );
	BytesToHexString( TempString, anEntry.PasswordKey, 8 );
	printf( "\n" );

	Value = -1;
	while( Value < 1 || Value > LASTRDWROPTION )
	{
	  memset( InputString, 0, 80 );
	  
		GetText( "What should the Read/Write parameter be set to? [ 1 ] ", InputString, 1, TRUE );

		if( InputString[ 0 ] == EOF )
		{
			printf( "%s\n", CANCEL );
			return;
		};
	
		if( InputString[ 0 ] == '\0' )
			Value = 1;
		else
			Value = atoi( InputString );

		if( Value < 1 || Value > LASTRDWROPTION )
			printf( "The value entered must be within the range 1 and %i.\n", LASTRDWROPTION );
	}
	anEntry.RDWRLevel = Value;

	Value = -1;
	while( Value < 1 || Value > LASTVIEWOPTION )
	{
	  memset( InputString, 0, 80 );
	  
		GetText( "What should the View parameter be set to? [ 1 ] ", InputString, 1, TRUE );
		if( InputString[ 0 ] == EOF )
		{
			printf( "%s\n", CANCEL );
			return;
		};

		if( InputString[ 0 ] == '\0' )
			Value = 1;
		else
			Value = atoi( InputString );

		if( Value < 1 || Value > LASTVIEWOPTION )
			printf( "The value entered must be within the range 1 and %i.\n", LASTVIEWOPTION);
	}
	anEntry.ViewLevel = Value;

	printf( "\nI'm going to install a user with the following details:\n" );
	PrintRecord( UserID, &anEntry );
	printf( "\n" );

	GetText( "Is this correct? Y/N ", TempString, 1, FALSE );
	printf( "\n" );

	if( TempString[ 0 ] == EOF )
	{
		printf( "%s\n", CANCEL );
		return;
	}

	if( tolower( TempString[ 0 ] ) == 'y' )
	{
		key.dptr = UserID;
		key.dsize = 8;
		data.dptr = (char *)&anEntry;
		data.dsize = sizeof( anEntry );

		if( dbm_store( DBfd, key, data, DBM_INSERT ) == 1 )
		{
			printf( "Somehow the record now exists.\n" );
			printf( "Add new user failed.\n\n" );
		}
		else
			printf( "Add new user succeeded.\n\n" );
	}
	else
		printf( "Add new user canceled.\n\n" );

	return;
}

void DoDelUser( DBM* DBfd )
{
	datum key, data;
	char UserID[ 9 ], Sure;
	struct DBEntry anEntry;

	memset( UserID, 0, 9 );
	GetText( "What is the id of the person you wish to delete? ", UserID, 8, FALSE );
	printf( "\n" );

	if( UserID[ 0 ] == EOF )
	{
		printf( "%s\n", CANCEL );
		return;
	}

	printf( "\n" );
	key.dptr = UserID;
	key.dsize = 8;
	data = dbm_fetch( DBfd, key );

	if( data.dptr == NULL )
	{
		printf( "This identifier isn't in the database.\n" );
		return;
	}

	memset( &anEntry, 0, sizeof( anEntry ));
	memcpy( &anEntry, data.dptr, sizeof( anEntry ));
	PrintRecord( key.dptr, &anEntry );

	Sure = 'X';
	while( toupper( Sure ) != 'Y' && toupper( Sure ) != 'N' && Sure != EOF )
		GetText( "Are you sure you wish to delete this id? (y/n)", &Sure, 1, TRUE );
	printf( "\n" );

	if( Sure == EOF )
	{
		printf( "%s\n", CANCEL );
		return;
	}
	else
	  printf( "\n" );

	if( toupper( Sure ) == 'N' )
		return;

	key.dptr = UserID;
        key.dsize = 8;

	if( dbm_delete( DBfd, key) == -1 )
	{
		printf( "Failed to delete entry in database.\n" );
		return;
	}
	else
	{
		printf( "Record deleted.\n" );
		return;
	}
}

void DoModUser( DBM* DBfd )
{
	char UserID[ 9 ], InputChar, InputLine[ 40 ], TempString[ 8 ];
	int Value;
	datum key, data;
	struct DBEntry anEntry;

	fprintf( stderr, "DoModUser.\n" );

	GetText( "What is the id of the person you wish to modify? ", UserID, 8, TRUE );
	printf( "\n" );

	if( UserID[ 0 ] == EOF )
	{
		printf( "%s\n", CANCEL );
		return;
	}

	if( UserID[ 0 ] == '\0' )
		return;

	key.dptr = UserID;
	key.dsize = 8;

	data = dbm_fetch( DBfd, key );

	if( data.dptr == NULL )
	{
		printf( "There is no entry in the database for list id.\n" );
		return;
	}

	memset( &anEntry, 0, sizeof( anEntry ));
	memcpy( &anEntry, data.dptr, sizeof( anEntry ));

	InputChar = 'X';

	while( toupper( InputChar ) != 'F' )
	{

		PrintRecord( UserID, &anEntry );

		GetText( "Modify field? R/D/C/W/V/S/P F/E ", &InputChar, 1, FALSE );
		printf( "\n" );

		if( InputChar != EOF )
		switch( toupper( InputChar ) )
		{
		case 'R':
                        memset( InputLine, 0, 40 );
			GetText( "Change the user's real name to: ", InputLine, 40, TRUE );
			printf( "\n" );

			if( InputLine[ 0 ] == EOF )
			{
				printf( "%s\n", CANCEL );
				InputChar = 'F';
				break;
			}

			if( InputLine[ 0 ] == '\0' )
			{
				printf( "Realname not replaced.\n\n" );
				break;
			}

			memset( anEntry.RealUserName, 0, 40 );
			memcpy( anEntry.RealUserName, InputLine, 40 );
			break;
		case 'D':
                        memset( InputLine, 0, 40 );
			GetText( "Change the user's department to: ", InputLine, 4, TRUE );
			printf( "\n" );

			if( InputLine[ 0 ] == EOF )
			{
				printf( "%s\n", CANCEL );
				InputChar = 'F';
				break;
			}

			if( InputLine[ 0 ] == '\0' )
			{
				printf( "Department not replaced.\n\n" );
				break;
			}
			
			memset( anEntry.Department, 0, 4 );
			memcpy( anEntry.Department, InputLine, 4 );
			break;
		case 'C':
			memset( InputLine, 0, 40 );
			GetText( "Change the user's contact number to: ", InputLine, 10, TRUE );
			printf( "\n" );

			if( InputLine[ 0 ] == EOF )
			{
				printf( "%s\n", CANCEL );
				InputChar = 'F';
				break;
			}

			if( InputLine[ 0 ] == '\0' )
			{
				printf( "Contact number not replaced.\n\n" );
				break;
			}
			
			memset( anEntry.ContactNumber, 0, 10 );
			memcpy( anEntry.ContactNumber, InputLine, 10 );
			break;
		case 'W':
			memset( InputLine, 0, 40 );

			printf( "Possible values for the Read/Write access levels are:\n" );
			printf( "\t1\tRead only.\n" );
			printf( "\t2\tWrite if department matches during registration.\n" );
			printf( "\t3\tWrite if department matches.\n" );
			printf( "\t4\tWrite during registration.\n" );
			printf( "\t5\tWrite always.\n" );
			printf( "\t6\tCreate and Write always.\n" );
			printf( "\t7\tCreate, Write and delete always.\n\n" );

			GetText( "What is the new Read / Write parameter? ", InputLine, 1, TRUE );
			printf( "\n" );

			if( InputLine[ 0 ] == EOF )
			{
				printf( "%s\n", CANCEL );
				InputChar = 'F';
				break;
			}

			if( InputLine[ 0 ] == '\0' )
			{
				printf( "Read / Write parameter not replaced.\n\n" );
				break;
			}

			Value  = atoi( InputLine );
			if( Value < 1 || Value > LASTRDWROPTION )
			{
				printf( "You can only have a number between 1 and %i.\n", LASTRDWROPTION );
				printf( "Read / Write parameter has been left unchanged.\n\n" );
				break;
			}

			anEntry.RDWRLevel = Value;

			break;
		case 'V':
			memset( InputLine, 0, 40 );

			printf( "Possible values for the View parameter are:\n" );
			printf( "\t1\tAlways restricted.\n" );
			printf( "\t2\tFull if department match during registration.\n" );
			printf( "\t3\tFull if department match.\n" );
			printf( "\t4\tFull during registration.\n" );
			printf( "\t5\tAlways full.\n");

			GetText( "What is the new View parameter? ", InputLine, 1, TRUE );
			printf( "\n" );

			if( InputLine[ 0 ] == EOF )
			{
				printf( "%s\n", CANCEL );
				InputChar = 'F';
				break;
			}
			if( InputLine[ 0 ] == '\0' )
			{
				printf( "View parameter not replaced.\n" );
				break;
			}

			Value = atoi( InputLine );
			if( Value < 1 || Value > LASTVIEWOPTION )
			{
				printf( "You can only have a number between 1 and %i.\n", LASTVIEWOPTION );
				printf( "View parameter not replaced.\n" );
				break;
			}

			anEntry.ViewLevel = Value;
			break;
		case 'F':
			key.dptr = UserID;
			key.dsize = 8;

			data.dptr = (char *)&anEntry;
			data.dsize = sizeof( anEntry );

			if( dbm_store( DBfd, key, data, DBM_REPLACE ) == -1 )
			{
				perror( "Failed to update record" );
			}
			else
				printf( "Record has been saved.\n" );

			break;
		case 'E':
			printf( "Record has NOT been saved.\n" );
			InputChar = 'F';
			break;
		case 'S':
			break;
		case 'P':
			memset( InputLine, 0, 40 );
			GetText( "What is the new password? ", InputLine, 40, TRUE );

			if( InputLine[ 0 ] == EOF )
			{
				printf( "%s\n", CANCEL );
				return;
			};

			if( InputLine[ 0 ] == '\0' )
			{
				printf( "\nPassword has been left unchanged.\n\n" );
				break;
			}

			if( InputLine[ 0 ] == '-' )
			{
				printf( "\nPassword has been blanked.\n" );
				memset( anEntry.PasswordKey, 0, 16 );
				break;
			}

			strongk( InputLine, TempString );
			BytesToHexString( TempString, anEntry.PasswordKey, 8 );
			break;
		default:
			printf( "Command not understood.\n" );
			printf( "Please type the character within the ()'s to select the listed command.\n" );
			printf( "\t(R)eal - change the real name of a user.\n" );
			printf( "\t(D)ept - change the department of a user.\n" );
			printf( "\t(C)ontact  - change the contact number of a user.\n" );
			printf( "\t(W)rite - change the Read/Write parameter for a user.\n" );
			printf( "\t(V)iew - change the View parameter for a user.\n" );
			printf( "\t(S)how - show the current configuration.\n\n" );
			printf( "\t(F)inish - save the current settings and exit modification.\n" );
			printf( "\t(E)xit - exit modiciation without saving the data.\n" );
			printf( "\n" );
			break;
		}
		else
			InputChar = 'F';

	}

	printf( "\n" );

}

void DoListUser( DBM* DBfd )
{
	char InputLine[ 80 ];
	datum key, data;

	key = dbm_firstkey( DBfd );
	if( key.dptr == NULL )
	{
		printf( "Database is empty.\n\n" );
		return;
	}

	GetText( "Which user id do you wish to see? (return is all) ", InputLine, 8, TRUE );

	if( InputLine[ 0 ] == EOF )
	{
		printf( "%s\n", CANCEL );
		return;
	};

	if( InputLine[ 0 ] == '\0' )
	for( ; key.dptr != NULL; key = dbm_nextkey( DBfd ) )
	{
		struct DBEntry anEntry;

		data = dbm_fetch( DBfd, key );
		memset( &anEntry, 0, sizeof( anEntry ));
		memcpy( &anEntry, data.dptr, sizeof( anEntry ));

		PrintRecord( key.dptr, &anEntry );
	}
	else
	{
		struct DBEntry anEntry;

		key.dptr = InputLine;
		key.dsize = 8;
		data = dbm_fetch( DBfd, key );

		if( data.dptr == NULL )
		{
			printf( "User identifier not found in database.\n\n" );
			return;
		}

		memset( &anEntry, 0, sizeof( anEntry ));
		memcpy( &anEntry, data.dptr, sizeof( anEntry ));
		PrintRecord( key.dptr, &anEntry );
	}
}
 
void DoHelp()
{
	printf( "The commands are:\n" );
	printf( "\t%s\t Add a user to the database.\n", ADDUSER );
	printf( "\t%s\t Delete a user from the database.\n", DELUSER );
	printf( "\t%s\t Change a user\'s config.\n", MODUSER );
	printf( "\t%s\t List a user or all users within the database.\n", LISTUSER );
	printf( "\t%s\t Get this help page.\n", HELP );
	printf( "\t%s\t Exit this software.\n\n", EXIT );
}

int CountEntries( DBM* DBfd )
{
	int Entries;
	datum key;

	key = dbm_firstkey( DBfd );
	for( Entries = 0; key.dptr != NULL; Entries++ )
		key = dbm_nextkey( DBfd );
	return Entries;
}

void Process( char* DBFilename )
{
	DBM *DBfd;
	int ProcessLoop;
	char InputLine[ 80 ];

	if(( DBfd = dbm_open( DBFilename, O_RDWR | O_CREAT, 0600 )) == NULL )
	{
		perror( "Failed to open database" );
		if( errno == 2 )
		{
			fprintf( stderr, "\nPlease use this command to create the database files.\n\n" );
			fprintf( stderr, "\t\ttouch %s.pag %s.dir\n\n", DBFilename, DBFilename );
		}
		exit( 1 );
	}

	printf( "The user database has %i entr%s.\n\n", CountEntries( DBfd ),
		( CountEntries( DBfd ) == 1 ? "y" : "ies" ));

	memset( InputLine, 0, 80 );

	while( memcmp( InputLine, EXIT, 4 ) != 0 )
	{
		GetText( CMDPROMPT, InputLine, sizeof( InputLine ), TRUE);
		printf( "\n" );

		if( InputLine[ 0 ] == EOF )
		{
			printf( "%s\n", EXIT );
			memcpy( InputLine, EXIT, strlen( EXIT ));
		}

		for( ProcessLoop = 0; ProcessLoop < strlen( InputLine ); ProcessLoop++ )
			InputLine[ ProcessLoop ] = tolower( InputLine[ ProcessLoop ] );

		if( memcmp( InputLine, ADDUSER, strlen( ADDUSER )) == 0 )
			DoAddUser( DBfd );
		else if( memcmp( InputLine, DELUSER, strlen( DELUSER )) == 0 )
			DoDelUser( DBfd );
		else if( memcmp( InputLine, MODUSER, strlen( MODUSER )) == 0 )
			DoModUser( DBfd );
		else if( memcmp( InputLine, LISTUSER, strlen( LISTUSER )) == 0 )
		{
			DoListUser( DBfd );
			continue;
		}
		else if( memcmp( InputLine, EXIT, strlen( EXIT )) == 0 )
			continue;
		else if( memcmp( InputLine, HELP, strlen( HELP )) == 0 )
		{
			DoHelp();
			continue;
		}
		else
			printf( "Command not understood.\n\n" );

		printf( "The user database has %i entr%s.\n\n", CountEntries( DBfd ),
			( CountEntries( DBfd ) == 1 ? "y" : "ies" ));

	}

	dbm_close( DBfd );
}

int main( int argc, char** argv )
{
	char DBFilename[ 256 ];

	memset( DBFilename, 0, 256 );

	if( argc == 1 )
	    memcpy( DBFilename, DBFile, strlen( DBFile ));
	else
	    memcpy( DBFilename, argv[ 2 ], strlen( argv[ 2 ] ));

	Process( DBFilename );

	return 0;
}

