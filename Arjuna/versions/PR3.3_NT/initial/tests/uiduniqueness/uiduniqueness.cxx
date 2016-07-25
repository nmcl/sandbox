/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: UidUniqueness.cc,v 1.2 1994/09/14 08:56:22 ngdp Exp $
 */

#include <System/iostream.h>
#include <Common/Uid.h>

int main(int, char*[])
{
    Uid uids[10];

    for (int i = 0; i < 10; i++)
	cout << uids[i] << '\n' << flush;

    return 0;
}
