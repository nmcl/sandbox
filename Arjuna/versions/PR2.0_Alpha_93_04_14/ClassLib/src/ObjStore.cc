/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjStore.cc,v 1.35 1993/03/22 09:20:00 ngdp Exp $
 */

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef SIGNAL_H_
#include <System/signal.h>
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef OBJSTORE_H_
#  include <Arjuna/ObjStore.h>
#endif

#ifndef LPOBJSTORE_H_
#  include <Arjuna/LPObjStore.h>
#endif

#ifndef RPOBJSTORE_H_
#  include <Arjuna/RPObjStore.h>
#endif

#ifndef LVOBJSTORE_H_
#include <Arjuna/LVObjStore.h>
#endif

#ifndef LSPOBJSTORE_H_
#include <Arjuna/LSPObjStore.h>
#endif

static const char RCSid[] = "$Id: ObjStore.cc,v 1.35 1993/03/22 09:20:00 ngdp Exp $";

ObjectStore::ObjectStore ()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectStore::ObjectStore()\n" << flush;
#endif
}

ObjectStore::~ObjectStore ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectStore::~ObjectStore()\n" << flush;
#endif
}

ObjectStore* ObjectStore::Create (const char* typeOf)
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectStore* ObjectStore::Create ( " << typeOf << " )\n" << flush;
#endif

    if (typeOf)
    {
	if (strcmp(typeOf, "RemotePersistentObjectStore") == 0)
	{
	    RemotePersistentObjectStore* x = new RemotePersistentObjectStore;
	    return x;
	}

	if (strcmp(typeOf, "LocalVolatileObjectStore") == 0)
	{
	    LocalVolatileObjectStore* x = new LocalVolatileObjectStore;
	    return x;
	}

	if (strcmp(typeOf, "LocalSimplePersistentObjectStore") == 0)
	{
	    LocalSimplePersistentObjectStore* x = new LocalSimplePersistentObjectStore;
	    return x;
	}
	else
	{
	    LocalPersistentObjectStore* x = new LocalPersistentObjectStore;
	    return x;
	}
    }
    else
    {
	LocalPersistentObjectStore* x = new LocalPersistentObjectStore;
	return x;
    }
}
