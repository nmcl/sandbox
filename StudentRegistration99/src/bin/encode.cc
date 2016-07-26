#include <string.h>
#include <memory.h>
#include <stdio.h>

#include "Defs.h"

int main( int argc, char** argv )
{
	char Key[ 8 ], Result[ 2048 ], Request[ 1024 ];

	memset( Key, 0, 8 );
	memset( Result, 0, 2048 );
	memset( Request, 0, 1024 );

	if( argc == 2 )
	{
	 	CreateToken( Request );
		memcpy( &Request[ 16 ], argv[ 1 ], strlen( argv[ 1 ] ));
	}
	else
		memcpy( Request, argv[ 1 ], strlen( argv[ 1 ] ));

	desinit( HexStringToBytes( "e3f7529e0ba11040", Key, 8 ));

	EncodeBlock( Request, Result, strlen( Request ));

	printf( "The encode is : \n%s\n", Result );

	return 1;

}

