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
 * $Id: Theatre.cc,v 1.1 1993/11/11 15:19:31 nmcl Exp $
 */

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <iostream.h>

#include <Arjuna/AtomicA.h>

#include <ObjectConfig/MultiMan.h>
#include <ObjectConfig/ConfIndex.h>

#include "Theatre.h"
#include "ParArgs.h"
#include "Time.h"
#include "TheatrePersis.h"

int            theatreSize   = 100;
TheatrePersis* theatrePersis = NULL;

int main(int argc, char** argv)
{
    int err;

    err = ParArgs(argc, argv);
    
    if (err != 1)
    {
        AtomicAction A;

        A.Begin();

        int sel;
        Time start, finish, dur;

	cout << "Object Uid : " << theatrePersis->get_uid() << endl;

        cout << endl << "Configurations:" << endl;
        cout << "1) Seat" << endl << "2) Row" << endl << "3) Theatre" << endl << endl;
        cout << "Selection [1-3] ";
        cin >> sel;

        start.get();
        
        if (sel == 1)
            theatrePersis->ClusterAsSeats(err);
        if (sel == 2)
            theatrePersis->ClusterAsRows(err);
        if (sel == 3)
            theatrePersis->ClusterAsTheatre(err);

        finish.get();

        dur = finish - start;

        cout << "Time = " << dur << endl;

        if (err == 1)
            cout << "Reconfiguration :: Done" << endl;
        else
            cout << "Reconfiguration :: Not Done (" << err << ')' << endl;

        A.End();
    }

    delete theatrePersis;
    SeatPersis::Destroy();
    MultiManager::Destroy();
    ConfigIndex::Destroy();

    return 0;
}
