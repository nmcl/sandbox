/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Members.h,v
 */


#ifndef MEMBERS_H_
#define MEMBERS_H_

class ClientRpc;
class RpcControl;
class RemoteStateDaemon;

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
    RpcControl* RPCCont;
    RemoteStateDaemon* r_state_daemon;
    
    Members* next;
};

#endif
