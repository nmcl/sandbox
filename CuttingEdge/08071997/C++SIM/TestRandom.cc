/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <stdlib.h>	// to get getopt
#include <iostream.h>
#include "Random.h"

void Gen(int count, RandomStream *r, const char *title)
{
    // Create and initialise buckets for a bar graph
    int i;
    int bucket[100];
    for (i=0; i<100; i++) bucket[i]=0;

    // generate numbers and update bucket counts
    for (i=0; i<count; i++) {
	double result = (*r)();
	int idx = result < 0.0 ? 0 : (result > 100.0) ? 100 : (int) result;
	bucket[idx]++;
    }

    // output a title followed by the bucket counts
    cout << endl << '"' << title << '"' << endl;
    for (i=0; i<100; i++)
	cout << i << ".0 " << bucket[i] << endl;

cerr << "Chi-Square error measure: " << r->Error() << endl;
}

int main (int argc, char **argv)
{
    extern char *optarg;
    extern int optind;
    int c;
    int count=10000;
    int errflg = 0;
    int uniform=0, normal=0, exponential=0, hyperexponential=0, erlang=0;
    int skip=0;

    while ((c = getopt(argc, argv, "s:c:unxhe")) != -1)
        switch(c) {
        case 'c': count = atoi(optarg); break;
        case 'u': uniform++; break;
        case 'n': normal++; break;
        case 'x': exponential++; break;
        case 'h': hyperexponential++; break;
        case 'e': erlang++; break;
        case 's': skip = atoi(optarg); break;

        case '?':
        default: errflg++;
        }

    // ASSERT: argc-optind == number of arguments remaining
    //         argv[optind] == first non-flag argument

    if (errflg || (argc-optind)) {
        cerr << "usage: " << argv[0] << "[-c <number>][-u][-n][-x][-h][-e]" << endl;

        cerr << "\t-c 99\t\tgenerate 99 points for each distribution" << endl;
        cerr << "\t-u\t\toutput a data set for a uniform distribution" << endl;
        cerr << "\t-n\t\toutput a data set for a normal distribution" << endl;
        cerr << "\t-x\t\toutput a data set for an exponential distribution" << endl;
        cerr << "\t-h\t\toutput a data set for a hyperexponential distribution" << endl;
        cerr << "\t-e\t\toutput a data set for an erlang distribution" << endl;
        return 2;
    }

    // output a normal distribution by default
    if (!uniform && !normal && !exponential && !hyperexponential && !erlang)
	normal++;

    cout << "TitleText: Random Number Distributions" 
	 << " (" << count << " numbers/distribution)" 
	 << endl;

    for (int q=0; q<skip; q++) delete new UniformStream(0.0, 100.0);

    if (uniform) Gen (count, new UniformStream (0.0, 100.0), "Uniform");
    if (normal) Gen (count, new NormalStream (50.0, 15.0), "Normal");
    if (exponential) Gen (count, new ExponentialStream(50.0), "Exponential");
    if (hyperexponential) Gen (count, new HyperExponentialStream(50.0, 55.0), "HyperExponential");
    if (erlang) Gen (count, new ErlangStream(50.0, 15.0), "Erlang");

    return 0;
}
