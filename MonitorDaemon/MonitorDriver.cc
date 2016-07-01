#include <iostream.h>
#include <string.h>
#include <stdlib.h>

#ifndef COMMON_H_
#  include "Common.h"
#endif

#ifndef MONITOR_H_
#  include "Monitor.h"
#endif


int main (int argc, char** argv)
{
    char* outputFile = (char*) 0;
    Monitor* daemon = (Monitor*) 0;
    int iterations = -1;
    
    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " [-help] [-output <output file>] [-iter <iterations>]" << endl;
	    return 0;
	}
	if (::strcmp(argv[i], "-output") == 0)
	    outputFile = argv[i+1];
	if (::strcmp(argv[i], "-iter") == 0)
	    iterations = atoi(argv[i+1]);
    }

    if (outputFile)
	daemon = new Monitor(outputFile);
    else
	daemon = new Monitor;

    if (daemon->monitor(iterations))
    {
	cout << "Monitor completed successfully." << endl;
	return 1;
    }

    cout << "An error occurred while monitoring." << endl;
    return -1;
}
