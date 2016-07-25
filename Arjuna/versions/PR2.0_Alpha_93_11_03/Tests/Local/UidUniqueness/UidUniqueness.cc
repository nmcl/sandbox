/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: UidUniqueness.cc,v 1.1 1993/11/03 15:01:43 nmcl Exp $
 */

#include <System/iostream.h>
#include <Common/Uid.h>

int main(int, char*[])
{
    Uid uids[16];

    for (int i = 0; i < 16; i++)
	cout << uids[i] << '\n' << flush;
}
