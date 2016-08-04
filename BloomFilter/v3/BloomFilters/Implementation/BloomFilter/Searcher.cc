/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Searcher.cc,v 1.1 1998/06/02 16:51:18 nmcl Exp $
 */

#include <System/string.h>
#include <System/iostream.h>
#include <System/stdlib.h>
#include <System/unistd.h>
#include <System/stdio.h>
#include <System/signal.h>
#include <System/sys/time.h>
#include <System/sys/types.h>

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#include "BloomFilter.h"
#include "BloomFilterNameService.h"

unsigned long iterations = 1000000;
unsigned long hashSize = 40000;
unsigned short hash = 5;

int main (int argc, char** argv)
{
    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-table") == 0)
	    hashSize = ::atol(argv[i+1]);
	if (::strcmp(argv[i], "-entries") == 0)
	    iterations = ::atol(argv[i+1]);
	if (::strcmp(argv[i], "-hash") == 0)
	    hash = ::atoi(argv[i+1]);
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " [-hash <number>] [-table <size>] [-entries <number>] [-help]" << endl;
	    return 0;
	}
    }

    BloomFilter* theFilter = new BloomFilter(hashSize, hash);

    for (int k = 0; k < iterations; k++)
    {
	Uid u;

	theFilter->insert(u);
    }

    cout << (*theFilter) << endl;
    
    BloomFilterNameService nameService(1);

    nameService.insert(theFilter, "");

    unsigned long hits = 0;
    
    for (int j = 0; j < iterations; j++)
    {
	Uid u;

	if (nameService.present(u))
	    hits++;
    }

    cout << "Objects inserted: " << iterations << endl;
    cout << "False hits: " << hits << endl;
    
    return 0;
}

