/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: GroupData.h
 */

#ifndef GROUPDATA_H_
#define GROUPDATA_H_

#ifndef SYS_PARAM_H_
#include <System/sys/param.h>
#endif

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#ifndef UID_H_
#include <Common/Uid.h>
#endif

// @NoRemote, @NoMarshall
class ostream;

// @NoRemote, @UserMarshall
class ReplicaDescriptor;

// @NoRemote, @NoMarshall
class GroupData
{
public:
    GroupData (char*);
    GroupData ();
    virtual ~GroupData ();

    void SetHost (char*);
    void SetUid (const Uid&);
    void SetActive (Boolean);

    char* GetHost () const;
    Uid* GetUid () const;
    Boolean GetActive () const;

    GroupData& operator= (const ReplicaDescriptor&);
    Boolean operator== (const ReplicaDescriptor&);
    
    virtual ostream& print (ostream&) const;

    GroupData* next;

private:
    char* destn_host;
    Uid* u;
    Boolean active;
};

extern ostream& operator<< (ostream& strm, const GroupData& gd);

#endif
