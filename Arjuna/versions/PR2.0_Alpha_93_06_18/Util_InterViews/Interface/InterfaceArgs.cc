/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $id:$
 */

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef SYS_TYPES_H_
#include <System/sys/types.h>
#endif

#ifndef SYS_SOCKET_H_
#include <System/sys/socket.h>
#endif

#ifndef NETDB_H_
#include <System/netdb.h>
#endif

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef INTERFACEARGS_H_
#include "InterfaceArgs.h"
#endif

InterfaceArgs::InterfaceArgs () {}

InterfaceArgs::~InterfaceArgs () {}

Boolean InterfaceArgs::CheckArgs (int argc, char* argv[], Boolean NameRequired)
{
    Boolean FoundName = FALSE;
    for (int i = 1; i < argc; i++)
    {
	if (strcmp("-iname", argv[i]) == 0)
	{
	    if (i+1 < argc)
	    {
		Definitions::InterfaceName = argv[i+1];
		FoundName = TRUE;
	    }
	    else
	    {
		cerr << "No interface name specified after -iname option.\n" << flush;
		return FALSE;
	    }
	}

	if (strcmp("-uid", argv[i]) == 0)
	{
	    if (i+1 < argc)
		Definitions::DatabaseUID = argv[i+1];
	    else
	    {
		cerr << "No UID string specified after -uid option.\n" << flush;
		return FALSE;
	    }
	}

	if (strcmp("-name", argv[i]) == 0)
	{
	    if (i+1 < argc)
	    {
		if (!gethostbyname(argv[i+1]))
		{
		    cerr << "Error. Invalid host name " << argv[i+1] << "\n" << flush;
		    return FALSE;
		}

		Definitions::DatabaseLocation = argv[i+1];
	    }
	    else
	    {
		cerr << "No database location specified after -name option.\n" << flush;
		return FALSE;
	    }
	}
    }

    if ((!FoundName) && (NameRequired))
    {
	cerr << "Error. No -iname option given.\n" << flush;
	return FALSE;
    }
    else
	return TRUE;
}
