/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HashedList.cc,v 1.6 1997/07/01 14:55:42 nsmw Exp $
 */

#ifndef TEMPLATE_HASHEDLIST_CC_
#define TEMPLATE_HASHEDLIST_CC_

template <class T, class U>
HashedList<T, U>::HashedList (unsigned long size)
			     : listSize(size),
			       numberOfEntries(0)
{
    hashedList = new BasicList<T, U>* [listSize];

    for (unsigned long i = 0; i < listSize; i++)
	hashedList[i] = new BasicList<T, U>;
}

template <class T, class U>
HashedList<T, U>::~HashedList ()
{
    for (unsigned long i = 0; i < listSize; i++)
	delete hashedList[i];

    delete [] hashedList;
}

template <class T, class U>
void HashedList<T, U>::insert (T* ToInsert, const U& key)
{
    if (!ToInsert)
	return;
    
    numberOfEntries++;
    hashedList[idToKey(key)]->insert(ToInsert);
}

template <class T, class U>
Boolean HashedList<T, U>::remove (T* element, const U& key)
{
    if ((numberOfEntries == 0) || (!element))
	return FALSE;

    if (hashedList[idToKey(key)]->remove(element))
    {
        numberOfEntries--;
	return TRUE;
    }

    return FALSE;
}

/*
 * Returns the thread list on which the thread is located. The
 * user then scans this list using the appropriate iterator and
 * looks for the destired thread in an implementation specific
 * manner.
 */

template <class T, class U>
const BasicList<T, U>* HashedList<T, U>::find (const U& key) const
{
    if (numberOfEntries == 0)
	return 0;

    return hashedList[idToKey(key)];
}

template <class T, class U>
unsigned long HashedList<T, U>::size () const
{
    return numberOfEntries;
}

template <class T, class U>
ostream& HashedList<T, U>::print (ostream& strm) const
{
    for (unsigned long i = 0; i < listSize; i++)
    {
        strm << "\nList element " << i << "\n";
	strm << *hashedList[i];
    }

    return strm;
}

template <class T, class U>
int HashedList<T, U>::idToKey (const U& id) const
{
    return (id.hash()%listSize);
}

template <class T, class U>
ostream& operator<< (ostream& strm, const HashedList<T, U>& hl)
{
    return hl.print(strm);
}

#endif
