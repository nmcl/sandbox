/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ResourceListTest1.cc,v 1.1 1997/06/20 11:42:30 nsmw Exp $
 */

#ifndef OS_STDDEF_H_
#   include <os/stddef.h>
#endif

#ifndef OS_IOSTREAM_H_
#   include <os/iostream.h>
#endif

#ifndef TEMPLATE_RESOURCELIST_H_
#   include <Template/ResourceList.h>
#endif

#ifndef RESOURCE1_H_
#   include "Resource1.h"
#endif

int main(int, char**)
{
    ResourceList<Resource1> resourceList;

    Resource1 *a = new Resource1;
    Resource1 *b = new Resource1;
    Resource1 *c = new Resource1;

    resourceList.append(a);
    resourceList.append(b);
    resourceList.append(c);

    ResourceListIterator<Resource1> resourceListIter(&resourceList);

    cout << "a : " << (void *) resourceListIter.content() << endl;
    cout << "a : " << (void *) resourceListIter.content() << endl;
    resourceListIter.next();
    cout << "b : " << (void *) resourceListIter.content() << endl;
    resourceListIter.next();
    cout << "c : " << (void *) resourceListIter.content() << endl;
    resourceListIter.previous();
    cout << "b : " << (void *) resourceListIter.content() << endl;
    resourceListIter.head();
    cout << "a : " << (void *) resourceListIter.content() << endl;
    resourceListIter.tail();
    cout << "c : " << (void *) resourceListIter.content() << endl;
 
    resourceList.remove(b);

    resourceListIter.head();
    cout << "a : " << (void *) resourceListIter.content() << endl;
    resourceListIter.next();
    cout << "c : " << (void *) resourceListIter.content() << endl;

    resourceList.remove(a);
    resourceList.remove(c);

    resourceListIter.head();
    cout << "NULL : " << (void *) resourceListIter.content() << endl;

    return 0;
}
