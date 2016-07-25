/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: UidUniqueness.cc,v 1.2 1993/03/18 15:08:03 n048z Exp $
 */

#include <System/iostream.h>
#include <Common/Uid.h>

int main(int, char*[])
{
    Uid uids[16];

    for (int i = 0; i < 16; i++)
	cout << uids[i] << '\n' << flush;
}
