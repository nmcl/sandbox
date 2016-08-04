/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: UidHash.cc,v 1.1 1998/05/27 11:13:01 nmcl Exp $
 */

#include <math.h>
#include <iostream.h>
#include <string.h>
#include <stdlib.h>
#include <strstream.h>
#include <fstream.h>

#include <Stat/PHistogram.h>
#include <ClassLib/Random.h>

#include <Common/Uid.h>

enum HashType { KNUTH, RANDOM1, RANDOM2 };

const int iters = 10000;

ifstream* input = NULL;
PrecisionHistogram* histogram = 0;

void print ()
{
    for (int i = 0; i < (int) histogram->numberOfBuckets(); i++)
    {
	double name = 0.0;

	if (histogram->bucketName(i, name))
	{
	    double size = 0.0;

	    histogram->sizeByName(name, size);
	    
	    cout << name << " " << size << endl;
	}
    }
}

void knuth ()
{
    for (int i = 0; i < iters; i++)
    {
	Uid u;
	char* buffer = u.stringForm();
	unsigned long g, h = 0;
	const char *p = (const char *)buffer;
	size_t len = ::strlen(buffer);

	if (buffer)
	{
	    while (len-- > 0)
	    {
		h = (h << 4) + (*p);
		if ((g = h & 0xf0000000))
		{
		    h = h ^ (g >> 24);
		    h = h ^ g;
		}
		p++;
	    }
	}
	
	histogram->setValue(h);

	::delete [] buffer;
    }
}


int main (int argc, char** argv)
{
    /*
     * Given the number of bits in the entry, work out
     * the number of random numbers to take.
     */

    histogram = new PrecisionHistogram();

    knuth();

    print();

    delete histogram;

    if (input)
	delete input;
    
    return 0;
}
