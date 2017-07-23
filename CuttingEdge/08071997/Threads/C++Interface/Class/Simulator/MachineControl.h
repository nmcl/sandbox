#ifndef MachineControl_
#define MachineControl_

#include "Machine.h"

class MachineControl : public Process
{
private:
    Machine *TheMachine;
public:
    MachineControl(Machine*);
    ~MachineControl();

    void Body();
};

#endif
