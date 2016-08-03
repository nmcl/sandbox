/*
 * Copyright 1990, 1991, 1992
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 *
 * Permission to use, copy, modify and distribute this program for
 * evaluation, teaching and/or research purposes only and without fee is
 * hereby granted, providing that this copyright and permission notice
 * appear on all copies and supporting documentation, and that similar
 * conditions are imposed on any individual or organization to whom the
 * program is distributed. The University of Newcastle upon Tyne makes no
 * representation about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 */

/*
 * $Id: TheatreDisplay.cc,v 1.2 1995/02/13 11:31:55 ngdp Exp $
 */

#include <System/stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <iostream.h>

#include <Arjuna/AtomicA.h>

#include <ObjectConfig/MultiMan.h>
#include <ObjectConfig/ConfIndex.h>

#include "Theatre.h"
#include "ParArgs.h"
#include "TheatrePersis.h"

int            theatreSize   = 100;
TheatrePersis* theatrePersis = NULL;

int main(int argc, char** argv)
{
    int err, res;

    srand48(getpid());
    
    err = ParArgs(argc, argv);
    
    if (err != 1)
    {
	    cout << "Object Uid : " << theatrePersis->get_uid() << endl;

	    {
		AtomicAction A;

		A.Begin();

		int numR = 0, numU = 0;
		for (int i = 0; i < theatreSize; i++)
		{
		    enum Seat_Status status;

		    res = 0;
		    theatrePersis->Get_Seat_Status(i, status, res);
		    if (res == 0)
			cout << "? ";
		    else
		    {
			if (status == Reserved)
			{
			    numR++;
			    cout << "R ";
			}
			else
			{
			    numU++;
			    cout << "U ";
			}
		    }

		    if ((i % 10) == 9)
			cout << endl;
		}
                cout << endl;

		cout << "Reserved   = " << numR << endl;
		cout << "Unreserved = " << numU << endl;
		
		A.End();
	    }
	}

    delete theatrePersis;
    SeatPersis::Destroy();
    MultiManager::Destroy();
    ConfigIndex::Destroy();

    return 0;
}
