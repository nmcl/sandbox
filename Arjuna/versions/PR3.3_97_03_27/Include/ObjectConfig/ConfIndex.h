/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ConfIndex.h,v 1.2 1994/08/05 12:12:31 nsmw Exp $
 */

#ifndef CONFINDEX_H_
#define CONFINDEX_H_

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

const int cIListSize = 120;

// @NoRemote, @NoMarshall
class Manager;

// @NoRemote, @NoMarshall
class MultiManager;

// @NoRemote, @NoMarshall
class ConfigIndex : public LockManager
{
public:
    static ConfigIndex* Create();
    static ConfigIndex* Create(const Uid&);
    static void         Destroy();
    
    Boolean add(const Uid& objUid, const MultiManager* multiMan);
    Boolean remove(const Uid& objUid);

    Boolean reconfig(Manager* man, MultiManager* dstMultiMan);

    Boolean find(const Uid& objUid, MultiManager*& multiMan);
    
    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

    ostream& PrintOn (ostream&) const;

protected:
    ConfigIndex ();
    ConfigIndex (const Uid&);
    virtual ~ConfigIndex ();
    
private:
    int         objectNumber;
    Uid         *objUidList[cIListSize];
    Uid         *mmUidList[cIListSize];
    
    static ConfigIndex *listHead;
    ConfigIndex        *nextElem;
};

#endif
