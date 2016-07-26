#include <System/stdio.h>
#include <System/string.h>
#include <System/stdio.h>
#include <System/stdlib.h>
#include <System/unistd.h>
#include <System/iostream.h>

#include <local.h>
#include <Defs.h>

/*
for a typical student number of 
	968045259 we have
	----
	base address to start the numbers
	    ----
	    start/end section of the numbers
		-
		check digit added by program
*/

/* BASEADDRESS = first 4 digits of the student number */
#define BASEADDRESS	"9882"

/* STARTNUMBER = next 4 digits of first student number */
#define STARTNUMBER	4525

/* ENDNUMBER = next 4 digits of last student number */
#define ENDNUMBER	4530

const char* base = BASEADDRESS;

char *RecordCopy;
char *RecordLocked;

void AddNumb( char* Record, int Number )
{
	char theNumber[9];
	char mult[ 8 ];
	int Counter, Total;

	memcpy( mult, "13791379", 8 );
	memset( theNumber, 0, 9 );
	memcpy( theNumber, BASEADDRESS, 4);

	theNumber[ 4 ] = (Number / 1000) + '0';
 	theNumber[ 5 ] = (Number / 100 ) - ((theNumber[4] - '0') * 10) + '0';
	theNumber[ 6 ] = (Number / 10 )  - ((theNumber[5] - '0') * 10 + (theNumber[4] - '0') * 100) + '0';
	theNumber[ 7 ] = Number % 10 + '0';

	Total = 0;
	for( Counter = 0; Counter < 8; Counter++ )
	Total += (theNumber[ Counter ] - '0') * (mult[ Counter ] - '0');

 	Total = (10 - ( Total % 10 )) % 10;
	memcpy( Record, theNumber, 8 );
	Record[ 8 ] = Total + '0';
}


void usage (const char* name)
{
    cout << "Usage: " << name << " [-base <base address>] [-start <start>] [-end <end>] [-number <size>] [-help]" << endl;
}


int main (int argc, char** argv)
{
    int n;
    char Record[ 1025 ];
    int start = STARTNUMBER;
    int end = ENDNUMBER;
    
    for (int i = 1; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    usage(argv[0]);
	    
	    return 0;
	}
	else
	{
	    if (::strcmp(argv[i], "-start") == 0)
	    {
		start = atoi(argv[i+1]);
		i++;
	    }
	    else
	    {
		if (::strcmp(argv[i], "-end") == 0)
		{
		    end = atoi(argv[i+1]);
		    i++;
		}
		else
		{
		    if (::strcmp(argv[i], "-base") == 0)
		    {
			base = argv[i+1];
			i++;
		    }
		    else
		    {
			if (::strcmp(argv[i], "-number") == 0)
			{
			    end = start+atoi(argv[i+1]);
			    i++;
			}
			else
			{
			    cout << "Unknown option " << argv[i] << endl;
			    usage(argv[0]);
			
			    return -1;
			}
		    }
		}
	    }
	}
    }

    memset( Record, 0, 1025 );
    
    for( n = start; n < end; n++ )
    {
	memset( Record, ' ', 1024 );
	CreateBlankRec( Record );
	AddNumb( Record, n );
	printf( "%s\n", Record );
    }
}
