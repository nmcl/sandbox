#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#include "Monitor.h"

void main (int argc, char** argv)
{
    int res = -1;
    Monitor* M = 0;
    Boolean done = FALSE, got;
    timeval *t1 = 0, *t2 = 0;

    if (argc > 1)
    {
	if (strcmp(argv[1], "-old") == 0)
	{
	    Uid u(argv[2]);
	    M = new Monitor(u, res);
	    done = TRUE;
	}
    }

    if (!done)
    {
	M = new Monitor(res);
    }

    if (res == 0)
    {
	got = FALSE;
	t1 = M->GetMTTF(got);

	if (got)
	{
	    cout << "Current MTTF is <" << t1->tv_sec << ", " << t1->tv_usec << ">\n" << flush;

	    got = FALSE;
	    t2 = M->GetMTTR(got);

	    if (got)
	    {
		cout << "Current MTTR is <" << t2->tv_sec << ", " << t2->tv_usec << ">\n" << flush;
		cout << "\nEntering monitor state\n" << flush;

		M->doMonitor(1);
	    }
	    else
		cout << "Error getting MTTR\n" << flush;
	}
	else
	    cout << "Error getting MTTF\n" << flush;
    }
    else
	cout << "Error in constructor\n" << flush;

    if (M)
	delete M;
}
