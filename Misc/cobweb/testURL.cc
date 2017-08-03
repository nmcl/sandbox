#include <iostream.h>
#include <string.h>
#include <stdlib.h>

#include "URL.h"

int main (int argc, char **argv)
{
    if (argc != 2)
    {
	cerr << "Usage: " << argv[0] << " url" << endl;
	cerr << "  e.g. " << argv[0]
	     << " http://arjuna.ncl.ac.uk/index.html"
	     << endl;
	exit(1);
    }

    URL a;
    a.parse(argv[1]);

    cout << "Reconstituated URL = " << a << endl;
    
    a.printInternal(cout);
}
