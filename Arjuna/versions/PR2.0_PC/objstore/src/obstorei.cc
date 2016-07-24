/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjStoreI.cc,v 1.4 1993/11/24 12:42:31 ngdp Exp $
 */

/*
 *
 * ObjectStore Iterator
 *
 */

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjStore/ObjStore.h>
#endif

#ifndef OBJSTOREI_H_
#  include <ObjStore/ObStoreI.h>
#endif

static const char RCSid[] = "$Id: ObjStoreI.cc,v 1.4 1993/11/24 12:42:31 ngdp Exp $";

/*
 * Public functions
 */

/*
 * Constructor :
 */

ObjectStoreIterator::ObjectStoreIterator ( ObjectStore& store,
					   const TypeName& tName )
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectStoreIterator::ObjectStoreIterator(" 
	<< tName << ")" << endl;
#endif

    uidList = store.allObjUids(tName);
}

ObjectStoreIterator::~ObjectStoreIterator ()
{
    if (uidList)
	delete uidList;
}

/*
 * return the Uids from the list one at a time. ObjStore returns either
 * null list or a list terminated by the NIL_UID. Use the latter to return
 * 0 (for end of list)
 */

Uid* ObjectStoreIterator::operator () ()
{
    if (uidList == 0)
	return 0;

    Uid *newUid = new Uid(NIL_UID);

    if (!newUid->unpack(*uidList) || (*newUid == NIL_UID))
    {
	delete newUid;
	return 0;
    }

    return newUid;
}
    
