/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: UidHash.cc,v 1.3 1998/07/23 10:47:59 nmcl Exp $
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

void pjw ()
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

/*
 * This program hashes a stringified version of the Uid using the pjw
 * hash function, and the corresponding value is placed into a histogram.
 * At the end, the histogram contents are output to give an indication of
 * the distribution of hash values, and the effect of the key structure.
 */

int main (int argc, char** argv)
{
    /*
     * Given the number of bits in the entry, work out
     * the number of random numbers to take.
     */
    
    histogram = new PrecisionHistogram();

    pjw();

    print();

    delete histogram;

    if (input)
	delete input;
    
    return 0;
}
