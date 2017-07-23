#ifndef MACHINE_
#define MACHINE_

#include <Random.h>
#include <Process.h>

class Machine : public Process
{
private:
    ExponentialStream* STime;
    boolean operational;
    boolean working;

public:
    Machine(double);
    ~Machine();

    void Body();
    void Broken();
    void Fixed();
    boolean IsOperational();
    boolean Processing();
    double ServiceTime();
};

#endif
