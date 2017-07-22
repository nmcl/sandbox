#include <math.h>
#include "Random.h"

// Use an assortment of seeds
static long InitialSeeds[] = { 1878892440L, 831312807L,  1449793615L,  1973272912L }; 

// Initial seeds will be selected round-robin from the above list
static int selector=0;

RandomStream::RandomStream()
{
    Seed = InitialSeeds[selector++%(sizeof(InitialSeeds)/sizeof(long))];
}

double RandomStream::Uniform() 
{
    // A linear congruential generator based on the algorithm from
    // "Algorithms", R. Sedgewick, Addison-Wesley, Reading MA, 1983.
    // pp. 36-38.
    const long m=100000000;
    const long b=31415821;
    const long m1=10000;

    // Do the multiplication in pieces to avoid overflow
    long    p0 = Seed%m1,
	    p1 = Seed/m1,
	    q0 = b%m1,
	    q1 = b/m1;

    Seed = (((((p0*q1+p1*q0)%m1)*m1+p0*q0)%m) + 1) % m;

    return (double) Seed / (double) m;
}

double RandomStream::Error()
{
    const long r=100;
    const long N=100*r;
    long i, f[r];
    for (i=0; i<r; i++) f[i]=0;
    for (i=0; i<N; i++) f[(int) (Uniform()*r)]++;
    long t=0;
    for (i=0; i<r; i++) t += f[i]*f[i];
    double rt = (double) r*t;
    double rtN = rt / (double) N - (double) N;
    return 1.0 - (rtN / r);
}

UniformStream::UniformStream(double l, double h)
{
    lo = l;
    hi = h;
    range = hi-lo;
}

double UniformStream::operator() ()
{
    return lo+(range*Uniform());
}

Draw::Draw(double p) { prob = p; }

boolean Draw::operator() ()
{
    UniformStream *s = new UniformStream(1, 100);

    double x = (*s());

    if (x >= prob)
	return true;
    else
	return false;
}

ExponentialStream::ExponentialStream(double m)
{
    Mean = m;
}

double ExponentialStream::operator() ()
{
    return -Mean*log(Uniform());
}

ErlangStream::ErlangStream(double m, double s)
{
    Mean = m;
    StandardDeviation = s;

    double z = Mean/StandardDeviation;
    k = (long) (z*z);
}

double ErlangStream::operator() ()
{
    double z=1.0;
    for (int i=0; i<k; i++) z*=Uniform();
    return -(Mean/k)*log(z);
}

HyperExponentialStream::HyperExponentialStream(double m, double s)
{
    Mean = m;
    StandardDeviation = s;
    double cv,z;
    cv=StandardDeviation/Mean;
    z = cv*cv;
    p = 0.5*(1.0-sqrt((z-1.0)/(z+1.0)));
}

double HyperExponentialStream::operator() ()
{
    double z = (Uniform()>p) ? Mean/(1.0-p) : Mean/p;
    return -0.5*z*log(Uniform());
}

NormalStream::NormalStream(double m, double s)
{
    Mean = m;
    StandardDeviation = s;
    z = 0.0;
}

double NormalStream::operator() ()
{
    // Use the polar method, due to Box, Muller and Marsaglia
    // Taken from Seminumerical Algorithms, Knuth, Addison-Wesley, p.117

    double X2;

    if (z!=0.0) {
	X2 = z;
	z = 0.0;
    } else {
	double S, v1, v2;
	do {
	    v1 = 2.0*Uniform()-1.0;
	    v2 = 2.0*Uniform()-1.0;
	    S = v1*v1 + v2*v2;
	} while (S>=1.0);

	S = sqrt((-2.0*log(S))/S);
	X2 = v1*S;
	z  = v2*S;
    }

    return Mean + X2*StandardDeviation;
}
