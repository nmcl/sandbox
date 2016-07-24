/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjStore.cc,v 1.6 1993/12/10 11:37:48 nmcl Exp $
 */

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef SIGNAL_H_
#  include <System/signal.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjStore/ObjStore.h>
#endif

#ifndef LSPOBJSTORE_H_
#  include <ObjStore/PObStore.h>
#endif


static const char RCSid[] = "$Id: ObjStore.cc,v 1.6 1993/12/10 11:37:48 nmcl Exp $";


ObjectStore::ObjectStore ()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectStore::ObjectStore ()" << endl;
#endif
}

ObjectStore::~ObjectStore ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectStore::~ObjectStore ()" << endl;
#endif
}

void ObjectStore::destroy (ObjectStore*& toDelete)
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "void ObjectStore::destroy ( " << toDelete->type() << " )" << endl;
#endif

    delete toDelete;
    toDelete = 0;
}

ObjectStore* ObjectStore::create (const TypeName name, const char* locationOfStore)
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectStore* ObjectStore::create ( " << name << " )" << endl;
#endif

	return new LocalSimplePersistentObjectStore(locationOfStore);
}


#ifdef NO_INLINES
#  define OBJSTORE_CC_
#  include "ObjectStore/ObjStore.n"
#  undef OBJSTORE_CC_
#endif
