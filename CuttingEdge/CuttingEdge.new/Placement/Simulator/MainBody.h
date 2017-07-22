#ifndef MAINBODY_
#define MAINBODY_

#include <Random.h>
#include <Process.h>
#include "Machine.h"
#include "Object.h"

/* This code initialized and starts the processes which make up the simulation.
 * If -DDEBUG is added to the CFLAGS for the Makefile then the simulation will
 * proceed as normal but information on the number of operations performed will
 * be outputted more regularly. If the -DFAST option is used then the simulation
 * only runs for 1000 successfully performed operations, as oppossed to 10000.
 */

class MainBody : public Process
{
public:
    MainBody (char*, char* = 0, char* = 0, char* = 0, char* = 0, char* = 0);
    ~MainBody ();

    void Body ();
    void Await ();

private:
    Machine* Sites[MAX_MACHINES];
    char* policy;
    int NumberOfObjects, NumberOfGoodNodes;
    double mean, sd, availability;
    NormalStream* NS;
};

#endif
