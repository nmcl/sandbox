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

#define OP_INSERT 1
#define OP_WITHDRAW 2
#define OP_INSPECT 3
#define OP_DONE 4
#define OP_NOTDONE 5
#define OP_PREPARE 6
#define OP_ABORT 7
#define OP_COMMIT 8
#define OP_TRANSFER 9

#define REQUEST_SIZE 128
#define REPLY_SIZE 128

#define outcomeFile "/tmp/transactionOutcome"

#endif
