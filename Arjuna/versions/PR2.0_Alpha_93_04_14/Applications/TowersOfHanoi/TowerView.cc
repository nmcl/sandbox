/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: TowerView.cc,v 1.5 1993/03/18 14:08:40 nmcl Exp $
 */

#include <Configure.h>

#include <System/string.h>

#include <Common/Error.h>

#include <InterViews/world.h>
#include <InterViews/frame.h>
#include <InterViews/box.h>
#include <InterViews/banner.h>

#include "TowerView.h"
#include "TowerDispatch.h"

char* PinHost;
Uid*  PinUid;
char* PinName;

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
{
    int err = 0;
    World* world = new World ("Tower View", argc, argv);
    
    PinHost  = NULL;
    PinUid   = new Uid("128.240.150.1:0:705945694:350435");
    PinName  = "Pin 1";
    
    for (int i = 1; i < argc; i++)
    {
        if (strcmp("-uid", argv[i]) == 0)
        {
            i++;
            if (i < argc)
            {
                if (PinUid != NULL)
                    free(PinUid);
		
                PinUid = new Uid(argv[i]);
                PinName  = "Pin Unknown";
            }
            else
            {
                error_stream << "Error after -uid\n";
                err = 1;
            }
        }
        else if (strcmp("-host", argv[i]) == 0)
        {
            i++;
            if (i < argc)
            {
                if (PinHost != NULL)
                    free(PinHost);
		
                PinHost = strdup(argv[i]);
            }
            else
            {
                error_stream << "Error after -host\n";
                err = 1;
            }
        }
        else if (strcmp("-pin1", argv[i]) == 0)
        {
	    if (PinUid != NULL)
		free(PinUid);
	    
	    PinUid = new Uid("128.240.150.1:0:705945694:350435");
            PinName  = "Pin 1";
        }
        else if (strcmp("-pin2", argv[i]) == 0)
        {
	    if (PinUid != NULL)
		free(PinUid);
	    
	    PinUid = new Uid("128.240.150.1:0:705945696:588992");
            PinName  = "Pin 2";
        }
        else if (strcmp("-pin3", argv[i]) == 0)
        {
	    if (PinUid != NULL)
		free(PinUid);
	    
	    PinUid = new Uid("128.240.150.1:0:705945697:966636");
            PinName  = "Pin 3";
        }
        else if (strcmp("-inhand", argv[i]) == 0)
        {
	    if (PinUid != NULL)
		free(PinUid);
	    
	    PinUid = new Uid("128.240.150.1:0:705945699:312595");
            PinName  = "In Hand";
        }
	else
	{
	    error_stream << "Unknown Argument\n";
	    err = 1;
	}
    }	
    
    if (err != 1)
    {
	TowerDispatch* towerDis = new TowerDispatch;
	
	world->InsertApplication(new Frame(new VBox(towerDis, new Banner("", PinName, ""))));
	
	towerDis->Run();
	
	delete towerDis;
    }

    return 0;
}
