/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Populator.cc,v 1.1 1998/07/23 10:47:27 nmcl Exp $
 */

#include <string.h>
#include <iostream.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>

#include <Stat/Variance.h>

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#include "BloomFilterNameService.h"

unsigned long entries = 1000000;
unsigned long hashSize = 40000;
int nhash = 5;
int nodes = 8;

/*
 * This program uses the BloomFilterNameService implementation to insert
 * the specified number of entries into the specified number of simulated
 * hosts. It then obtains the average time taken to obtain a "hit", and
 * the average number of false hits.
 */

int main (int argc, char** argv)
{
    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-hashSize") == 0)
	    hashSize = ::atol(argv[i+1]);
	if (::strcmp(argv[i], "-entries") == 0)
	    entries = ::atol(argv[i+1]);
	if (::strcmp(argv[i], "-nodes") == 0)
	    nodes = ::atoi(argv[i+1]);	
	if (::strcmp(argv[i], "-nhash") == 0)
	    nhash = ::atoi(argv[i+1]);
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " [-nhash <number>] [-hashSize <size>] [-entries <number>] [-nodes <number>] [-help]" << endl;
	    return 0;
	}
    }

    BloomFilterNameService nameService(nodes, hashSize, nhash);
    
    if (entries%nodes != 0)   // make sure we can evenly distribute objects
	entries = entries + (nodes-entries%nodes);

    struct timeval stime, ftime;
    Variance v;
    
    for (int l = 0; l < 100; l++)
    {
	nameService.reset();
	
	::gettimeofday(&stime, NULL);
	
	for (int k = 0; k < nodes; k++)
	{
	    if (nameService.addDomain("", k))
	    {
		for (int m = 0; m < entries/nodes; m++)
		{
		    Uid u;
		    
		    nameService.insert(u, k);
		}
	    }
	    else
	    {
		cerr << "Error adding domain " << k << endl;
		return -1;
	    }
	}
	
	::gettimeofday(&ftime, NULL);

	long msec = ((ftime.tv_sec*1000000) + ftime.tv_usec) -
	    ((stime.tv_sec*1000000) + stime.tv_usec);

	v += (double) msec/ (double) 1000;
    }

    cout << "Statistics on population time are:\n" << endl;
    
    cout << "Mean number of hits: " << v.mean() << endl;
    cout << "Max number of hits: " << v.max() << endl;
    cout << "Min number of hits: " << v.min() << endl;
    cout << "Variance: " << v.variance() << endl;
    cout << "StdDev: " << v.stdDev() << "\n" << endl;    

    return 0;
}

