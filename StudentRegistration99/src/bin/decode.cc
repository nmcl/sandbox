#include <string.h>
#include <memory.h>
#include <stdio.h>

#include "Defs.h"

int main( int , char** argv )
{
	char Key[ 8 ], Result[ 2048 ];

	memset( Key, 0, 8 );
	memset( Result, 0, 2048 );

	desinit( HexStringToBytes( "e3f7529e0ba11040", Key, 8 ));

	printf( "%s\n", argv[ 1 ] );
	DecodeBlock( argv[ 1 ], Result, strlen( argv[ 1 ] ));

	printf( "The decode is : %s\n", Result );

	return 1;

}

