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

    Buffer readState (const Uid&, const TypeName, int&) const;
    Buffer readUnCommitted (const Uid&, const TypeName, int&) const;
    Boolean removeState (const Uid&, const TypeName, int&) const;
    Boolean removeUnCommitted (const Uid&, const TypeName, int&) const;
    Boolean writeCommitted (const Uid&, const TypeName, Buffer, int&) const;
    Boolean writeState (const Uid&, const TypeName, Buffer, int&) const;
    Boolean commitState (const Uid&, const TypeName, int&) const;

    /* These few should really be private */
    Buffer allObjUids (const TypeName, int&) const;

    void areYouAlive (Boolean&) const;

private:
    static ObjectStore* OS;
};

#endif
