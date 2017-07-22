#ifndef ARRIVALS_
#define ARRIVALS_

#include <Process.h>
#include <Random.h>

class Arrivals : public Process
{
private:
    ExponentialStream* InterArrivalTime;

public:
    Arrivals(double);
    ~Arrivals();

    void Body();
};

#endif
