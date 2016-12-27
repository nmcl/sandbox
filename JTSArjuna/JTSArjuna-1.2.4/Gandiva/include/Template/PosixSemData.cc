/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PosixSemData.cc,v 1.2 1998/01/12 12:58:14 nmcl Exp $
 */

#ifndef TEMPLATE_POSIXSEMDATA_CC_
#define TEMPLATE_POSIXSEMDATA_CC_

#ifndef OS_PTHREAD_PTHREAD_H_
#  include <os/pthread/pthread.h>
#endif

#ifndef TEMPLATE_POSIXSEMDATA_H_
#  include <Template/PosixSemData.h>
#endif

template<class T>
PosixSemData<T>::PosixSemData (array_t size)
			      : SemData<T>(size)
{
}

template<class T>
PosixSemData<T>::~PosixSemData ()
{
}

template<class T>
Boolean PosixSemData<T>::isOwner (const T& me, array_t& index) const
{
    for (index = 0; index < _numberAllowed; index++)
    {
        if ((_currentOwners[index]) && (pthread_equal(_currentOwners[index], me)))
	    return TRUE;
    }

    return FALSE;
}

#endif
