#include <iostream.h>

#include <Process.h>
#include "Job.h"
#include "Machine.h"
#include "Queue.h"

extern Machine* M;
extern Queue JobQ;
extern long TotalJobs;
extern double TotalResponseTime;
extern SimulatedTime;
extern Scheduler *sc;

Job::Job()
{
    boolean empty;

    ResponseTime = 0;
    ArrivalTime = sc->CurrentTime();
    empty = JobQ.IsEmpty();
    JobQ.Enqueue(this);
    TotalJobs++;

    if (empty && !M->Processing() && M->IsOperational())
	M->Activate();
}

Job::~Job()
{
    ResponseTime = sc->CurrentTime() - ArrivalTime;

//    cerr << "ArrivalTime is " << ArrivalTime << "\n";
//    cerr << "Time now is " << sc->CurrentTime() << "\n";
//    cerr << "ResponseTime is " << ResponseTime << "\n";

    TotalResponseTime += ResponseTime;
}
