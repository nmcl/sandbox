/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: UidTest2.cc,v 1.3 1997/01/27 17:22:41 nsmw Exp $
 */

#include <os/stdlib.h>
#include <os/iostream.h>

#ifndef COMMON_UID_H_
#  include <Common/Uid.h>
#endif


int main (int, char**)
{
    Uid uid1, uid2, uid3, uid4;

    cout << "Uid 1 : " << uid1 << endl;
    cout << "Uid 2 : " << uid2 << endl;
    cout << "Uid 3 : " << uid3 << endl;
    cout << "Uid 4 : " << uid4 << endl;

    if ((uid1 != uid2) && (uid2 != uid3) && (uid3 != uid4))
	cout << "\nAll unique.\n" << endl;
    
    if (uid1 < uid2)
	cout << "Uid 1 < Uid 2" << endl;
    else
	cout << "Uid 1 > Uid 2" << endl;

    if (uid3 > uid4)
	cout << "Uid 3 > Uid 4" << endl;
    else
	cout << "Uid 3 < Uid 4" << endl;

    cout << "\nAssigning Uid 1 to Uid 3" << endl;
    uid3 = uid1;

    if (uid3 == uid1)
	cout << "Assigned ok" << endl;
    else
	cout << "Assignment error" << endl;

    return 0;
}
