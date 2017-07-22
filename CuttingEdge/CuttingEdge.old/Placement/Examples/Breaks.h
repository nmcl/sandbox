#ifndef BREAKS_
#define BREAKS_

#include <Random.h>
#include <Process.h>

/* Waits for a time interval drawn from the OperativeTime. Then kills of the
 * Machine and waits for a time interval drawn from the RepairTime before
 * restarting the Machine.
 */

class Breaks : public Process
{
public:
    Breaks ();
    ~Breaks ();

    void Body ();

private:
    UniformStream* RepairTime;
    UniformStream* OperativeTime;
    boolean interrupted_service;
};

#endif
