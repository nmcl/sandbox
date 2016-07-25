/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Manager.h,v 1.1 1993/08/31 16:46:17 nsmw Exp $
 */

#ifndef MANAGER_H_
#define MANAGER_H_

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

// @NoRemote, @NoMarshall
class ConfigIndex;

// @NoRemote, @NoMarshall
class MultiManager;

// @NoRemote, @NoMarshall
class Manager
{
public:
    LockResult setlock (Lock*, int retry = 100);

    const Uid& get_uid () const;

    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual const TypeName type () const = 0;

protected:
    /* Constructors and destructor */

    Manager (ConfigIndex *confIndex, MultiManager *multiManager);
    Manager (const Uid& uid, ConfigIndex *confIndex);
    virtual ~Manager ();

    /* non-virtual member functions */

    void terminate ();
private:
    ConfigIndex  *configIndex;
    MultiManager *multiManager;
    Uid          objectUid;
};

#endif

