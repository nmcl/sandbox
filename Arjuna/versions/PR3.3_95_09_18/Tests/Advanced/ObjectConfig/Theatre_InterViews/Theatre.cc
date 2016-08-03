/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: Theatre.cc,v 1.3 1995/02/22 14:55:50 ngdp Exp $
 */

#include <Config/Configure.h>

#include <string.h>
#include <iostream.h>

#include <InterViews/world.h>
#include <IV-look/2.6/InterViews/frame.h>

#include "Theatre.h"
#include "TheatreDispatch.h"

#include <ObjectConfig/MultiMan.h>
#include <ObjectConfig/ConfIndex.h>

Uid*  TheatreUid;
int   TheatreNew;

int main(int argc, char** argv)
{
    int err = 0;
    World* world = new World ("Theatre", argc, argv);

    TheatreUid  = new Uid("128.240.150.8:0:735569029:654026");
    TheatreNew  = 0;

    for (int i = 1; i < argc; i++)
    {
	if (strcmp("-new", argv[i]) == 0)
	{
	    if (TheatreUid != NULL)
		delete(TheatreUid);

	    TheatreUid  = NULL;
	    TheatreNew  = 1;
	}
	else if (strcmp("-uid", argv[i]) == 0)
	{
	    i++;
	    if (i < argc)
	    {
		if (TheatreUid != NULL)
		    delete(TheatreUid);

		TheatreUid = new Uid(argv[i]);
		TheatreNew = 0;
	    }    
	    else
	    {
		cerr << "Error afer -uid\n";
		err = 1;
	    }
	}
	else
	{
	    cerr << "Unknown Argument\n";
	    err = 1;

	}
    }

    if (err != 1)
    {
	TheatreDispatch* theatreDisp = new TheatreDispatch;

	world->InsertApplication(new Frame(theatreDisp));

	theatreDisp->Run();

	delete theatreDisp;
    }

    SeatPersis::Destroy();
    MultiManager::Destroy();
    ConfigIndex::Destroy();

    return 0;
}
