/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjStoreI.cc,v 1.6 1995/06/26 10:30:01 ngdp Exp $
 */

/*
 *
 * ObjectStore Iterator
 *
 */

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef OBJSTOREI_H_
#  include <ObjectStore/ObjStoreI.h>
#endif

static const char RCSid[] = "$Id: ObjStoreI.cc,v 1.6 1995/06/26 10:30:01 ngdp Exp $";

/*
 * Public functions
 */

/*
 * Constructor :
 */

ObjectStoreIterator::ObjectStoreIterator ( ObjectStore& store,
					   const TypeName& tName )
{
    store.allObjUids(tName, uidList);
}

ObjectStoreIterator::~ObjectStoreIterator ()
{
}

/*
 * return the Uids from the list one at a time. ObjStore returns either
 * null list or a list terminated by the NIL_UID. Use the latter to return
 * 0 (for end of list)
 */

Uid ObjectStoreIterator::operator () ()
{
    Uid newUid(NIL_UID);

    if (!newUid.unpack(uidList))
    {
	newUid = NIL_UID;
    }

    return newUid;
}
    
