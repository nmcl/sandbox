/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: MultiMan.h,v 1.1 1993/08/31 16:46:21 nsmw Exp $
 */

#ifndef MULTIMAN_H_
#define MULTIMAN_H_

#ifndef STATEMAN_H_
#  include <Arjuna/StateMan.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifdef STUB
#  pragma @NoRename
#endif

const int mmListSize = 120;

// @NoRemote, @NoMarshall
class Manager;

// @NoRemote, @NoMarshall
class MultiManager : public LockManager
{
public:
    static MultiManager* Create();
    static MultiManager* Create(const Uid&);
    static void          Destroy();

    Boolean do_setlock (Manager*, LockResult&, Lock*, int);

    Boolean do_add(Uid, Manager*, ObjectState*);
    Boolean do_remove(Manager*, Manager*&, ObjectState*&);
    Boolean do_register(Manager*);
    Boolean do_unregister(Manager*);

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

    virtual ostream& PrintOn (ostream&) const;
private:
    int     FindUid(const Uid&);

    MultiManager ();
    MultiManager (const Uid&);
    virtual ~MultiManager ();

    int         numObjects;
    Uid         *uidList[mmListSize];
    ObjectState *objectStateList[mmListSize];
    Manager     *managerList[mmListSize];

    static MultiManager *listHead;
    MultiManager        *nextElem;
};

#endif
