/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjStoreI.h,v 1.2 1994/07/11 13:14:09 ngdp Exp $
 */

#ifndef OBJSTOREI_H_
#define OBJSTOREI_H_

/*
 *
 * Object Store Iterator Class
 *
 */

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef OBJSTATE_H_
#  include <Common/ObjState.h>
#endif

class ObjectStore;
class Uid;

// @NoRemote, @NoMarshall
class ObjectStoreIterator
{
public:
        /* Constructors & destructor */

    ObjectStoreIterator (ObjectStore& store, const TypeName& tName);
    ~ObjectStoreIterator ();
 
    Uid operator() ();

private:   
    ObjectState uidList;
};

#endif
