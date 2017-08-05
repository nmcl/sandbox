/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ListTest1.cc,v 1.1 1997/03/20 10:14:13 nsmw Exp $
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

    cout << "12 : " << *(listIter.content()) << endl;
    cout << "12 : " << *(listIter.content()) << endl;
    listIter.next();
    cout << "17 : " << *(listIter.content()) << endl;
    listIter.next();
    cout << "20 : " << *(listIter.content()) << endl;
    listIter.previous();
    cout << "17 : " << *(listIter.content()) << endl;
    listIter.head();
    cout << "12 : " << *(listIter.content()) << endl;
    listIter.tail();
    cout << "20 : " << *(listIter.content()) << endl;
 
    list.remove(&b);

    listIter.head();
    cout << "12 : " << *(listIter.content()) << endl;
    listIter.next();
    cout << "20 : " << *(listIter.content()) << endl;

    list.remove(&a);
    list.remove(&c);

    listIter.head();
    cout << "NULL : " << listIter.content() << endl;

    return 0;
}
