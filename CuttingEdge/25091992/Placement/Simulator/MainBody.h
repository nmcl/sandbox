#ifndef MAINBODY_
#define MAINBODY_

#include <Process.h>
#include "Machine.h"

/* This code initialized and starts the processes which make up the simulation.
 * If -DDEBUG is added to the CFLAGS for the Makefile then the simulation will
 * proceed as normal but information on the number of operations performed will
 * be outputted more regularly.
 */

class MainBody : public Process
{
public:
    MainBody (char*);
    ~MainBody ();

    void Body ();
    void Await ();

private:
    Machine* Sites[MAX_MACHINES];
    char* policy;
};

#endif
