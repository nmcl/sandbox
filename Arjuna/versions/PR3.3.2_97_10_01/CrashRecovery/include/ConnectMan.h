/*
 * Copyright (C) 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ConnectMan.h,v 1.4 1995/02/22 14:55:19 ngdp Exp $
 */

#ifndef CONNECTMAN_H_
#define CONNECTMAN_H_

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

/*
 * Enum: ConnectManStatus
 * Status of connection managed by a ConnectMan
 */

enum ConnectManStatus
{
    UNKNOWN, DEAF, LISTENING, DOWN
};

/*
 * Enum: RPCType
 * Possible RPC mechanisms supported by ConnectMan
 */

enum RPCType
{
    UNKNOWN_RPC, RAJDOOT
};

ostream& operator<< (ostream&, ConnectManStatus);
ostream& operator<< (ostream&, RPCType);

/*
 * Class: Connection Manager (ConnectMan)
 * Generic interface to alternative RPC mechanisms. Controls whether a
 * node will receive requests from remote nodes.
 */

/*
 * NB: We should really use inheritance and have managers for each RPC type
 * i.e. RajdootManager but since currently only Rajdoot is supported properly
 * this is not really warranted
 */

class ConnectMan
{
public:
    ConnectMan(ostream&);
    ConnectMan(ostream&, RPCType);
    ~ConnectMan();

    Boolean control(Boolean);
    Boolean isAlive();
    void print (ostream&);

private:
    // Operations for Rajdoot manager
    Boolean switchManager(Boolean);
    Boolean pingManager();
    int getManagerPid();

    // ConnectMan state
    ostream&         outStrm;
    ConnectManStatus status;
    RPCType          rpcType;
};

#endif // CONNECTMAN_H_ 
