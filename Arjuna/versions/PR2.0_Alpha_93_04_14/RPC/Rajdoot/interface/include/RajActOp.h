/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 *    This header file contains the definitions of the opcode
 *    and error codes used across the Rajdoot Action Interface.
 */

/*
 *  $Id: RajActOp.h,v 1.3 1993/03/18 15:35:11 nmcl Exp $
 */

// Opcodes

#define RAO_NESTEDABORT     1
#define RAO_NESTEDCOMMIT    2
#define RAO_NESTEDPREPARE   3
#define RAO_TOPLEVELABORT   4
#define RAO_TOPLEVELCOMMIT  5
#define RAO_TOPLEVELPREPARE 6

#define RAO_DONE	    7
#define RAO_NOTDONE         8
