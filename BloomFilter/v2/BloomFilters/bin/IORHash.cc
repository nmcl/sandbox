/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: IORHash.cc,v 1.2 1998/07/17 09:02:45 nmcl Exp $
 */

#include <math.h>
#include <iostream.h>
#include <string.h>
#include <stdlib.h>
#include <strstream.h>
#include <fstream.h>

#include <Stat/PHistogram.h>
#include <ClassLib/Random.h>

#define TRUE 1
#define FALSE 0
#define IOR_SIZE 1024

enum HashType { PJW, RANDOM1, RANDOM2 };

const int numberSize = sizeof(long)*8;  // in bits

ifstream* input = NULL;
PrecisionHistogram* histogram = 0;

char* readFile ()
{
    if (input->peek() != EOF)
    {
	char* buffer = ::new char[IOR_SIZE];
	::memset(buffer, '\0', IOR_SIZE);

	input->getline(buffer, IOR_SIZE);

	return buffer;
    }
    else
	return NULL;
}

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
    char* buffer = readFile();
    
    while (buffer != NULL)
    {
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

	buffer = readFile();	
    }
}

/*
 * This program reads a file of IORs and hashes them using the pjw
 * hash function, and the corresponding value is placed into a histogram.
 * At the end, the histogram contents are output to give an indication of
 * the distribution of hash values, and the effect of the key structure.
 */

int main (int argc, char** argv)
{
    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-file") == 0)
	    input = new ifstream(argv[i+1]);
    }

    if (!input)
    {
	cerr << "Error - no IOR file specified." << endl;
	return -1;
    }
    
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
