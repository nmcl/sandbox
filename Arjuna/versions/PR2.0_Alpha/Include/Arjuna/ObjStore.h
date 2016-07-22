/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjStore.h,v 1.18 1993/03/22 09:37:01 ngdp Exp $
 */

#ifndef OBJSTORE_H_
#define OBJSTORE_H_

#ifdef STUB
#  pragma @NoRename
#endif

/*
 *
 * Base Object Store Class
 *
 */

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

class ostream;

class Buffer;
class ObjectState;
class Uid;

enum FileType { SHADOW, ORIGINAL, HIDDEN };

// @NoRemote, @NoMarshall
class ObjectStore
{
public:
    virtual ~ObjectStore ();

    /* The real interface */
    virtual Boolean commit_state (const Uid&, const TypeName) = 0;
    virtual ObjectState *read_state (const Uid&, const TypeName) = 0;
    virtual ObjectState *read_uncommitted (const Uid&, const TypeName) = 0;
    virtual Boolean remove_state (const Uid&, const TypeName) = 0;
    virtual Boolean remove_uncommitted (const Uid&, const TypeName) = 0;
    virtual Boolean write_committed (const Uid&, const TypeName, const ObjectState&) = 0;
    virtual Boolean write_state (const Uid&, const TypeName, const ObjectState&) = 0;
    virtual ObjectState *allObjUids (const TypeName) = 0;

    virtual const TypeName type () const = 0;
    virtual void storeLocation (const char*) = 0;

    static ObjectStore* Create (const char*);

protected:
    /* Constructors & destructor */

    ObjectStore ();
};

#endif
