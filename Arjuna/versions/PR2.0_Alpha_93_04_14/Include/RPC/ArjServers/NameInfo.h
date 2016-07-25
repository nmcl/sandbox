/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: NameInfo.h
 */

#ifndef NAMEINFO_H_
#define NAMEINFO_H_

#ifndef LOCKMAN_H_
#include <Arjuna/LockMan.h>
#endif

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

class NameData
{
public:
    NameData ();
    ~NameData ();

    char* hostname;
    Uid* myUID;
    NameData* next;
};

class NameInfo : public LockManager
{
public:
    NameInfo (Boolean&);
    NameInfo (const Uid&, Boolean&);
    ~NameInfo ();

    NameData* GetReplicationInfo (Boolean&, int&);
    Boolean SetReplicationInfo (NameData*);

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    NameData* head;
    int replicationLevel;
};

#endif
