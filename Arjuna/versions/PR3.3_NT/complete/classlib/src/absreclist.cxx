/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AbsRecList.cc,v 1.5 1995/06/26 13:10:53 ngdp Exp $
 */


#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef ABSTRACTR_H_
#  include <Arjuna/AbstractR.h>
#endif

#ifndef ABSRECLIST_H_
#  include "AbsRecList.h"
#endif


AbstractRecordList::AbstractRecordList ( AbstractRecord* (*func1)(),
					 void (*func2)(AbstractRecord*&),
					 const TypeName tn )
				       : next(0),
					 RecordCreate(func1),
					 RecordDestroy(func2)
{
    tName = ::new char[::strlen(tn)+1];
    ::memset(tName, '\0', ::strlen(tn)+1);
    ::memcpy(tName, tn, ::strlen(tn));
}

AbstractRecordList::~AbstractRecordList ()
{
    if (tName)
#ifndef GCC_ARRAY_DELETE_BUG
      ::delete [] tName;
#else
      ::delete tName;
#endif
    if (next)
	delete next;
}

const TypeName AbstractRecordList::type () const { return tName; }

AbstractRecord* AbstractRecordList::create () { return (*RecordCreate)(); }

void AbstractRecordList::remove (AbstractRecord*& toDelete) { (*RecordDestroy)(toDelete); }
