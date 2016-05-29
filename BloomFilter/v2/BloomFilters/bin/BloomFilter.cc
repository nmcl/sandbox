/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BloomFilter.cc,v 1.5 1998/07/23 10:47:58 nmcl Exp $
 */

#include <stdlib.h>
#include <unistd.h>
#include <iostream.h>
#include <string.h>
#include <math.h>

/*
 * This program uses Blooms equations to compute the size of the
 * bit vector given the required probability of false hits.
 */

int main (int argc, char ** argv)
{
    double p = (double) (1.0/16.0); // probability of false positive
    long long n = 50000;            // number of entries
    long long N = 0;                // size of filter
    double T = 2.0;                 // 1/proportion of bits still 0
    double h = 0.0;                 // number of hash functions to apply
    long long b = 19;               // size of entries in bits

    for (int i = 0; i < argc; i++)
    {
        if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " [-n <size>] [-T <time>] [-p <prob>] [-b <bits>] [-help]" << endl;
	    return 1;
	}
	if (::strcmp(argv[i], "-p") == 0)
	    p = (double) ::atof(argv[i+1]);
	if (::strcmp(argv[i], "-n") == 0)
	    n = ::atol(argv[i+1]);
	if (::strcmp(argv[i], "-T") == 0)
	    T = ::atof(argv[i+1]);
	if (::strcmp(argv[i], "-b") == 0)
	    b = ::atol(argv[i+1]);
    }

    double log2 = log(2);
    double log2p = log(p)/log2;
    double log2T1 = log(1.0/T)/log2;
    double log2T2 = log(1.0 - (1.0/T))/log2;
    double log2e = log(exp(1.0))/log2;

    N = (long long) ceil((n*(-log2p)*(log2e/(log2T1*log2T2))));

    cout << "\nProbability of false positive: " << p << endl;
    cout << "Number of entries: " << n << endl;
    cout << "Normalized time measure: " << T << endl;
    cout << "Bit size of entries: " << b << endl;
    
    cout << "\nBloom filter size is " << N << " bits" << endl;

    h = log2p/log2T2;

    cout << "Number of hash functions for filter is " << ceil(h) << endl;
    
    T += 2;  // from example
    
    N = (long long) ceil((n*(b+1)*((T+2)/(T-1))));
    
    cout << "\nTraditional hash size is " << N << " bits" << endl;

    return 0;
}
