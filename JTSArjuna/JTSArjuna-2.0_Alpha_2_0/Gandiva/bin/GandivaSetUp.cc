/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: GandivaSetUp.cc,v 1.3 1997/01/27 17:22:36 nsmw Exp $
 */

#ifndef OS_STDDEF_H_
#   include <os/stddef.h>
#endif

#ifndef OS_IOSTREAM_H_
#   include <os/iostream.h>
#endif

#ifndef GANDIVA_H_
#   include <Gandiva.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef COMMON_OBJECTNAME_H_
#   include <Common/ObjectName.h>
#endif

int main(int, char *[])
{
    Boolean res = FALSE;

    ObjectName on1("LNS:Gandiva");

    cout << on1 << endl;
    res = on1.setAttribute("Version", "1.2.0");
    cout << on1 << ", \"Version\", " << "\"1.2.0\"";
    if (res)
        cout << " : succeed" << endl;
    else
        cout << " : failed" << endl;
    cout << endl;

    ObjectName on2("INS:Gandiva");

    cout << on2 << endl;
    res = on2.setAttribute("$ObjectName", on1);
    cout << on2 << ", \"$ObjectName\", " << on1;
    if (res)
        cout << " : succeed" << endl;
    else
        cout << " : failed" << endl;

    return 0;
}
