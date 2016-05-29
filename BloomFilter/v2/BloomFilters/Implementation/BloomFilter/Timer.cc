/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Timer.cc,v 1.2 1998/07/23 10:47:57 nmcl Exp $
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

    Uid** u = new Uid* [entries];
    long index = 0;
    
    for (int k = 0; k < nodes; k++)
    {
	int domainNumber = 0;

	if (nameService.addDomain("", domainNumber))
	{
	    for (int m = 0; m < entries/nodes; m++)
	    {
		u[index] = new Uid;
		index++;
		
		nameService.insert(*u[index], domainNumber);
	    }	    
	}
	else
	{
	    cerr << "Error adding domain " << k << endl;
	    return -1;
	}
    }    

    struct timeval stime, ftime;
    Variance v;
    long long currentTime = 0;
    
    index = 0;
    
    /*
     * Now go back through all of the entries we have just added and
     * see how many "hits" we get for each.
     */
    
    for (int j = 0; j < entries; j++)
    {
	int size = 0;

	::gettimeofday(&stime, NULL);
	
	char** iors = nameService.locate(*u[index], size);

	::gettimeofday(&ftime, NULL);

	currentTime += ((ftime.tv_sec*1000000)+ftime.tv_usec) -
	    ((stime.tv_sec*1000000)+stime.tv_usec);
	
	if (iors == NULL)
	{
	    cerr << "Error - could not find uid " << *u[index] << endl;
	    return -2;
	}
	
	v += size;

	for (int l = 0; l < size; l++)
	    ::delete [] iors[l];

	::delete [] iors;
    }

    cout << "Average time to lookup " << entries << " entries is: "
	 << (double) (currentTime/(double) entries)/1000
	 << " milliseconds" << endl;

    cout << "Statistics on hit probability are:\n" << endl;
    
    cout << "Mean number of hits: " << v.mean() << endl;
    cout << "Max number of hits: " << v.max() << endl;
    cout << "Min number of hits: " << v.min() << endl;
    cout << "Variance: " << v.variance() << endl;
    cout << "StdDev: " << v.stdDev() << "\n" << endl;    

    for (int l = 0; l < entries; l++)
    {
	if (u[l] != NULL)
	    delete u[l];

	delete [] u;
    }
    
    return 0;
}

