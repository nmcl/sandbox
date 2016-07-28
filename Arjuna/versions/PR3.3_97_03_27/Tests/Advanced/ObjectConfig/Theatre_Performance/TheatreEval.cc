/*
 * Copyright 1990, 1991, 1992
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: TheatreEval.cc,v 1.2 1994/12/06 09:27:09 ngdp Exp $
 */

#include <System/stdlib.h>
#include <System/sys/types.h>
#include <System/sys/time.h>
#include <System/unistd.h>
#include <System/string.h>
#include <System/iostream.h>

#include <Arjuna/AtomicA.h>

#include <ObjectConfig/MultiMan.h>
#include <ObjectConfig/ConfIndex.h>

#include "Theatre.h"
#include "ParArgs.h"
#include "Operation.h"
#include "TheatrePersis.h"

TheatrePersis* theatrePersis = NULL;
int  theatreSize = 100;

int main(int argc, char** argv)
{
    int err, res;

    srand48(getpid());
    
    err = ParArgs(argc, argv);
    
    if (err != 1)
    {
#ifdef DEBUG
         cout << "Object Uid : " << theatrePersis->get_uid() << endl;
#endif

        int numS = 0;
        int numF = 0;
        int seatToAccess = 10;

        for (int i = 0; i < 10; i++)
        {
            res = Operation(seatToAccess);

            if (res == 1)
                numS++;
            else
                numF++;
	}

        cout << "Tasks: Sucesses " << numS << " of " << numS + numF
             << " (" << 100.0 * numS / (numS + numF) << "%) (A"
             << seatToAccess << ")" << endl;
    }


    if (theatrePersis != NULL)
        delete theatrePersis;

    SeatPersis::Destroy();
    MultiManager::Destroy();
    ConfigIndex::Destroy();

    return 0;
}
