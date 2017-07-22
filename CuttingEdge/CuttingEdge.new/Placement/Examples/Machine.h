#ifndef MACHINE_
#define MACHINE_

#include <Random.h>
#include <Process.h>

/* This is the machine which services job requests. It is prone to simulated
 * failures caused by the Breaks process.
 */

class Machine : public Process
{
public:
    Machine (double);
    ~Machine ();

    void Body ();

    void Broken ();
    void Fixed ();
    boolean IsOperational ();
    boolean Processing ();
    double ServiceTime ();

private:
    ExponentialStream* STime;
    boolean operational;
    boolean working;
};

#endif
