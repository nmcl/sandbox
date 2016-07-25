/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: PrimaryNS.h
 */

#ifndef PRIMARYNS_H_
#define PRIMARYNS_H_

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#include <Common/Buffer.h>
#endif

#ifndef ARJNAME_H_
#include <RPC/ArjServers/ArjName.h>
#endif

#ifndef NSINTERFACE_H_
#include <RPC/ArjServers/NSInterface.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#ifndef REMOTEOSTYPES_H_
#  include <RPC/ArjServers/RemoteOSTypes.h>
#endif

class PrimaryNS
{
public:
    PrimaryNS (int&);
    ~PrimaryNS ();

    ActionStatus Begin ();
    ActionStatus End ();
    ActionStatus Abort ();

    Boolean fetchNameList (const ArjunaName&);
    ArjunaName* getPrimary ();
    Boolean registerPrimary ();

private:
    void scanPast (int, Buffer&, Boolean = FALSE);
    void scanAfter (int, Buffer&);

    ArjunaName* findPrimary ();

    ArjunaName* AN;
    AtomicAction* Z;
    NSInterface* NS;
    Buffer NameList, key;
    unsigned short index;
    Boolean first, got, IsPrimary;
    int number;
};

#endif
