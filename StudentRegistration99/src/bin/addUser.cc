
#include <stdio.h>

main( argc, argv )
int argc;
char **argv;
{
	if( AddUser( argv[ 1 ], argv[ 2 ] ) == 1 )
		printf( "Already there.\n" );
	else
		printf( "Done.\n" );
	return 0;
}

