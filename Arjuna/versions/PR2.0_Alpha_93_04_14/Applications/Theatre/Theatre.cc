/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: Theatre.cc,v 1.7 1993/03/18 14:03:22 nmcl Exp $
 */

#include <Configure.h>

#include <string.h>
#include <iostream.h>

#include <InterViews/world.h>
#include <IV-look/2.6/InterViews/frame.h>

#include "Theatre.h"
#include "TheatreDispatch.h"

char* TheatreHost;
Uid*  TheatreUid;
int   TheatreNew;

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
{
    int err = 0;
    World* world = new World ("Theatre", argc, argv);

    TheatreHost = NULL;
    TheatreUid  = new Uid("128.240.150.1:0:706281940:636525");
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
