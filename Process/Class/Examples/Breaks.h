#ifndef BREAKS_
#define BREAKS_

#include <Random.h>
#include <Process.h>

class Breaks : public Process
{
private:
    UniformStream* RepairTime;
    UniformStream* OperativeTime;
    boolean interrupted_service;

public:
    Breaks();
    ~Breaks();

    void Body();
};

#endif
