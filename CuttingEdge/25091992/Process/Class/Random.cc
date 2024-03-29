#include <iostream.h>
#include <math.h>
#include "Random.h"

RandomStream::RandomStream(long MGSeed, long LCGSeed)
{
    // Clean up input parameters
    if ((MGSeed&1) == 0) MGSeed--;
    if (MGSeed<0) MGSeed = -MGSeed;
    if (LCGSeed<0) LCGSeed = -LCGSeed;

    // Initialise state
    MSeed = MGSeed;
    LSeed = LCGSeed;

    for (int i=0; i< (sizeof(series)/sizeof(double)); i++)
	series[i] = MGen();
}

double RandomStream::MGen()
{
    // A multiplicative generator, courtesy I. Mitrani 1992,
    // private correspondence
    // Y[i+1] = Y[i] * 5^5 mod 2^26
    // period is 2^24, initial seed must be odd

    const long int two2the26th = 67108864;	// 2**26

    MSeed = (MSeed * 25) % two2the26th;
    MSeed = (MSeed * 25) % two2the26th;
    MSeed = (MSeed * 5) % two2the26th;

    return (double) MSeed / (double) two2the26th;
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
    long    p0 = LSeed%m1,
	    p1 = LSeed/m1,
	    q0 = b%m1,
	    q1 = b/m1;

    LSeed = (((((p0*q1+p1*q0)%m1)*m1+p0*q0)%m) + 1) % m;

    // The results of the LC generator are shuffled with
    // the multiplicative generator as suggested by
    // Maclaren and Marsaglia (See Knuth Vol2, Seminumerical Algorithms)

    long choose = LSeed % (sizeof(series)/sizeof(double));

    double result = series[choose];
    series[choose] =  MGen();

    return result;
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
    
Draw::Draw(double p)
{
    prob = p;
    s = new UniformStream(0, 1);
}

boolean Draw::operator() ()
{
    double x = (*s)();

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
