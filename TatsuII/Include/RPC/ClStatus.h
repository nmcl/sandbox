/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


#ifndef CLSTATUS_H_
#define CLSTATUS_H_

//  @NoRemote, @NoMarshall
enum ServerStatus
{
    UNINITIATED, OPERATIONAL, INOPERATIONAL, SOFTDEAD, DEAD
};

//  @NoRemote, @NoMarshall
enum RPC_Status
{
    OPER_DONE, OPER_UNKNOWN, OPER_NOTDONE
};

#endif
