/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: uidToLocation.cc,v 1.2 1995/09/30 10:28:33 nmcl Exp $
 */

/*
 * Give Location in HashedStore for UID.
 */


#include <Config/Configure.h>

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STDIO_H_
#  include <System/stdio.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif


int main (int argc, char** argv)
{
    char* UidName = 0;
    
    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " -Uid uid" << endl;
	    exit(0);
	}
	if (::strcmp(argv[i], "-Uid") == 0)
		UidName = argv[i+1];
	}

    if (UidName == 0)
    {
	// We've don't have enough info
	cout << "Usage: " << argv[0] << " -uid uid" << endl;
	exit(0);
    }
    else
    {
        Uid objectUid(UidName);

	cout << "Location for " << objectUid << " is " << objectUid.hash() % 255 << endl;
    }

    return 0;
}

