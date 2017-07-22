#ifndef MAINBODY_
#define MAINBODY_

#include <Process.h>
#include "Machine.h"

class MainBody : public Process
{
private:
    Machine* Sites[MAX_MACHINES];
    char* policy;

public:
    MainBody(char*);
    ~MainBody();

    void Body();
    void Await();
};

#endif
