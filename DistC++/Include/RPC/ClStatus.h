/*
 * Copyright (C) 1993-1998,
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
    UNKNOWN_STATUS, UNINITIATED, OPERATIONAL, INOPERATIONAL, DEAD, INIT_UNABLE, UNABLE
};

//  @NoRemote, @NoMarshall
enum RPC_Status
{
    OPER_DONE, OPER_UNKNOWN, OPER_NOTDONE
};

#endif
