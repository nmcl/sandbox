/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ListTest2.cc,v 1.1 1997/03/20 10:14:14 nsmw Exp $
 */

#ifndef OS_STDDEF_H_
#   include <os/stddef.h>
#endif

#ifndef OS_IOSTREAM_H_
#   include <os/iostream.h>
#endif

#ifndef TEMPLATE_LIST_H_
#   include <Template/List.h>
#endif

int main(int, char**)
{
    List<int> list;

    int a = 12, b = 17, c = 20;

    list.append(&a);
    list.append(&b);
    list.append(&c);

    ListIterator<int> listIter(&list);

    listIter.head();
    while (listIter.content() != NULL)
    {
        cout << "O: " << *(listIter.content()) << endl;

        listIter.next();
    }
 
    listIter.head();
    listIter.previous();
    cout << "swapForward: on ";
    if (listIter.content() != NULL)
        cout << *(listIter.content());
    else
        cout << "NULL";
    if (listIter.swapForward())
        cout << ": succeeded, now ";
    else
        cout << ": failed, now ";
    if (listIter.content() != NULL)
        cout << *(listIter.content()) << endl;
    else
        cout << "NULL" << endl;

    listIter.head();
    while (listIter.content() != NULL)
    {
        cout << "F-1: " << *(listIter.content()) << endl;

        listIter.next();
    }

    listIter.head();
    listIter.previous();
    cout << "swapBackward: on ";
    if (listIter.content() != NULL)
        cout << *(listIter.content());
    else
        cout << "NULL";
    if (listIter.swapBackward())
        cout << ": succeeded, now ";
    else
        cout << ": failed, now ";
    if (listIter.content() != NULL)
        cout << *(listIter.content()) << endl;
    else
        cout << "NULL" << endl;

    listIter.head();
    while (listIter.content() != NULL)
    {
        cout << "B-1: " << *(listIter.content()) << endl;

        listIter.next();
    }

    listIter.head();
    cout << "swapForward: on ";
    if (listIter.content() != NULL)
        cout << *(listIter.content());
    else
        cout << "NULL";
    if (listIter.swapForward())
        cout << ": succeeded, now ";
    else
        cout << ": failed, now ";
    if (listIter.content() != NULL)
        cout << *(listIter.content()) << endl;
    else
        cout << "NULL" << endl;

    listIter.head();
    while (listIter.content() != NULL)
    {
        cout << "F0: " << *(listIter.content()) << endl;

        listIter.next();
    }

    listIter.head();
    cout << "swapBackward: on ";
    if (listIter.content() != NULL)
        cout << *(listIter.content());
    else
        cout << "NULL";
    if (listIter.swapBackward())
        cout << ": succeeded, now ";
    else
        cout << ": failed, now ";
    if (listIter.content() != NULL)
        cout << *(listIter.content()) << endl;
    else
        cout << "NULL" << endl;

    listIter.head();
    while (listIter.content() != NULL)
    {
        cout << "B0: " << *(listIter.content()) << endl;

        listIter.next();
    }

    listIter.head();
    listIter.next();
    cout << "swapForward: on ";
    if (listIter.content() != NULL)
        cout << *(listIter.content());
    else
        cout << "NULL";
    if (listIter.swapForward())
        cout << ": succeeded, now ";
    else
        cout << ": failed, now ";
    if (listIter.content() != NULL)
        cout << *(listIter.content()) << endl;
    else
        cout << "NULL" << endl;

    listIter.head();
    while (listIter.content() != NULL)
    {
        cout << "F1: " << *(listIter.content()) << endl;

        listIter.next();
    }

    listIter.head();
    listIter.next();
    cout << "swapBackward: on ";
    if (listIter.content() != NULL)
        cout << *(listIter.content());
    else
        cout << "NULL";
    if (listIter.swapBackward())
        cout << ": succeeded, now ";
    else
        cout << ": failed, now ";
    if (listIter.content() != NULL)
        cout << *(listIter.content()) << endl;
    else
        cout << "NULL" << endl;

    listIter.head();
    while (listIter.content() != NULL)
    {
        cout << "B1: " << *(listIter.content()) << endl;

        listIter.next();
    }

    listIter.head();
    listIter.next();
    listIter.next();
    cout << "swapForward: on ";
    if (listIter.content() != NULL)
        cout << *(listIter.content());
    else
        cout << "NULL";
    if (listIter.swapForward())
        cout << ": succeeded, now ";
    else
        cout << ": failed, now ";
    if (listIter.content() != NULL)
        cout << *(listIter.content()) << endl;
    else
        cout << "NULL" << endl;

    listIter.head();
    while (listIter.content() != NULL)
    {
        cout << "F2: " << *(listIter.content()) << endl;

        listIter.next();
    }

    listIter.head();
    listIter.next();
    listIter.next();
    cout << "swapBackward: on ";
    if (listIter.content() != NULL)
        cout << *(listIter.content());
    else
        cout << "NULL";
    if (listIter.swapBackward())
        cout << ": succeeded, now ";
    else
        cout << ": failed, now ";
    if (listIter.content() != NULL)
        cout << *(listIter.content()) << endl;
    else
        cout << "NULL" << endl;

    listIter.head();
    while (listIter.content() != NULL)
    {
        cout << "B2: " << *(listIter.content()) << endl;

        listIter.next();
    }

    listIter.head();
    listIter.next();
    listIter.next();
    listIter.next();
    cout << "swapForward: on ";
    if (listIter.content() != NULL)
        cout << *(listIter.content());
    else
        cout << "NULL";
    if (listIter.swapForward())
        cout << ": succeeded, now ";
    else
        cout << ": failed, now ";
    if (listIter.content() != NULL)
        cout << *(listIter.content()) << endl;
    else
        cout << "NULL" << endl;

    listIter.head();
    while (listIter.content() != NULL)
    {
        cout << "F3: " << *(listIter.content()) << endl;

        listIter.next();
    }

    listIter.head(); 
    listIter.next();
    listIter.next();
    listIter.next();
    cout << "swapBackward: on ";
    if (listIter.content() != NULL)
        cout << *(listIter.content());
    else
        cout << "NULL";
    if (listIter.swapBackward())
        cout << ": succeeded, now ";
    else
        cout << ": failed, now ";
    if (listIter.content() != NULL)
        cout << *(listIter.content()) << endl;
    else
        cout << "NULL" << endl;

    listIter.head();
    while (listIter.content() != NULL)
    {
        cout << "B3: " << *(listIter.content()) << endl;

        listIter.next();
    }

    return 0;
}
