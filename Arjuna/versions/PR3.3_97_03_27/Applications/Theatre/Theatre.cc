/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: Theatre.cc,v 1.5 1995/02/22 14:54:59 ngdp Exp $
 */

#include <Config/Configure.h>

#include <string.h>
#include <iostream.h>

#include <IV-2_6/InterViews/world.h>
#include <IV-2_6/InterViews/frame.h>

#include "Theatre.h"
#include "TheatreDispatch.h"

char* TheatreHost;
Uid*  TheatreUid;
int   TheatreNew;

int main(int argc, char** argv)
{
    int err = 0;
    World* world = new World ("Theatre", argc, argv);

    TheatreHost = NULL;
    TheatreUid  = new Uid("80f09608:559a:2cda7599:1");
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
	else if (strcmp("-host", argv[i]) == 0)
	{
	    i++;
	    if (i < argc)
	    {
		if (TheatreHost != NULL)
		    delete(TheatreHost);

		TheatreHost = strdup(argv[i]);
	    }    
	    else
	    {
		cerr << "Error after -host\n";
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

    return 0;
}
