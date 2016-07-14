/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: TowersOfHanoi.cc,v 1.3 1993/03/18 14:08:48 nmcl Exp $
 */

#include <Configure.h>

#include <System/iostream.h>
#include <System/string.h>

#include <InterViews/world.h>
#include <InterViews/frame.h>

#include "TowersDispatch.h"

char* Pin1Host;
Uid*  Pin1Uid;
char* Pin2Host;
Uid*  Pin2Uid;
char* Pin3Host;
Uid*  Pin3Uid;
char* Pin4Host;
Uid*  Pin4Uid;
char* WSHost;
Uid*  WSUid;
int   TowersNew;

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
{
    int err = 0;
    World* world = new World ("Towers Of Hanoi", argc, argv);

    Pin1Host  = NULL;
    Pin1Uid   = new Uid("128.240.150.1:0:705945694:350435");
    Pin2Host  = NULL;
    Pin2Uid   = new Uid("128.240.150.1:0:705945696:588992");
    Pin3Host  = NULL;
    Pin3Uid   = new Uid("128.240.150.1:0:705945697:966636");
    Pin4Host  = NULL;
    Pin4Uid   = new Uid("128.240.150.1:0:705945699:312595");
    WSHost    = NULL;
    WSUid     = new Uid("128.240.150.1:0:705945691:436904");
    TowersNew = 0;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp("-new", argv[i]) == 0)
        {
            if (Pin1Uid != NULL)
                free(Pin1Uid);
            if (Pin2Uid != NULL)
                free(Pin2Uid);
            if (Pin3Uid != NULL)
                free(Pin3Uid);
            if (Pin4Uid != NULL)
                free(Pin4Uid);
            if (WSUid != NULL)
                free(WSUid);

            Pin1Uid  = NULL;
            Pin2Uid  = NULL;
            Pin3Uid  = NULL;
            Pin4Uid  = NULL;
            WSUid    = NULL;
            TowersNew  = 1;
        }
        else if (strcmp("-uid1", argv[i]) == 0)
        {
            i++;
            if (i < argc)
            {
                if (Pin1Uid != NULL)
                    free(Pin1Uid);

                Pin1Uid = new Uid(argv[i]);
                TowersNew = 0;
            }
            else
            {
                cerr << "Error afer -uid1\n";
                err = 1;
            }
        }
        else if (strcmp("-host1", argv[i]) == 0)
        {
            i++;
            if (i < argc)
            {
                if (Pin1Host != NULL)
                    free(Pin1Host);

                Pin1Host = strdup(argv[i]);
            }
            else
            {
                cerr << "Error after -host1\n";
                err = 1;
            }
        }
        else if (strcmp("-uid2", argv[i]) == 0)
        {
            i++;
            if (i < argc)
            {
                if (Pin2Uid != NULL)
                    free(Pin2Uid);

                Pin2Uid = new Uid(argv[i]);
                TowersNew = 0;
            }
            else
            {
                cerr << "Error afer -uid2\n";
                err = 1;
            }
        }
        else if (strcmp("-host2", argv[i]) == 0)
        {
            i++;
            if (i < argc)
            {
                if (Pin2Host != NULL)
                    free(Pin2Host);

                Pin2Host = strdup(argv[i]);
            }
            else
            {
                cerr << "Error after -host2\n";
                err = 1;
            }
        }
        else if (strcmp("-uid3", argv[i]) == 0)
        {
            i++;
            if (i < argc)
            {
                if (Pin3Uid != NULL)
                    free(Pin3Uid);

                Pin3Uid = new Uid(argv[i]);
                TowersNew = 0;
            }
            else
            {
                cerr << "Error afer -uid3\n";
                err = 1;
            }
        }
        else if (strcmp("-host3", argv[i]) == 0)
        {
            i++;
            if (i < argc)
            {
                if (Pin3Host != NULL)
                    free(Pin3Host);

                Pin3Host = strdup(argv[i]);
            }
            else
            {
                cerr << "Error after -host3\n";
                err = 1;
            }
        }
        else if (strcmp("-uid4", argv[i]) == 0)
        {
            i++;
            if (i < argc)
            {
                if (Pin4Uid != NULL)
                    free(Pin4Uid);

                Pin4Uid = new Uid(argv[i]);
                TowersNew = 0;
            }
            else
            {
                cerr << "Error afer -uid4\n";
                err = 1;
            }
        }
        else if (strcmp("-host4", argv[i]) == 0)
        {
            i++;
            if (i < argc)
            {
                if (Pin4Host != NULL)
                    free(Pin4Host);

                Pin4Host = strdup(argv[i]);
            }
            else
            {
                cerr << "Error after -host4\n";
                err = 1;
            }
        }
        else if (strcmp("-uidws", argv[i]) == 0)
        {
            i++;
            if (i < argc)
            {
                if (WSUid != NULL)
                    free(WSUid);

                WSUid = new Uid(argv[i]);
                TowersNew = 0;
            }
            else
            {
                cerr << "Error afer -uidws\n";
                err = 1;
            }
        }
        else if (strcmp("-hostws", argv[i]) == 0)
        {
            i++;
            if (i < argc)
            {
                if (WSHost != NULL)
                    free(WSHost);

                WSHost = strdup(argv[i]);
            }
            else
            {
                cerr << "Error after -hostws\n";
                err = 1;
            }
        }
	else 
	{
	    cerr << "Unknown Argument\n";
	    err = 1;
	}
    }

    if ((err == 0) && (TowersNew == 0))
	if ((WSUid   == NULL) || (Pin1Uid == NULL) || (Pin2Uid == NULL) ||
	    (Pin3Uid == NULL) || (Pin4Uid == NULL))
	{
	    cerr << "Error not all uids set for objects\n";
	    err = 1;
	}

    if (err != 1)
    {
	TowersDispatch* towersDis = new TowersDispatch;

	world->InsertApplication(new Frame(towersDis));

	towersDis->Run();

	delete towersDis;
    }

    return 0;
}
