/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Opcode.h,v 1.5 1995/02/23 15:09:31 nmcl Exp $
 */

#ifndef OPCODE_H_
#define OPCODE_H_

enum Opcode
{
    OP_INSERT = 5, OP_WITHDRAW, OP_INSPECT,
    OP_TRANSFER, OP_RECOVER
};

enum BankActionStatus
{
    PREPARE = 10, ABORT, COMMIT
};

enum ErrorStatus
{
    NO_ERROR = 13, INVALID_UID, NO_ACTION
};
    
#define REQUEST_SIZE 128
#define REPLY_SIZE 128

#define outcomeDir "/tmp/"

#endif
