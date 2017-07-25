#ifndef ARRIVALS_
#define ARRIVALS_

#include <Process.h>
#include <Random.h>

/* Controls the rate at which Jobs arrive at the Machine */

class Arrivals : public Process
{
public:
    Arrivals (double);
    ~Arrivals ();

    void Body ();

private:
    ExponentialStream* InterArrivalTime;
};

#endif
