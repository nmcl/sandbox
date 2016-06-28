#include <iostream.h>
#include "common.h"
#include "Process.h"
#include "ProcessList.h"

class Processor : public Process
{
    int state;
public:
    Processor(int);
    ~Processor();

    void Body();
};
