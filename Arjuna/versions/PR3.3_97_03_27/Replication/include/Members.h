/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Members.h,v 1.5 1995/09/09 13:26:02 nmcl Exp $
 */

#ifndef MEMBERS_H_
#define MEMBERS_H_

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

class ClientRpc;
class ClientRpcManager;
class RemoteRPObjStore;

/*
 * List of successfully created daemons. Always a subset of the groupview.
 */


// @NoRemote, @NoMarshall
class Members
{
public:
    Members ();
    ~Members ();

    ClientRpc* CRPC;
    ClientRpcManager* RPCCont;
    RemoteRPObjStore* rpobjstore_daemon;
    Uid pingTopLevel;
    
    Members* next;
};

#endif
