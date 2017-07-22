/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef JOB_H_
#define JOB_H_

/* Instances of this class represent the jobs which the Machine attempts
 * to process.
 * If the -DDEBUG option is added to the CFLAGS for the Makefile then useful
 * debugging code will be produced.
 */

class Job
{
public:
    Job ();
    ~Job ();

private:
    double ArrivalTime;
    double ResponseTime;
};

#endif
