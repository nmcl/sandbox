/*
 * Copyright 1990, 1991, 1992, 1993,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: ParArgs.cc,v 1.1 1993/11/11 15:19:29 nmcl Exp $
 */

#include <System/string.h>
#include <System/iostream.h>

#include <Arjuna/AtomicA.h>

#include "ParArgs.h"
#include "Theatre.h"
#include "TheatrePersis.h"

static Uid TheatreUid;
static int TheatreNew   = 0;
static int TheatreReset = 0;
static int groupSize    = 100;

static Uid TheatreTheatreUid("128.240.150.8:0:735225179:949346");
static Uid RowTheatreUid("128.240.150.8:0:735225128:109721");
static Uid SeatTheatreUid("128.240.150.8:0:735224810:389087");

int ParArgs(int argc, char** argv)
{
    int err = 0;

    TheatreNew = 0;
    TheatreUid = TheatreTheatreUid;

    for (int i = 1; i < argc; i++)
    {
	if (strcmp("-new", argv[i]) == 0)
	    TheatreNew = 1;
	else if (strcmp("-reset", argv[i]) == 0)
            TheatreReset = 1;
	else if (strcmp("-seat", argv[i]) == 0)
        {
	    groupSize  = 1;
            TheatreUid = SeatTheatreUid;
        }
	else if (strcmp("-row", argv[i]) == 0)
        {
	    groupSize  = 5;
            TheatreUid = RowTheatreUid;
        }
	else if (strcmp("-theatre", argv[i]) == 0)
        {
	    groupSize  = 100;
            TheatreUid = TheatreTheatreUid;
        }
	else if (strcmp("-uid", argv[i]) == 0)
	{
	    i++;
	    if (i < argc)
	    {
		TheatreUid = Uid(argv[i]);
		TheatreNew = 0;
	    }    
	    else
	    {
		cerr << "Error after -uid" << endl;
		err = 1;
	    }
	}	else
	{
	    cerr << "Unknown Argument" << endl;
	    err = 1;

	}
    }

    if (err == 0)
    {
        int res;

        if (TheatreNew == 1)
	{
            theatrePersis = new TheatrePersis(theatreSize, groupSize, res);
	    
	    if (res != 1)
            {
                cerr << "Object not created." << endl;
                err = 1;
            }
	}
        else
        {
	    theatrePersis = new TheatrePersis(res, TheatreUid);
	    
	    if (res != 1)
            {
                cerr << "Object not recreated." << endl;
                err = 1;
            }
        }

        if ((err == 0) && (TheatreReset == 1))
        {
            AtomicAction A;

            A.Begin();

            res = 1;
	    for (int i = 0; (res != 0) && (i < theatreSize); i++)
            {
                if (i < (theatreSize / 2))
                    theatrePersis->Reserve_Seat(i, res);
                else
                    theatrePersis->Unreserve_Seat(i, res);

            }

            if (res == 0)
            {
                A.Abort();
                cerr << "Object not reset." << endl;
                err = 1;
            }
            else
                if (A.End() != COMMITTED)
                {
                    cerr << "RESET: End() aborted." << endl;
                    err = 1;
                }
        }        
    }

    return err;
}
