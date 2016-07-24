/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: MultiMan.h,v 1.3 1994/10/21 12:27:58 nsmw Exp $
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

    Boolean do_setlock (Manager*, LockResult&, Lock*, int, unsigned int);

    Boolean do_add(Uid, Manager*, ObjectState*);
    Boolean do_remove(Manager*, Manager*&, ObjectState*&);
    Boolean do_register(Manager*);
    Boolean do_unregister(Manager*);

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

    virtual ostream& PrintOn (ostream&) const;

protected:
    MultiManager();
    MultiManager(const Uid&);
    virtual ~MultiManager();

private:
    int     FindUid(const Uid&);

    int         numObjects;
    Uid         *uidList[mmListSize];
    ObjectState *objectStateList[mmListSize];
    Manager     *managerList[mmListSize];

    static MultiManager *listHead;
    MultiManager        *nextElem;
};

#endif
