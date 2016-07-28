/*
 * Copyright 1990, 1991, 1992, 1993,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: Operation.cc,v 1.2 1995/02/13 11:31:54 ngdp Exp $
 */

#include <System/stdlib.h>

#include <iostream.h>

#include <Arjuna/AtomicA.h>

#include "TheatrePersis.h"

#include "Theatre.h"

static int ResWork(int resNumSeats)
{
    int res = 0;
    int i   = ((int) rand()) % theatreSize;
    int j   = i;
    int k   = i;

    AtomicAction A;

    A.Begin();

    while (j < (k + resNumSeats))
    {
	enum Seat_Status status = Unreserved;
	    
	theatrePersis->Get_Seat_Status((j % theatreSize), status, res);
	    
	j++;
	if (! ((res == 1) && (status == Unreserved)))
	    k = j;
    }

    for (int l = 0; (l < resNumSeats) && (res == 1); l++)
	theatrePersis->Reserve_Seat((k + l) % theatreSize, res);

    if (res == 1)
    {
        if (A.End() != COMMITTED)
	    cerr << "ResWork: unable to commit Atomic Action!!" << endl;

#ifdef DEBUG
	cout << "ResWork: Done (" << i << ", " << k % theatreSize << ")." << endl;
#endif

	return 1;
    }
    else
    {
	A.Abort();

#ifdef DEBUG
	if (res == 0)
	    cout << "ResWork: Lock conflict (" << i << ", " << k % theatreSize << ")." << endl;
	else if (res == 2)
	    cout << "ResWork: Seat reserved (" << i << ", " << k % theatreSize << ")." << endl;
	else
	    cout << "ResWork: Unknown error (" << i << ", " << k % theatreSize << ")." << endl;
#endif

	return 0;
    }
}

int UnresWork(int unresNumSeats)
{
    int res = 0;
    int i   = ((int) lrand48()) % theatreSize;
    int j   = i;
    int k   = i;

    AtomicAction A;

    A.Begin();

    while (j < (k + unresNumSeats))
    {
	enum Seat_Status status = Reserved;
	    
	theatrePersis->Get_Seat_Status((j % theatreSize), status, res);
	    
	j++;
        if (! ((res == 1) && (status == Reserved)))
 	    k = j;
    }

    for (int l = 0; (l < unresNumSeats) && (res == 1); l++)
	theatrePersis->Unreserve_Seat((k + l) % theatreSize, res);

    if (res == 1)
    {
	if (A.End() != COMMITTED)
	    cerr << "UnresWork: unable to commit Atomic Action!!" << endl;

#ifdef DEBUG
	cout << "UnresWork: Done (" << i << ", " << k % theatreSize << ")." << endl;
#endif

	return 1;
    }
    else
    {
	A.Abort();

#ifdef DEBUG
	if (res == 0)
	    cout << "UnresWork: Lock conflict (" << i << ", " << k % theatreSize << ")." << endl;
	else if (res == 2)
	    cout << "UnresWork: Seat reserved (" << i << ", " << k % theatreSize << ")." << endl;
	else
	    cout << "UnresWork: Unknown error (" << i << ", " << k % theatreSize << ")." << endl;
#endif

	return 0;
    }
}
int Operation(int resNumSeats)
{
    int res = 0;

    AtomicAction A;

    A.Begin();

    if ((res = ResWork(resNumSeats)) == 1)
         res = UnresWork(resNumSeats);

    if (res == 1)
    {
#ifdef DEBUG
        cout << "Tasks: Done" << endl;
#endif
        if (A.End() != COMMITTED)
        {
            cerr << "Tasks: unable to commit Atomic Action!!" << endl;
            res = 0;
        }
    }
    else
    {
#ifdef DEBUG
        cout << "Tasks: NotDone" << endl;
#endif
        A.Abort();
    }

    return res;
}
