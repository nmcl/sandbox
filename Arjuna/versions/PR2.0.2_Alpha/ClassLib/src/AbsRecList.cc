/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AbsRecList.cc,v 1.2 1993/12/14 15:43:05 nmcl Exp $
 */


#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef ABSTRACTR_H_
#  include <Arjuna/AbstractR.h>
#endif

#ifndef ABSRECLIST_H_
#  include "AbsRecList.h"
#endif


AbstractRecordList::AbstractRecordList (AbstractRecord* (*func1)(),
					void (*func2)(AbstractRecord*&), const TypeName tn)
                                                                                          : RecordCreate(func1),
											    RecordDestroy(func2),
											    next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "AbstractRecordList::AbstractRecordList ( " << tn << " )" << endl;
#endif
    tName = ::new char[::strlen(tn)+1];
    ::memset(tName, '\0', ::strlen(tn)+1);
    ::memcpy(tName, tn, ::strlen(tn));
}

AbstractRecordList::~AbstractRecordList ()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "AbstractRecordList::~AbstractRecordList ()" << endl;
#endif

    if (tName)
#ifndef __GNU__
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
