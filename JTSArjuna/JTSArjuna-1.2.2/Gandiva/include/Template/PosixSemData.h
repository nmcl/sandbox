/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PosixSemData.h,v 1.2 1998/01/12 12:58:14 nmcl Exp $
 */

#ifndef TEMPLATE_POSIXSEMDATA_H_
#define TEMPLATE_POSIXSEMDATA_H_

#ifndef TEMPLATE_SEMDATA_H_
#  include <Template/SemData.h>
#endif

template <class T>
class PosixSemData : public SemData<T>
{
public:
    PosixSemData (array_t);
    virtual ~PosixSemData ();

    virtual Boolean isOwner (const T& me, array_t& index) const;
};

#ifndef HAVE_TEMPLATE_REPOSITORY
#  include <Template/PosixSemData.cc>
#endif

#endif
