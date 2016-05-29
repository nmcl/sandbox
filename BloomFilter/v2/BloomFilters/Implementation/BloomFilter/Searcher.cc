/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Searcher.cc,v 1.3 1998/07/23 10:47:56 nmcl Exp $
 */

#include <string.h>
#include <iostream.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/types.h>

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#include <Stat/Variance.h>

#include "BloomFilterNameService.h"

unsigned long entries = 1000000;
unsigned long hashSize = 40000;
int nhash = 5;
int nodes = 1;
Variance v;

/*
 * This program uses the BloomFilterNameService implementation to insert
 * the specified number of entries into the specified number of simulated
 * hosts. It then checks how many false hits it can obtain by attempting
 * to put in the same number of entries again.
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

    for (int ii = 0; ii < 100; ii++)
    {
	nameService.reset();
	
	for (int j = 0; j < nodes; j++)
	{
	    if (nameService.addDomain("", j))
	    {
		for (int k = 0; k < entries/nodes; k++)
		{
		    Uid u;

		    nameService.insert(u, j);

#ifdef DEBUG		    
		    if (!nameService.present(u))
		    {
			cerr << "Error - cannot find added entry!" << endl;
			::abort();
		    }
#endif		    
		}
	    }
	    else
	    {
		cerr << "Error adding domain " << j << endl;
		return -1;
	    }
	}

#ifdef DEBUG	
	cout << nameService << endl;
#endif	
    
	unsigned long hits = 0;
    
	for (int l = 0; l < entries; l++)
	{
	    Uid u;

	    if (nameService.present(u))
		hits++;
	}

	//#ifdef DEBUG	
	cout << "hits " << hits << endl;
	//#endif	
	
	v += (double) hits/entries;
    }

    cout << "Objects inserted: " << entries << endl;
    cout << "Mean number of false hits: " << v.mean() << endl;
    cout << "Max number of false hits: " << v.max() << endl;
    cout << "Min number of false hits: " << v.min() << endl;
    cout << "Variance: " << v.variance() << endl;
    cout << "StdDev: " << v.stdDev() << "\n" << endl;

    cout << "\n" << v.mean() - v.stdDev() << endl;
    cout << v.mean() << endl;
    cout << v.mean() + v.stdDev() << endl;
    
    return 0;
}
