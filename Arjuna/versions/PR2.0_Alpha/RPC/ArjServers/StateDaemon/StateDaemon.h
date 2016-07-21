/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: StateDaemon.h
 */

#ifndef STATEDAEMON_H_
#define STATEDAEMON_H_

#ifndef UID_H_
#include <Common/Uid.h>
#endif

#ifndef BUFFER_H_
#include <Common/Buffer.h>
#endif

#ifndef LOCKMAN_H_
#include <Arjuna/LockMan.h>
#endif

#ifndef OBJSTATE_H_
#include <Arjuna/ObjState.h>
#endif

typedef char* string;

// @NoRemote, @NoMarshall
class RpcBuffer;

inline RpcBuffer& operator>> (RpcBuffer& rpcbuff, FileType& ft)
{
    int dummy;
    rpcbuff >> dummy;

    ft = (FileType) dummy;
    return rpcbuff;
}

class StateDaemon
{
public:
    StateDaemon (int&, Boolean);
    virtual ~StateDaemon ();

    Buffer ReadState (const Uid&, const TypeName, int&) const;
    Buffer ReadUnCommitted (const Uid&, const TypeName, int&) const;
    Boolean RemoveState (const Uid&, const TypeName, int&) const;
    Boolean RemoveUnCommitted (const Uid&, const TypeName, int&) const;
    Boolean WriteCommitted (const Uid&, const TypeName, Buffer, int&) const;
    Boolean WriteState (const Uid&, const TypeName, Buffer, int&) const;
    Boolean CommitState (const Uid&, const TypeName, int&) const;

    /* These few should really be private */
    Buffer AllObjUids (const TypeName, int&) const;

    void AreYouAlive (Boolean&) const;

private:
    ObjectStore* OS;
};

#endif
