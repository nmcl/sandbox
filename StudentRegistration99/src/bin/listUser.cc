
#include <stdio.h>

main( int argc, char** argv )
{
	char Result[ 17 ];

	memset( Result, 0, 17 );

	if( ResolveKey( argv[ 1 ], Result ) == -1 )
		printf( "Not there.\n" );
	else
		printf( "%s : %s\n", argv[ 1 ], Result );
	return 0;
}

