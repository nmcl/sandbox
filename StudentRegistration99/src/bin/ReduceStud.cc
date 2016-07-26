#include <System/string.h>
#include <System/unistd.h>
#include <System/stdlib.h>
#include <System/stdio.h>

int main()
{
	char Record[ 1025 ];

	while( read( 0, Record, 1025 ) == 1025 )
		write( 1, Record, 1024 );

	return 0;

}

