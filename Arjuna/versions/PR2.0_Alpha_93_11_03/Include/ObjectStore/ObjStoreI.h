/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjStoreI.h,v 1.1 1993/11/03 14:29:16 nmcl Exp $
 */

#ifndef OBJSTOREI_H_
#define OBJSTOREI_H_

#ifdef STUB
#  pragma @NoRename
#endif

/*
 *
 * Object Store Iterator Class
 *
 */

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

class ObjectState;
class ObjectStore;
class Uid;

// @NoRemote, @NoMarshall
class ObjectStoreIterator
{
public:
        /* Constructors & destructor */

    ObjectStoreIterator (ObjectStore& store, const TypeName& tName);
    ~ObjectStoreIterator ();
 
    Uid *operator() ();

private:   
    ObjectState *uidList;
};

#endif
