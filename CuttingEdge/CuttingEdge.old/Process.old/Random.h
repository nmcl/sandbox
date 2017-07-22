#ifndef RANDOM_H
#define RANDOM_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
//  This file contains the interfaces for five different (pseudo-) random     //
//  number  generators:                                                       //
//                                                                            //
//  1) Uniform -          returns a  number drawn from a uniform distribution //
//                        with the given lower and upper bounds.              //
//                        Note: there are two versions of this class, one     //
//                              returning integers and the other doubles      //
//                                                                            //
//  2) Exponential -      returns a number from an exponential distribution   //
//                        with the given mean                                 //
//                                                                            //
//  3) Erlang -           returns a number from an Erlang distribution with   //
//                        the given mean and standard deviation               //
//                                                                            //
//  4) HyperExponential - returns a number from a hyperexpontial distribution //
//                        with the given mean and standard deviation          //
//                                                                            //
//  5) Normal -           returns a number from a normal distribution with    //
//                        the given mean and standard deviation.              //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// The class RandomStream is an abstract base class from which the other      //
// distribution classes are derived.                                          //
////////////////////////////////////////////////////////////////////////////////
class RandomStream {

public:
    RandomStream();
    virtual double operator() ()=0;
    double Error();	// returns a chi-square error measure on the uniform
			// distribution function
protected:
    double Uniform();

private:
    long Seed;
};

class UniformStream : public RandomStream {
public:
    UniformStream(double lo, double hi);
    virtual double operator() ();
private:
    double lo,hi;
    double range;
};

class Draw
{
public:
    Draw(double p);
    virtual boolean operator() ();
private:
    double prob;
};

class ExponentialStream : public RandomStream {
public:
    ExponentialStream(double Mean);
    virtual double operator() ();
private:
    double Mean;
};

class ErlangStream : public RandomStream {
public:
    ErlangStream(double Mean, double StandardDeviation);
    virtual double operator() ();
private:
    double Mean,StandardDeviation;
    long k;
};

class HyperExponentialStream : public RandomStream {
public:
    HyperExponentialStream(double Mean, double StandardDeviation);
    virtual double operator() ();
private:
    double Mean,StandardDeviation;
    double p;
};

class NormalStream : public RandomStream {
public:
    NormalStream(double Mean, double StandardDeviation);
    virtual double operator() ();
private:
    double Mean,StandardDeviation;
    double z;
};

#endif // RANDOM_H

