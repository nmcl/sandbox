/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: NameInfo.h,v 1.7 1994/08/05 11:37:28 nmcl Exp $
 */

#ifndef NAMEINFO_H_
#define NAMEINFO_H_

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

class NameData
{
public:
    NameData ();
    NameData (const Uid&, char*);
    ~NameData ();

    NameData& operator=  (const NameData&);
    Boolean   operator== (const NameData&);

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

    NameData* getReplicationInfo (Boolean&, int&);
    Boolean setReplicationInfo (NameData*);
    Boolean reset ();
    
    Boolean add (NameData*);
    Boolean remove (NameData*);

    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

private:
    NameData* head;
    int replicationLevel;
};

#endif
